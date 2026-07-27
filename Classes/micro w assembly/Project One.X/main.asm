; extra credit: noise maker - passive buzzer on D13

; Pin mapping:
; PORTB:
;   PB5 (D13) - passive buzzer
;   PB4 (D12) - light LED
;   PB3 (D11) - segment b
;   PB2 (D10) - segment a
;   PB1 (D9)  - segment f
;   PB0 (D8)  - segment g
;
; PORTD:
;   PD7 (D7)  - decimal point (unused, kept off)
;   PD6 (D6)  - segment c
;   PD5 (D5)  - segment d
;   PD4 (D4)  - segment e
;   PD3 (D3)  - reset button
;   PD2 (D2)  - start button

.equ START_BTN   = 2       ; PD2 - start button
.equ RESET_BTN   = 3       ; PD3 - reset button
.equ LED_BIT     = 4       ; PB4 - light LED
.equ BUZZER_BIT  = 5       ; PB5 - passive buzzer

; ------------ RESET / INIT ------------

main:
    ; Stack pointer init
    ldi r16, high(ramend)
    out sph, r16
    ldi r16, low(ramend)
    out spl, r16

    ; PORTB: PB0..PB5 outputs (segments g,f,a,b + LED + buzzer)
    ldi r16, 0b00111111        ; PB0..PB5 as outputs
    out DDRB, r16

    ; PORTD: PD4..PD7 outputs (segments e,d,c,dp), PD2,PD3 inputs (buttons)
    ldi r16, 0b11110000        ; PD7..PD4 outputs, PD3..PD2 inputs, PD1..PD0 inputs
    out DDRD, r16

    ; Enable pull-ups on start (PD2) and reset (PD3)
    ldi r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16             ; pull-ups on buttons, others low

    ; Ensure LED and buzzer off
    ldi r16, 0x00
    out PORTB, r16

    rjmp reset_system          ; go to idle "0" state

; ------------ MAIN LOOP ------------

main_loop:
    ; wait for start button press (active low on PD2 with pull-up)
    sbis PIND, START_BTN       ; skip next if bit is set (not pressed)
    rcall count_loop           ; if bit is 0 (pressed), run countdown
    rjmp main_loop

; ------------ COUNTDOWN 9 -> 0 ------------

count_loop:
    ; Digit 9
    ; PORTB = 0b00001111 (g,f,a,b)
    ; PORTD = 0b01100000 (c,d,e) + pull-ups on PD2,3 (0b00001100)
    ldi r16, 0b00001111
    out PORTB, r16
    ldi r16, 0b01100000
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 8
    ldi r16, 0b00001111        ; PB pattern for 8
    out PORTB, r16
    ldi r16, 0b01110000        ; PD pattern for 8
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 7
    ldi r16, 0b00001100        ; PB pattern for 7
    out PORTB, r16
    ldi r16, 0b01000000        ; PD pattern for 7
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 6
    ldi r16, 0b00000111        ; PB pattern for 6
    out PORTB, r16
    ldi r16, 0b01110000        ; PD pattern for 6
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 5
    ldi r16, 0b00000111        ; PB pattern for 5
    out PORTB, r16
    ldi r16, 0b01100000        ; PD pattern for 5
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 4
    ldi r16, 0b00001011        ; PB pattern for 4
    out PORTB, r16
    ldi r16, 0b01000000        ; PD pattern for 4
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 3
    ldi r16, 0b00001101        ; PB pattern for 3
    out PORTB, r16
    ldi r16, 0b01100000        ; PD pattern for 3
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 2
    ldi r16, 0b00001101        ; PB pattern for 2
    out PORTB, r16
    ldi r16, 0b00110000        ; PD pattern for 2
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 1
    ldi r16, 0b00001000        ; PB pattern for 1
    out PORTB, r16
    ldi r16, 0b01000000        ; PD pattern for 1
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall delay

    ; Digit 0 (normal display at the end of countdown)
    ldi r16, 0b00001110        ; PB pattern for 0
    out PORTB, r16
    ldi r16, 0b01110000        ; PD pattern for 0
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16
    rcall launch_and_noise

    rjmp reset_system

; ------------ ZERO + LED + BUZZER ------------

launch_and_noise:
    ; Show 0 with LED and buzzer ON for ~1 second

    ; base pattern for digit 0 on segments
    ldi r16, 0b00001110                ; PB: segments g,f,a,b for 0
    ori r16, (1<<LED_BIT)|(1<<BUZZER_BIT)   ; turn on LED + buzzer
    out PORTB, r16

    ldi r16, 0b01110000                ; PD: segments c,d,e for 0
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16

    rcall delay                        ; ~1 second with LED + buzzer on

    ; turn LED and buzzer off (keep showing 0)
    ldi r16, 0b00001110                ; segments only, LED/buzzer off
    out PORTB, r16
    ldi r16, 0b01110000
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16

    ret

; ------------ RESET / IDLE (SHOW 0) ------------

reset_system:
    ; LED + buzzer off, 7-seg shows 0 and stays there

    ldi r16, 0b00001110        ; PB pattern for digit 0 (segments only)
    out PORTB, r16

    ldi r16, 0b01110000        ; PD pattern for digit 0
    ori r16, (1<<START_BTN)|(1<<RESET_BTN)
    out PORTD, r16

    rjmp main_loop

; ------------ DELAY (~1 SECOND) ------------

delay:
    ldi r16, 0x00
    out TCCR0A, r16
    ldi r16, (1<<CS02)|(1<<CS00)   ; prescaler = 1024
    out TCCR0B, r16
    ldi r18, 61                    ; number of overflows for ~1 s

delay_loop:
    ldi r17, 0x00
    out TCNT0, r17                 ; reset counter to 0

wait_overflow:
    ; reset button check - active low on PD3 with pull-up
    sbis PIND, RESET_BTN           ; skip next if reset not pressed
    rjmp reset_system              ; if pressed, go to idle 0 immediately

    in  r16, TIFR0                 ; read flag register
    sbrs r16, TOV0                 ; skip if overflow flag not set
    rjmp wait_overflow

    ldi r16, (1<<TOV0)
    out TIFR0, r16                 ; clear overflow flag
    dec r18
    brne delay_loop
    ret
