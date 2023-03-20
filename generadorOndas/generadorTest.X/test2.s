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
  
restart_tmr0 macro  ;timer para definir la frecuencia de la onda
    banksel PORTA
    movf newMirror, W
    call tablaFrec
    movwf TMR0	;periodo = (4 * t_osc)(256-TMR0)(PRESCALER)
    bcf T0IF
    endm
  
restart_tmr1 macro  ;timer para rotar los displays
    movlw 0xFC 
    movwf TMR1H
    movlw 0x18 
    movwf TMR1L	;4ms
    bcf TMR1IF
    endm
    
PSECT udata_bank0
    var:		DS 1	;variable temporal para hacer division
    centenas:		DS 1	;variable para display uno
    decenas:		DS 1	;variable para display dos
    unidades:		DS 1	;variable para display tres
    display_var:	DS 5	;variable para guardar los valores que se cargan a los displays
    cont:		DS 1	;variable llevar la cuanta de los contadores
    new:		DS 1	;variable para llevar la cuenta del contador de frecuencias
    new2:		DS 1	;variable para cuenta interna de onda rec y trian
    new3:		DS 1	;variable para conversion de cuenta de lineal a triangular	
    key2:		DS 1	;variable/toggle para rotar entre los tipos de onda	
    newMirror:		DS 1	;variable espejo para seleccionar la frecuencia de la tabla
  
    
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
    call int_tx
    btfsc RBIF
    call int_iocb
    btfsc TMR1IF
    call int_t0
    
pop:
    swapf STATUS_TEMP, W
    movwf STATUS
    swapf W_TEMP, F
    swapf W_TEMP, W
    retfie
    
;------------subrutina de interrupcion----------------
    
int_tx:		    ;"switch case" para los tipos de onda
    movf key2, w
    sublw 00000000B
    btfsc STATUS, 2
    goto int_t1
    movf key2, w
    sublw 00000001B
    btfsc STATUS, 2
    goto int_t2
    movf key2, w
    sublw 00000010B
    btfsc STATUS, 2
    goto int_t3
    return
    
int_t0:		    ;rutina para alternar y cambiar los displays
    restart_tmr1
    banksel PORTA
    movf new, w
    sublw 00010011B
    btfsc STATUS, 2
    bsf PORTD, 3
    movf new, w
    sublw 00010010B
    btfsc STATUS, 2
    bcf PORTD, 3
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
display0:	    ;display de las centenas
    incf cont
    movf new, w
    sublw 00010010B ;si la frecuencia es mayor 210 se carga con "0." al display
    btfss STATUS, 0
    bcf display_var, 7
    movf display_var, W
    movwf PORTC
    bcf PORTD, 2
    goto siguiente
display1:	    ;display de las decenas
    incf cont
    movf display_var+1, W
    movwf PORTC
    bcf PORTD, 1
    goto siguiente
display2:	    ;display de las unidades
    incf cont
    movf display_var+2, W
    movwf PORTC
    bcf PORTD, 0
    goto siguiente
siguiente:	    ;ruinta intermedia para continuar el loop y resetear cont
    movf cont, w
    sublw 4
    btfsc STATUS, 2 ;si la cuenta de cont es igual a 3 se reinicia el contador
    clrf cont
    btfsc STATUS, 2
    incf cont
    return
   
int_t1:		    ;subrutina para onda rectangular
    restart_tmr0
    incf new2
    movf new2, w
    sublw 01000000B ;si el contador es mayor a 63 se reinicia a 0 (0-63)
    btfsc STATUS, 2
    clrf new2
    movf new2, w
    sublw 00011111B ;si el contador es menor o igual a 31 el DAC se pone en 0
    btfsc STATUS, 0 ;si el contador es mayor a 31 el DAC se ponde en 255
    goto before
    goto after  
before:		    ;cambia la onda a 0
    movlw 00000000B
    movwf PORTA
    return
after:		    ;cambia la onda a 255
    movlw 11111111B
    movwf PORTA
    return
    
int_t2:		    ;subrutina para onda triangular
    restart_tmr0
    incf new2
    movf new2, w
    sublw 01000000B ;si el contador es mayor a 63 se reinicia a 0 (0-63)
    btfsc STATUS, 2
    clrf new2
    movf new2, w
    sublw 00000000B ;si el contador new2 es igual a 0 el DAC se pone en 0
    btfsc STATUS, 2
    goto zero
    movf new2, w
    sublw 00100000B ;si el contador new2 es igual a 32 se le suma 7 al contador
    btfsc STATUS, 2 ;new3 que lleva la cuenta de la conversion de onda triangular
    goto top
    movf new2, w
    sublw 00011111B ;si el contador es menor o igual a 31 se le suma 8 al contador
    btfsc STATUS, 0 ;new3 que lleva la cuanta de la conversión de onda triangular
    goto upR	    ;si es mayor a 31 se le resta 8 al contador new3
    goto downR 
zero:
    clrf new3
    movf new3, w
    movwf PORTA
    return
top:
    movlw 00000111B
    addwf new3, 1
    movf new3, w
    movwf PORTA
    return
upR:
    movlw 00001000B
    addwf new3, 1
    movf new3, w
    movwf PORTA
    return
downR:
    movlw 00001000B
    subwf new3, 1
    movf new3, w
    movwf PORTA
    return
    
int_t3:		    ;subrutina de la onda sin
    restart_tmr0
    incf new2
    movf new2, w
    sublw 01000000B ;si el contador es mayor a 63 se reinicia a 0 (0-63)
    btfsc STATUS, 2
    clrf new2
    movf new2, w    ;el contador new2 se lleva a la tablaSin la cual
    call tablaSin   ;tiene mapeados los valores para la onda sin
    movwf PORTA
    return
    
int_iocb:	    ;subrutina de interrupcoin de botones
    banksel PORTA
    btfss PORTB, 5  ;rotar entre tipos de onda
    goto wave
    btfss PORTB, 6  ;aumentar display
    goto displayPlus
    btfss PORTB, 7  ;reducir display
    goto displayMinus
    return    
wave:
    bcf RBIF
    clrf new2	    ;se aumenta el contador para recorrer los pedazos de onda
    incf key2	    ;se incrementa key2 para rotar el tipo de onda
    movf key2, w
    sublw 00000011B ;se reinicia key2 si es mayor 2
    btfsc STATUS, 2
    clrf key2
    return 
displayPlus:	    
    bcf RBIF
    incf new	    ;aumenta el contador principal
    incf newMirror  ;aumenta el espejo del contador principal
    movf new, w
    sublw 00010110B ;reinicia el contador si es igual a 22 igual que al espejo
    btfsc STATUS, 2
    decf new
    movf newMirror,w
    sublw 00010110B
    btfsc STATUS, 2
    decf newMirror
    return
displayMinus:
    bcf RBIF
    decf new	    ;reduce el contador principal
    decf newMirror  ;reduce el espejo del contaodr principal
    movf new, w
    sublw 11111111B ;reinicial el contador si es igual a 255 igual que al espejo
    btfsc STATUS, 2
    incf new
    movf newMirror, w
    sublw 11111111B
    btfsc STATUS, 2
    incf newMirror
    return
    
;----------------codigo principal--------------------
    
PSECT code, delta=2, abs
 ORG 100h   ;posicion para el codigo
tabla:		    ;tabla para los valores de los displays
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

tablaFrec:	    ;tabla para los valores de frecuencia que se cargan al tmr0
    clrf PCLATH
    bsf PCLATH, 0   ; PCLATCH = 01
    addwf PCL	    ; PC = PCLATH + PCL
	retlw	11110100B; 244 010Hz 1:256
	retlw	11111010B; 250 020Hz 1:256
	retlw	11111100B; 252 030Hz 1:256
	retlw	11111101B; 253 040Hz 1:256
	retlw	11111011B; 251 050Hz 1:128
	retlw	11111100B; 252 060Hz 1:128
	retlw	11110011B; 243 070Hz 1:032
	retlw	11110101B; 245 080Hz 1:032
	retlw	11110110B; 246 090Hz 1:032
	retlw	11110111B; 247 100Hz 1:032
	retlw	11111000B; 248 110Hz 1:032
	retlw	11110010B; 242 120Hz 1:016
	retlw	11110100B; 245 130Hz 1:016
	retlw	10100100B; 164 140Hz 1:002 
	retlw	10101010B; 170 150Hz 1:002 
	retlw	11011000B; 216 160Hz 1:004 
	retlw	10111110B; 190 170Hz 1:002 
	retlw	11101101B; 237 180Hz 1:008
	retlw	11001000B; 200 210Hz 1:002
	retlw	11010010B; 210 260Hz 1:002
	retlw	11100110B; 230 350Hz 1:002
	retlw	11110000B; 240 500Hz 1:002
    
tablaValDisplay:    ;tabla para los valores de los que se calcularon las frecuencias
    clrf PCLATH
    bsf PCLATH, 0   ; PCLATCH = 01
    addwf PCL	    ; PC = PCLATH + PCL
	retlw	00001010B; 010
	retlw	00010100B; 020
	retlw	00011110B; 030
	retlw	00101000B; 040
	retlw	00110010B; 050
	retlw	00111100B; 060
	retlw	01000110B; 070
	retlw	01010000B; 080
	retlw	01011010B; 090
	retlw	01100100B; 100
	retlw	01101110B; 110
	retlw	01111000B; 120
	retlw	10000010B; 130
	retlw	10001100B; 140
	retlw	10010110B; 150
	retlw	10100000B; 160
	retlw	10101010B; 170
	retlw	10110100B; 180
	retlw	11010010B; 210
	retlw	00011010B; 0.26k
	retlw	00100011B; 0.35k
	retlw	00110010B; 0.50k

tablaSin:	    ;tabla mapeada para la onda sin
    clrf PCLATH
    bsf PCLATH, 0   ; PCLATCH = 01
    addwf PCL	    ; PC = PCLATH + PCL
	retlw	10000000B
	retlw	10001100B
	retlw	10011000B
	retlw	10100101B
	retlw	10110000B
	retlw	10111100B
	retlw	11000110B
	retlw	11010000B
	retlw	11011010B
	retlw	11100010B
	retlw	11101010B
	retlw	11110000B
	retlw	11110101B
	retlw	11111010B
	retlw	11111101B
	retlw	11111110B
	retlw	11111111B
	retlw	11111110B
	retlw	11111101B
	retlw	11111010B
	retlw	11110101B
	retlw	11110000B
	retlw	11101010B
	retlw	11100010B
	retlw	11011010B
	retlw	11010000B
	retlw	11000110B
	retlw	10111100B
	retlw	10110000B
	retlw	10100101B
	retlw	10011000B
	retlw	10001100B
	retlw	10000000B
	retlw	01110011B
	retlw	01100111B
	retlw	01011010B
	retlw	01001111B
	retlw	01000011B
	retlw	00111001B
	retlw	00101111B
	retlw	00100101B
	retlw	00011101B
	retlw	00010101B
	retlw	00001111B
	retlw	00001010B
	retlw	00000101B
	retlw	00000010B
	retlw	00000001B
	retlw	00000000B
	retlw	00000001B
	retlw	00000010B
	retlw	00000101B
	retlw	00001010B
	retlw	00001111B
	retlw	00010101B
	retlw	00011101B
	retlw	00100101B
	retlw	00101111B
	retlw	00111001B
	retlw	01000011B
	retlw	01001111B
	retlw	01011010B
	retlw	01100111B
	retlw	01110011B
	
tablaSetups:	    ;tabla para cambiar el prescaler
    clrf PCLATH
    bsf PCLATH, 0   ; PCLATCH = 01
    addwf PCL	    ; PC = PCLATH + PCL
    goto setup256   ;10
    goto setup256   ;20
    goto setup256   ;30
    goto setup256   ;40
    goto setup128   ;50
    goto setup128   ;60
    goto setup032   ;70
    goto setup032   ;80
    goto setup032   ;90
    goto setup032   ;100
    goto setup032   ;110
    goto setup016   ;120
    goto setup016   ;130
    goto setup002   ;140
    goto setup002   ;150
    goto setup004   ;160
    goto setup002   ;170
    goto setup008   ;180
    goto setup002   ;210
    goto setup002   ;260
    goto setup002   ;350
    goto setup002   ;500
    
;----------configuracion------------

setup256:
    banksel TRISA
    bsf PS2 ;1:256
    bsf PS1
    bsf PS0
    restart_tmr0
    return
    
setup128:
    banksel TRISA
    bsf PS2 ;1:128
    bsf PS1
    bcf PS0
    restart_tmr0
    return
   
setup064:
    banksel TRISA
    bsf PS2 ;1:064
    bcf PS1
    bsf PS0
    restart_tmr0
    return
    
setup032:
    banksel TRISA
    bsf PS2 ;1:032
    bcf PS1
    bcf PS0
    restart_tmr0
    return
    
setup016:
    banksel TRISA
    bcf PS2 ;1:016
    bsf PS1
    bsf PS0
    restart_tmr0
    return
    
setup008:
    banksel TRISA
    bcf PS2 ;1:008
    bsf PS1
    bcf PS0
    restart_tmr0
    return
    
setup004:
    banksel TRISA
    bcf PS2 ;1:004
    bcf PS1
    bsf PS0
    restart_tmr0
    return
    
setup002:
    banksel TRISA
    bcf PS2 ;1:002
    bcf PS1
    bcf PS0
    restart_tmr0
    return

 main:
    call config_io
    call config_reloj
    call config_tmr0_ie
    call config_tmr1	;no aumenta el contador
    call config_int_enable
    call config_iocrb
    movlw 00000000B	;se inicializan las variables en 0 o 1
    movwf new2
    movwf new3
    movwf key2
    movwf new
    movwf newMirror
    movlw 1
    movwf cont
    banksel PORTA
    
loop:
    movf new, w
    call tablaValDisplay;se convierte el contador a los valores calculados
    movwf var		;se carga el valor del contador de 8 bits en una variable para dividir
    movlw 00000000B
    movwf centenas	;si inicializa las centenas, decenas y unidade en 00000000B
    movwf decenas
    movwf unidades
    call separar
    call preparar_displays
    movf newMirror, w
    call tablaSetups
    goto loop
    
separar:		;se hace la division
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
    movlw 01000000B; 0.
    movwf display_var+3
    return

config_io:
    banksel ANSEL
    clrf ANSEL
    clrf ANSELH
    
    banksel TRISA
    clrf TRISA		;puerto A como salida del DAC
    clrf TRISC		;puerto C como salida de los displays
    bcf TRISD, 0	;puerto D 0-2 com switches de los displays
    bcf TRISD, 1	;puerto D 3 como indicador de hz(apagadp) o khz(encendido)
    bcf TRISD, 2
    bcf TRISD, 3	
    bsf TRISB, 5	;puerto B5 como alternador de ondas
    bsf TRISB, 6	;puerto B6 para incrementar el contador principal
    bsf TRISB, 7	;puerto B7 para reducir el contador principal
    bcf OPTION_REG, 7	;habilitar pullups
    bsf WPUB, 5
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
    bsf IRCF2	; 8mhz
    bsf IRCF1
    bsf IRCF0
    bsf SCS
    return
    
config_tmr0_ie:
    banksel TRISA
    bcf T0CS
    bcf PSA ;1:8
    bcf PS2
    bsf PS1
    bcf PS0
    restart_tmr0
    return
    
config_tmr1:
    banksel PORTA
    bcf TMR1GE	;siempre contando
    bsf T1CKPS1	;prescaler 8:1
    bsf	T1CKPS0
    bcf T1OSCEN	;reloj interno
    bcf TMR1CS
    bsf TMR1ON	;prender tmr1
    restart_tmr1
    return
    
config_int_enable:
    banksel PORTA
    bsf GIE	;interrupciones globales
    bsf T0IE	;habilitar tmr0
    bcf T0IF	;bandera tmr0
    bsf RBIE
    bcf RBIF
    bsf PEIE	;interrupciones perifericas
    bcf TMR1IF	;bandera tmr1
    banksel TRISA
    bsf TMR1IE
    return
    
config_iocrb:
    banksel TRISA
    bsf IOCB, 5
    bsf IOCB, 6
    bsf IOCB, 7
    
    banksel PORTA
    movf PORTB, W   ;al leer termina la condicion de ser distintos
    bcf RBIF
    
    return
END


