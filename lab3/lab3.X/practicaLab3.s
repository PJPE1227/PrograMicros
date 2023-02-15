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

PSECT recVect, class=CODE, abs, delta=2
ORG 00h
  
resetVec:
    PAGESEL setup
    goto setup
  
PSECT code, delta=2, abs
ORG 100h
 
setup:
    call config_reloj
    call config_io
    call config_tmr0
    banksel PORTA
    
loop:
    btfss T0IF
    goto $-1
    call reiniciar_tmr0
    incf PORTA
    movf PORTA, W
    sublw 00001010B
    btfsc STATUS, 2
    ;btfsc PORTA, 3
    call check
    btfsc PORTB, 6
    call inc_portb
    btfsc PORTB, 7
    call dec_portb
    movf PORTB, W 
    call tabla
    movwf PORTC
    goto loop
   
config_io:
    banksel ANSEL
    clrf ANSEL	    ; puertos como digitales
    clrf ANSELH
    
    banksel TRISA
    clrf TRISA	    ; puerto A como salida del tmr0
    clrf TRISC	    ; puerto C como salida de 7segmentor
    clrf TRISD	    ; puerto D como salida de contador alarma
    clrf TRISB	    ; puerto B contador binario del display de 7 segmentos
    clrf TRISE	    ; puerto E como salida de la alarma
    bsf TRISB, 6    ; boton de incremento
    bsf TRISB, 7    ; boton de decremento
    
    banksel PORTA
    clrf PORTA
    clrf PORTC
    clrf PORTB
    clrf PORTD
    clrf PORTE
    
    return
    
config_reloj:
    banksel OSCCON
    bcf IRCF2	; IRCF = 010 = 250kHz
    bsf IRCF1
    bcf IRCF0
    bsf SCS
    return
    
config_tmr0:
    banksel TRISA
    bcf T0CS	; reloj interno
    bcf PSA	; prescaler
    bsf PS2
    bsf PS1
    bsf PS0	; PS = 111 = 1:256
    banksel PORTA
    call reiniciar_tmr0
    return
    
reiniciar_tmr0:
    movlw 232	; configuración para 100 msen el contador 1
    movwf TMR0
    bcf T0IF
    return
   
tabla:
    clrf PCLATH
    bsf PCLATH, 0   ; PCLATCH = 01
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
    
inc_portb:
    btfsc PORTB, 6  ;antirebote
    goto $-1
    incf PORTB
    btfsc PORTB, 4  ;reiniciar el contador cuando el bit 4 se prenderia
    clrf PORTB
    return
    
dec_portb:
    btfsc PORTB, 7  ;antirebote
    goto $-1
    decf PORTB
    btfsc PORTB, 4 ; no permitir que el contador vaya en reversa desde 0
    clrf PORTB
    return
   
check:
    clrf PORTA	    ; reiniciar contador 1 al llegar a 10
    incf PORTD	    ; incrementar el contador de segundos
    btfsc PORTD, 4  ; reiniciar el contador de segundos si va a pasar de 15
    clrf PORTD
    movf PORTD, W   ; convertir el valor binario desplay para comparar
    call tabla
    subwf PORTC, W  ; comparar el valor convertido con el valor del display
    btfsc STATUS, 2
    call alarm
    return
alarm:
    clrf PORTD	    ;reiniciar el contador de segundos
    incf PORTE	    ; cambiar el estado de la alarma
    btfsc PORTE, 1  
    clrf PORTE
    return
end