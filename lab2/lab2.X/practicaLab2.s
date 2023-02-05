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
  
PSECT udata_bank0
  
PSECT resVect, class=CODE, abs, delta=2
ORG 00h
resetVec:
    PAGESEL main
    goto main
    
PSECT code, delta=2, abs
ORG 100h

 
main:
    call config_io
    call config_reloj
    banksel PORTA
    
loop:
    btfsc PORTB, 0
    call inc_porta
    btfsc PORTB, 1
    call dec_porta
    btfsc PORTB, 2
    call inc_portc
    btfsc PORTB, 5
    call dec_portc
    btfsc PORTB, 4
    call add_porta_and_portc
    goto loop
    
inc_porta:
    btfsc PORTB, 0  ;antirebote
    goto $-1
    incf PORTA
    btfsc PORTA, 4  ;reiniciar el contador cuando el bit 4 se prenderia
    clrf PORTA
    return
    
dec_porta:
    btfsc PORTB, 1  ;antirebote
    goto $-1
    decf PORTA
    return
 
inc_portc:
    btfsc PORTB, 2  ;antirebote
    goto $-1
    incf PORTC
    btfsc PORTC, 4  ;reiniciar el contador cuando el bit 4 se prenderia
    clrf PORTC
    return
    
dec_portc:
    btfsc PORTB, 5  ;antirebote
    goto $-1
    decf PORTC
    return    
    
add_porta_and_portc:
    btfsc PORTB, 4  ;antirebote
    goto $-1
    movf PORTA, 0   ;mover actual del contador 1 a W
    addwf PORTC, 0  ;sumar W con el contador 2 y guardarlo en W
    movwf PORTD	    ;mover W al puerto D
    return
    
config_io:
    bsf STATUS, 5
    bsf STATUS, 6
    clrf ANSEL	    ;configurar los pines como digitales
    clrf ANSELH
    
    bsf STATUS, 5
    bcf STATUS, 6
    clrf TRISA	    ;puerto A como salida contador 1
    clrf TRISC	    ;puerto C como salida contador 2
    clrf TRISD	    ;puerto D como salida sumador
    bsf TRISB, 0    ;puerto 0 de B para incrementar cuenta de contador 1
    bsf TRISB, 1    ;puerto 1 de B para reducir cuenta de contador 1
    bsf TRISB, 2    ;puerto 1 de B para incrementar cuenta de contador 2
    ;bsf TRISB, 3   ; el puerto 3 no funcionaba durante la practica
    bsf TRISB, 4    ;puerto 4 de B para reducir cuenta de contador 2
    bsf TRISB, 5    ;puerto 5 de B  para calcular la suma entre contadores
    
    bcf STATUS, 5
    bcf STATUS, 6
    clrf PORTA
    clrf PORTB
    clrf PORTC
    clrf PORTD
    
    return
    
config_reloj:
    banksel OSCCON
    bcf IRCF2	;configuración 100 del IRFC para 1MHz
    bsf IRCF1
    bsf IRCF0
    bsf SCS
    return
 
END


