.equ LED_BIT = 0			    ; defines constant bit numbers inside the ports
.equ START_BTN = 4			    ; how to select individual bits in the register
.equ ABORT_BTN = 5			    ; bit 0 of PORTD (LED), bit 4 of PORTB (D12), bit 5 of PORTB (D13)


main:
    ldi r16, high(ramend)		    ; Stack Pointer Initialization
    out sph, r16			    ; These 4 lines make rcall and ret instructions work (push/pop)
    ldi r16, low(ramend)
    out spl, r16
    
    ldi r16, 0xff			    ; load imm of all 1s into reg 16
    out DDRD, r16			    ; Data Direction Reg writing to port D 1 means output
    ldi r16, (1<<START_BTN)|(1<<ABORT_BTN)  ; Puts pull up resistors on PB4 and 5 by ORing them togeather to make 00110000 High normally Low when pressed
    out PORTB, r16			    ; loads r16 into PORTB
    ldi r16, 0x00			    ; load imm to 0
    out DDRB, r16			    ; makes PB4 and 5 into inputs bc 0 is input
    
main_loop:
    sbis PINB, START_BTN		    ; Skip next instruction if bit in 1/0 reg is 0 
    rcall count_loop			    ; recalls countdown sub routine if PB4 is pressed
    rjmp main_loop			    ; loops forever checking forever

count_loop:
    ldi r16, 0b11011110   ; (9)
    out PORTD, r16
    rcall delay

    ldi r16, 0b11111110   ; (8)
    out PORTD, r16
    rcall delay

    ldi r16, 0b00001110   ; (7)
    out PORTD, r16
    rcall delay

    ldi r16, 0b11111010   ; (6)
    out PORTD, r16
    rcall delay
    
    ldi r16, 0b11011010   ; (5)
    out PORTD, r16
    rcall delay
    
    ldi r16, 0b11001100   ; (4)
    out PORTD, r16
    rcall delay
    
    ldi r16, 0b10011110   ; (3)
    out PORTD, r16
    rcall delay
    
    ldi r16, 0b10110110   ; (2)
    out PORTD, r16
    rcall delay
    
    ldi r16, 0b00001100   ; (1)
    out PORTD, r16
    rcall delay
    
    ldi r16, 0b01111110   ; (0)
    out PORTD, r16
    rcall delay
    
launch:
    sbi PORTD, LED_BIT			    ; set bit in 1/0 reg to 0 (Turns LED on)
    rcall delay				    ; holds the number for 1 sec
    cbi PORTD, LED_BIT			    ; clears bit in 1/0 reg (LED OFF)
    
reset_system:
    cbi PORTD, LED_BIT			    ; clears bit in 1/0 reg (LED OFF)
    clr r17				    ; clears reg 17
    out PORTD, r17			    ; writes 0 to all PORTD pins (7 seg OFF)
    rjmp main_loop			    ; returns to main loop
      
delay:
    ldi r16, 0x00
    out TCCR0A, r16
    ldi r16, (1<<CS02)|(1<<CS00)   ; prescaler = 1024
    out TCCR0B, r16
    ldi r18, 61                ; number of overflows for ~1 s
delay_loop:
    ldi r17, 0x00
    out TCNT0, r17             ; reset counter to 0
wait_overflow:
    sbis PINB, ABORT_BTN		    ; skip next instruction if bit on abort_btn is set (HIGH) 
    rjmp reset_system			    ; if abort_btn is pressed (LOW) then go to reset_system
    
    in  r16, TIFR0             ; read flag register
    sbrs r16, TOV0             ; skip if overflow flag not set
    rjmp wait_overflow

    ldi r16, (1<<TOV0)
    out TIFR0, r16             ; clear flag by writing 1
    dec r18
    brne delay_loop
    ret
    