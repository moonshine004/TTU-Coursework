; ============================================================
;  AVR Countdown (5 ? 0) With 15 Second Fan Spin
;  Matches Amna?s wiring exactly
;  COMMON CATHODE 7-segment
; ============================================================

; Pin definitions
.equ FAN_BIT   = 5        ; PB5 = D13
.equ LIGHT_BIT = 4        ; PB4 = D12
.equ START_BTN = 2        ; PD2 = D2
.equ RESET_BTN = 3        ; PD3 = D3

; ------------------------------------------------------------
; Segment Pin Mapping:
; PORTB bits:
;   PB3 = D11 = b
;   PB2 = D10 = a
;   PB1 = D9  = f
;   PB0 = D8  = g
;
; PORTD bits:
;   PD7 = D7  = dp
;   PD6 = D6  = c
;   PD5 = D5  = d
;   PD4 = D4  = e
;
; dp always off
; ------------------------------------------------------------

; Digit patterns (PORTB + PORTD)
.equ DIG0_B = 0b00001110    ; 0
.equ DIG0_D = 0b01110000

.equ DIG1_B = 0b00001000    ; 1
.equ DIG1_D = 0b01000000

.equ DIG2_B = 0b00001101    ; 2
.equ DIG2_D = 0b00110000

.equ DIG3_B = 0b00001111    ; 3
.equ DIG3_D = 0b01100000

.equ DIG4_B = 0b00001011    ; 4
.equ DIG4_D = 0b01000000

.equ DIG5_B = 0b00000111    ; 5
.equ DIG5_D = 0b01100000

; ============================================================
; Initialization
; ============================================================

.org 0x0000
rjmp main

main:
    ; stack pointer
    ldi r16, high(RAMEND)
    out SPH, r16
    ldi r16, low(RAMEND)
    out SPL, r16

    ; PB0?PB5 outputs
    ldi r16, 0b00111111
    out DDRB, r16

    ; PD7?PD4 outputs, PD3?PD0 inputs
    ldi r16, 0b11110000
    out DDRD, r16

    ; enable pullups on start/reset buttons
    ldi r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16

    rjmp reset_system


; ============================================================
; Main loop: wait on start
; ============================================================

main_loop:
    sbis PIND, START_BTN     ; wait for D2 LOW
    rcall count_loop         ; run countdown
    rjmp main_loop


; ============================================================
; Countdown 5 ? 0
; ============================================================

count_loop:

    ; ----- 5 -----
    ldi r16, DIG5_B
    out PORTB, r16
    ldi r16, DIG5_D | (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; ----- 4 -----
    ldi r16, DIG4_B
    out PORTB, r16
    ldi r16, DIG4_D | (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; ----- 3 -----
    ldi r16, DIG3_B
    out PORTB, r16
    ldi r16, DIG3_D | (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; ----- 2 -----
    ldi r16, DIG2_B
    out PORTB, r16
    ldi r16, DIG2_D | (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; ----- 1 -----
    ldi r16, DIG1_B
    out PORTB, r16
    ldi r16, DIG1_D | (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; ----- 0 -----
    ldi r16, DIG0_B
    out PORTB, r16
    ldi r16, DIG0_D | (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16

    rjmp launch_and_fan


; ============================================================
; FAN + LIGHT FOR 15 SECONDS
; ============================================================

launch_and_fan:
    ; turn fan + light ON
    sbi PORTB, FAN_BIT        ; PB5
    sbi PORTB, LIGHT_BIT      ; PB4

    ; 15 × 1-second delays
    ldi r20, 15
fan_loop:
    rcall delay
    dec r20
    brne fan_loop

    ; turn fan + light OFF
    cbi PORTB, FAN_BIT
    cbi PORTB, LIGHT_BIT

    rjmp reset_system


; ============================================================
; Reset state = display 0 and wait
; ============================================================

reset_system:
    ; fan/light OFF
    cbi PORTB, FAN_BIT
    cbi PORTB, LIGHT_BIT

    ; display 0
    ldi r16, DIG0_B
    out PORTB, r16
    ldi r16, DIG0_D | (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16

    rjmp main_loop


; ============================================================
; 1-SECOND DELAY (checks reset button)
; ============================================================

delay:
    ldi r16, 0x00
    out TCCR0A, r16

    ldi r16, (1<<CS02)|(1<<CS00)   ; prescaler 1024
    out TCCR0B, r16

    ldi r18, 61        ; ~1 second

delay_loop:
    ldi r17, 0x00
    out TCNT0, r17

wait_overflow:
    sbis PIND, RESET_BTN
    rjmp reset_system

    in  r16, TIFR0
    sbrs r16, TOV0
    rjmp wait_overflow

    ldi r16, (1<<TOV0)
    out TIFR0, r16

    dec r18
    brne delay_loop

    ret
