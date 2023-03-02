; CONFIG1
  CONFIG  FOSC = INTRC_NOCLKOUT ; Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
  CONFIG  WDTE = OFF            ; Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
  CONFIG  PWRTE = OFF           ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  MCLRE = OFF           ; RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
  CONFIG  CP = OFF              ; Code Protection bit (Program memory code protection is disabled)
  CONFIG  CPD = OFF             ; Data Code Protection bit (Data memory code protection is disabled)
  CONFIG  BOREN = OFF           ; Brown Out Reset Selection bits (BOR disabled)
  CONFIG  IESO = OFF            ; Internal External Switchover bit (Internal/External Switchover mode is disabled)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
  CONFIG  LVP = OFF              ; Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

; CONFIG2
  CONFIG  BOR4V = BOR40V        ; Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
  CONFIG  WRT = OFF             ; Flash Program Memory Self Write Enable bits (Write protection off)

// config statements should precede project file includes.
#include <xc.inc>
PROCESSOR 16F887
  
restart_tmr0 macro
    banksel PORTA
    movlw 217	;periodo = (4 * t_osc)(256-TMR0)(PRESCALER) 10ms
    movwf TMR0
    bcf T0IF
    endm
  
PSECT udata_bank0
  var:		DS 1
  banderas:	DS 1
  nibble:	DS 2
  display_var:	DS 2
    
PSECT udata_shr
    W_TEMP: DS 1
    STATUS_TEMP: DS 1
    
PSECT resVect, class=CODE, abs, delta=2
;---------------------vector reset----------------
ORG 00h	    ;posicion 0000h para el reset
resetVec:
    PAGESEL main
    goto main
    
PSECT intVect, class=CODE, abs, delta=2
;--------------interrupt vector-------------
ORG 04h
    
push:
    movwf W_TEMP
    swapf STATUS, W
    movwf STATUS_TEMP
    
isr:
    btfsc T0IF
    call int_t0
    btfsc RBIF
    call int_iocb
    
pop:
    swapf STATUS_TEMP, W
    movwf STATUS
    swapf W_TEMP, F
    swapf W_TEMP, W
    retfie
    
;------------subrutina de interrupcion----------------

int_t0:
    restart_tmr0
    clrf PORTD
    btfsc banderas, 0
    goto display1 
display0:
    movf display_var, W
    movwf PORTC
    bsf PORTD, 1
    goto siguiente_display
display1:
    movf display_var+1, W
    movwf PORTC
    bsf PORTD, 0
siguiente_display:
    movlw 1
    xorwf banderas, F
    return
    
int_iocb:
    banksel PORTA
    btfss PORTB, 6
    incf PORTA
    btfss PORTB, 7
    decf PORTA
    bcf RBIF
    return

;----------------codigo principal--------------------
    
PSECT code, delta=2, abs
 ORG 100h   ;posicion para el codigo
tabla:
    clrf PCLATH
    bsf PCLATH, 0   ; PCLATCH = 01
    andlw 0x0f
    addwf PCL	    ; PC = PCLATH + PCL
    retlw 11000000B ;0
    retlw 11111001B ;1  
    retlw 10100100B ;2
    retlw 10110000B ;3
    retlw 10011001B ;4
    retlw 10010010B ;5
    retlw 10000010B ;6
    retlw 11111000B ;7
    retlw 10000000B ;8
    retlw 10010000B ;9
    retlw 10001000B ;a
    retlw 10000011B ;b
    retlw 11000110B ;c
    retlw 10100001B ;d
    retlw 10000110B ;e
    retlw 10001110B ;f
    
;----------configuracion------------

 main:
    call config_io
    call config_reloj
    call config_tmr0_ie
    call config_int_enable
    call config_iocrb
    banksel PORTA
    
loop:
    movf PORTA, W
    movwf var 
    call separar_nibbles
    call preparar_displays
    goto loop
    
separar_nibbles:
    movf var, W
    andlw 0x0f
    movwf nibble
    swapf var, W
    andlw 0x0f
    movwf nibble+1
    return
    
preparar_displays:
    movf nibble, W
    call tabla
    movwf display_var
    movf nibble+1, W
    call tabla
    movwf display_var+1
    return

config_io:
    banksel ANSEL
    clrf ANSEL
    clrf ANSELH
    
    banksel TRISA
    clrf TRISA
    clrf TRISC
    bcf TRISD, 0
    bcf TRISD, 1
    bsf TRISB, 6
    bsf TRISB, 7
    bcf OPTION_REG, 7	;habilitar pullups
    bsf WPUB, 6
    bsf WPUB, 7
    
    banksel PORTA
    clrf PORTA
    clrf PORTB
    clrf PORTC
    clrf PORTD
    return
    
config_reloj:
    banksel OSCCON
    bsf IRCF2	; 4mhz
    bsf IRCF1
    bcf IRCF0
    bsf SCS
    return
    
config_tmr0_ie:
    banksel TRISA
    bcf T0CS
    bcf PSA ;1:256
    bsf PS2
    bsf PS1
    bsf PS0
    restart_tmr0
    return
    
config_int_enable:
    bsf GIE
    bsf T0IE
    bcf T0IF
    bsf RBIE
    bcf RBIF
    return
    
config_iocrb:
    banksel TRISA
    bsf IOCB, 6
    bsf IOCB, 7
    
    banksel PORTA
    movf PORTB, W   ;al leer termina la condicion de ser distintos
    bcf RBIF
    
    return
END