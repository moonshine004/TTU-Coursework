// MSP-EXP430FR6989
// P1.1 = S1 (active-LOW), P1.0 = LED
// Start Timer_A on press; if still held after HOLD_MS -> LED ON.
// Release anytime -> stop/reset timer + LED OFF.

#include <msp430.h>

#define LED      BIT0          // P1.0
#define BTN      BIT1          // P1.1
#define HOLD_MS  1000          // CHANGE THE TIME HERE (milliseconds)

int main(void) {
    WDTCTL  = WDTPW | WDTHOLD;     // stop watchdog
    PM5CTL0 &= ~LOCKLPM5;          // enable GPIO (FRAM)

    P1DIR |=  LED;  P1OUT &= ~LED; // LED out, off
    P1DIR &= ~BTN;  P1REN |= BTN;  P1OUT |= BTN;   // button in, pull-up

    // Port1 IRQ: first watch HIGH->LOW (press)
    P1IES |= BTN;  P1IFG &= ~BTN;  P1IE |= BTN;

    // Sleep between events:
    // - LPM0 puts the CPU to sleep but keeps SMCLK running so Timer_A can count.
    // - GIE enables interrupts so the button/timer ISRs can run and wake the CPU.
    __bis_SR_register(LPM0_bits | GIE);

    // Optional: one no-op instruction; handy as a debugger breakpoint.
    __no_operation();
}

/* PORT1: start timer on press; stop/reset + LED off on release */
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void) {
    if (P1IFG & BTN) {
        if (P1IES & BTN) {                     // press (HIGH->LOW)
            TA0CTL  = TASSEL__SMCLK | ID__8 | MC__UP | TACLR; // SMCLK, /8, up
            TA0EX0  = TAIDEX_7;                                // extra /8 => /64
            TA0CCR0 = (unsigned int)((15625UL * HOLD_MS)/1000UL - 1UL); // ~HOLD_MS
            TA0CCTL0 = CCIE;                                   // enable CCR0 ISR
            P1IES &= ~BTN;                                     // next: release
        } else {                                               // release (LOW->HIGH)
            TA0CCTL0 = 0;  TA0CTL = MC__STOP;  TA0R = 0;       // stop/reset timer
            P1OUT &= ~LED;                                     // LED off
            P1IES |= BTN;                                      // next: press
        }
        P1IFG &= ~BTN;                                         // clear flag
    }
}

/* TIMER0_A0: fires after HOLD_MS; LED on only if still pressed */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void) {
    TA0CCTL0 = 0;  TA0CTL = MC__STOP;  TA0R = 0;               // one-shot done
    if ((P1IN & BTN) == 0) P1OUT |= LED;                       // still holding?
}
