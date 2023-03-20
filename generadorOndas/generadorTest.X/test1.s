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
    movlw 255	;periodo = (4 * t_osc)(256-TMR0)(PRESCALER) 4ms
    movwf TMR0
    bcf T0IF
    endm
    
restart_tmr1 macro
    movlw 0xB
    movwf TMR1H
    movlw 0x47
    movwf TMR1L
    bcf TMR1IF
    endm
    
PSECT udata_bank0
    cont: DS 2
    contD: DS 1
    var:		DS 1
    key:		DS 1
    banderas:		DS 1
    nibble:		DS 2
    centenas:		DS 1
    decenas:		DS 1
    unidades:		DS 1
    display_var:	DS 5
    contOne:		DS 1
     
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
    btfsc TMR1IF
    call int_t1
    
pop:
    swapf STATUS_TEMP, W
    movwf STATUS
    swapf W_TEMP, F
    swapf W_TEMP, W
    retfie

    
int_t0:
    restart_tmr0
    incf cont
    movf cont, W
    sublw 10
    btfss STATUS, 2
    goto return_t0
    clrf cont
    incf PORTA	;incrementar contador binario en puerto A
    movf PORTA, W
    
return_t0:
    return
 
int_t1:
    restart_tmr1
    bsf PORTD, 0
    bsf PORTD, 1
    bsf PORTD, 2
    movf cont, w
    sublw 1
    btfsc STATUS, 2
    goto display0   ;si la cuenta de cont es igual a 0 se muestran las centenas
    movf cont, w
    sublw 2
    btfsc STATUS, 2
    goto display1   ;si la cuenta de cont es igual a 1 se muestran las decenas
    movf cont, w
    sublw 3
    btfsc STATUS, 2
    goto display2   ;si la cuenta de cont es igual a 2 se muestran las unidades
    return
display0:
    incf cont
    movf display_var, W
    movwf PORTC
    bcf PORTD, 2
    goto siguiente
display1:
    incf cont
    movf display_var+1, W
    movwf PORTC
    bcf PORTD, 1
    goto siguiente
display2:
    incf cont
    movf display_var+2, W
    movwf PORTC
    bcf PORTD, 0
    goto siguiente
siguiente:
    movf cont, w
    sublw 4
    btfsc STATUS, 2 ;si la cuenta de cont es igual a 3 se reinicia el contador
    clrf cont
    btfsc STATUS, 2
    incf cont
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
    
main:
    call config_io
    call config_reloj
    call config_tmr0
    call config_int_enable
    call config_iocrb
    call config_tmr1
    movlw 00000000B
    movwf contOne
    banksel PORTA
    
loop:
    movf contOne, W
    movwf var	    ;se carga el valor del contador de 8 bits en una variable para dividir
    movlw 00000000B
    movwf centenas  ;si inicializa las centenas, decenas y unidade en 00000000B
    movwf decenas
    movwf unidades
    call separar
    goto loop
    
separar:
    movlw 01100100B
    subwf var, 1
    btfsc STATUS, 0
    incf centenas
    btfsc STATUS, 0
    goto $-5	    ;se le resta 100 al valor del contador hasta que el resultado sea negativo
    movlw 01100100B
    addwf var, 1    ;ahora se le suma 100 porque la ultima operacion dejo al valor en negativo
    movlw 00001010B 
    subwf var, 1
    btfsc STATUS, 0
    incf decenas
    btfsc STATUS, 0
    goto $-5	    ;se le resta 10 al valor del contador hasta que el resultado sea negativo
    movlw 00001010B
    addwf var, 1    ;ahora se le sumam 10 porque la ultimam operación dejo al valor en negativo
    movlw 00000001B
    subwf var, 1
    btfsc STATUS, 0
    incf unidades
    btfsc STATUS, 0
    goto $-5	    ;se le resta 1 al valor del contador hasta que el resultado sea negativo
    return
    
preparar_displays:
    movf centenas, W	;se cargan las centenas en el primer display
    call tabla
    movwf display_var
    movf decenas, W	;se cargan las decenas en el segundo display
    call tabla
    movwf display_var+1
    movf unidades, W	;se cargan las unidades en el tercer display
    call tabla
    movwf display_var+2
    return
    
config_io:
    banksel ANSEL
    clrf ANSEL
    clrf ANSELH
    
    banksel TRISA
    clrf TRISA
    clrf TRISC
    bcf TRISD, 0	;puerto D 0-2 com switches de los displays
    bcf TRISD, 1
    bcf TRISD, 2
    bsf TRISB, 6	;puerto B 0-1 como botones para contador
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
    bsf IRCF2	; 4 MHz
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
    
config_tmr1:
    banksel PORTA
    bcf TMR1GE	; siempre contando
    bsf T1CKPS1	; prescaler 8:1
    bsf T1CKPS0
    bcf T1OSCEN	; reloj enterno
    bcf TMR1CS
    bsf TMR1ON	; prender tmr1
    restart_tmr1
    return
   
config_int_enable:
    banksel TRISA
    bsf TMR1IE	; interrupcion tmr1
    banksel PORTA
    bsf GIE	; interrupciones globales
    bsf PEIE	; interrupciones perifericas
    bsf T0IE	; interrupcion tmr0
    bcf T0IF	; bandera tmr0
    bsf RBIE
    bcf RBIF
    bcf TMR1IF	; bandera tmr1
    return
    
config_iocrb:
    banksel TRISA
    bsf IOCB, 0
    bsf IOCB, 6
    bsf IOCB, 7
    
    banksel PORTA
    movf PORTB, W   ;al leer termina la condicion de ser distintos
    bcf RBIF
    
    return
END