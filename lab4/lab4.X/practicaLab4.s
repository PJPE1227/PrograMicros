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
    movlw 232	;periodo = (4 * t_osc)(256-TMR0)(PRESCALER)
    movwf TMR0
    bcf T0IF
    endm
  
PSECT udata_bank0
    cont: DS 2
    contD: DS 1
    
PSECT udata_shr
    W_TEMP:	    DS 1
    STATUS_TEMP:    DS 1
    idk: DS 1
    
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
    call int_T0
    btfsc RBIF
    call int_iocb
    
pop:
    swapf STATUS_TEMP, W
    movwf STATUS
    swapf W_TEMP, F
    swapf W_TEMP, W
    retfie
;-------surutina de interrupccion----------- 
int_T0:
    restart_tmr0
    incf cont
    movf cont, W
    sublw 10
    btfss STATUS, 2
    goto return_t0
    clrf cont
    incf PORTA	;incrementar contador binario en puerto A
    movf PORTA, W
    sublw 00001010B
    btfsc STATUS, 2
    clrf PORTA
    movf PORTA, W
    call tabla	;convertir valor binario del puerto A en valor para el diplay 7
    movwf PORTD	; poner el valor en el puerto D
    movf PORTA, W
    sublw 00000000B; comparar el valor del puerto A con 0 para incrementar las decenas
    btfsc STATUS, 2
    incf cont+1	;incrementar el display de las decenas en el puerto C
    movf cont+1, W
    call tabla
    movwf PORTC
    movf cont+1, W
    sublw 00000110B ;compara el valor de las decenas con 6 para reinicir el minuto
    btfsc STATUS, 2
    clrf cont+1
    return
    
return_t0:
    return
    
int_iocb:
    banksel PORTA
    btfss PORTB, 6
    incf PORTB
    btfss PORTB, 7
    decf PORTB
    btfsc PORTB, 4
    clrf PORTB
    bcf RBIF
    return
    
    
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
;----------configuracion------------
 main:
    call config_io
    call config_reloj
    call config_tmr0
    call config_int_enable
    call config_iocrb
    banksel PORTA
    
loop:
    goto loop
    
config_io:
    banksel ANSEL
    clrf ANSEL
    clrf ANSELH
    
    banksel TRISA
    clrf TRISA
    clrf TRISB
    clrf TRISC
    clrf TRISD
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
    clrf cont
    clrf cont+1
    return
    
config_reloj:
    banksel OSCCON
    bcf IRCF2	; 250 kHz
    bsf IRCF1
    bcf IRCF0
    bsf SCS
    return
    
config_tmr0:
    banksel TRISA
    bcf T0CS
    bcf PSA
    bsf PS2
    bsf PS1
    bsf PS0
    restart_tmr0
    return
    
restart_tmr0:
    banksel PORTA
    movlw 61	;periodo = (4 * t_osc)(256-TMR0)(PRESCALER)
    movwf TMR0
    bcf T0IF
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