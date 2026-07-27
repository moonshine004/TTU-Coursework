// MSP430FR6989 — Software PWM on P1.4 using Timer_A1 (DriverLib, CCS-ready)
// Period 20 ms, default 1.0 ms pulse; P1.1 press -> request 2.0 ms (clamped to 1.83 ms)

#include <stdint.h>
#include <stdbool.h>
#include "driverlib.h"

#define TAx_BASE               TIMER_A1_BASE

// Timing (SMCLK = 8 MHz, /16 => 2 us/tick)
#define PWM_PERIOD_TICKS       10000u          // 20 ms frame
#define TICKS_PER_MS           500u            // 1.0 ms -> 500 ticks

// Logical targets
#define PWM_1MS_TICKS          (1u * TICKS_PER_MS)      // 1.00 ms -> 500
#define PWM_2MS_TICKS          (2u * TICKS_PER_MS)      // 2.00 ms -> 1000

// MG996R trim clamp (bench-tune as needed)
#define OUT_MIN_MS             1.60f
#define OUT_MAX_MS             1.83f
#define OUT_MIN_TICKS          ((uint16_t)(OUT_MIN_MS * TICKS_PER_MS + 0.5f))  // 800
#define OUT_MAX_TICKS          ((uint16_t)(OUT_MAX_MS * TICKS_PER_MS + 0.5f))  // 915

// Pins
#define PWM_PORT               GPIO_PORT_P1
#define PWM_PIN                GPIO_PIN4        // P1.4 stays GPIO (software PWM)
#define BTN_PORT               GPIO_PORT_P1
#define BTN_PIN                GPIO_PIN1        // S1 on LaunchPad

static volatile uint16_t dutyTicks = PWM_1MS_TICKS;     // start at 1.00 ms

static inline uint16_t clampTicks(uint16_t req) {
    if (req < OUT_MIN_TICKS) return OUT_MIN_TICKS;
    if (req > OUT_MAX_TICKS) return OUT_MAX_TICKS;
    return req;
}

static inline void setDutyTicks(uint16_t reqTicks) {
    uint16_t t = clampTicks(reqTicks);
    if (t >= PWM_PERIOD_TICKS) t = PWM_PERIOD_TICKS - 1; // safety
    dutyTicks = t;
    Timer_A_setCompareValue(TAx_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, dutyTicks);
}

static inline void setDutyMs(float ms) {
    uint16_t req = (uint16_t)(ms * TICKS_PER_MS + 0.5f);
    setDutyTicks(req);
}

static void initClocks(void) {
    // DCO = 8 MHz; SMCLK = DCO
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_8);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}

static void initGPIO(void) {
    PMM_unlockLPM5();  // enable I/O on FRAM devices

    // PWM pin as GPIO output
    GPIO_setAsOutputPin(PWM_PORT, PWM_PIN);
    GPIO_setOutputLowOnPin(PWM_PORT, PWM_PIN);

    // Button with pull-up, falling-edge interrupt
    GPIO_setAsInputPinWithPullUpResistor(BTN_PORT, BTN_PIN);
    GPIO_selectInterruptEdge(BTN_PORT, BTN_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(BTN_PORT, BTN_PIN);
    GPIO_enableInterrupt(BTN_PORT, BTN_PIN);
}

static void initTimerA1(void) {
    // Timer_A1 up mode: SMCLK/16 => 2 us/tick; CCR0=9999; CCR0 interrupt ON
    Timer_A_initUpModeParam up = {0};
    up.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    up.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_16;
    up.timerPeriod = PWM_PERIOD_TICKS - 1;                     // CCR0 = 9999
    up.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    up.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
    up.timerClear = TIMER_A_DO_CLEAR;
    up.startTimer = true;
    Timer_A_initUpMode(TAx_BASE, &up);

    // CCR1 schedules the "turn-off" edge (interrupt only)
    Timer_A_initCompareModeParam c1 = {0};
    c1.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1;    // CCR1 = duty
    c1.compareInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    c1.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
    c1.compareValue = dutyTicks;
    Timer_A_initCompareMode(TAx_BASE, &c1);
}

int main(void) {
    WDT_A_hold(WDT_A_BASE);
    initClocks();
    initGPIO();
    initTimerA1();
    __enable_interrupt();

    while (1) {
        __bis_SR_register(LPM0_bits | GIE);
        __no_operation();
    }
}

/* ---------------- ISRs ---------------- */

// Start of each 20 ms frame -> set P1.4 HIGH
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt
#endif
void TIMER1_A0_ISR(void) {
    GPIO_setOutputHighOnPin(PWM_PORT, PWM_PIN);
    __bic_SR_register_on_exit(LPM0_bits);
}

// Duty edge -> set P1.4 LOW
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt
#endif
void TIMER1_A1_ISR(void) {
    switch (Timer_A_getInterruptVector(TAx_BASE)) {
        case TIMER_A_IV_TACCR1:
            GPIO_setOutputLowOnPin(PWM_PORT, PWM_PIN);
            break;
        default:
            break;
    }
    __bic_SR_register_on_exit(LPM0_bits);
}

// Button press -> request 2.00 ms; clamp limits to 1.83 ms (per your trim)
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt
#endif
void PORT1_ISR(void) {
    uint16_t flags = GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN_ALL8);

    if (flags & BTN_PIN) {
        GPIO_clearInterrupt(BTN_PORT, BTN_PIN);

        setDutyMs(2.00f);                 // request 2.0 ms -> clamped to 1.83 ms
        __delay_cycles(80000);            // ~10 ms debounce @ 8 MHz

        GPIO_clearInterrupt(BTN_PORT, BTN_PIN);
    }
    __bic_SR_register_on_exit(LPM0_bits);
}
