/*
 * File:   preLabLab6.c
 * Author: PJPE
 *
 * Created on 23 de marzo de 2023, 11:30 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#define _tmr0_value 1
#define _XTAL_FREQ 8000000

int cont = 0;
float adc = 0;
float adc1 = 0;
float cad = 0;
float centenas, unidades, decenas;
int d1 = 0;
int d2 = 0;
int d3 = 0;
int _displays(int a);
void __interrupt()isr(void){
    if(T0IF){
        PORTDbits.RD7 ++;
        TMR0 = _tmr0_value;
        T0IF = 0;
    }
    if(ADIF){
        cont ++;
        if(cont%2 == 0){
            __delay_ms(1);
            ADCON0bits.CHS = 0b0111;
            adc1 = ADRESH;
        }else{
            __delay_ms(1);
            ADCON0bits.CHS = 0b0110;
            cad = ADRESH;
            adc = (cad/255)*500;
        }
    }
}
void setup(void);
void main(void) {   
    setup();
    while(1){
        ADCON0bits.GO = 1;
        __delay_ms(1);
        centenas = (int)adc/100;
        d1 = _displays(centenas);
        decenas = ((int)adc%100)/10;
        d2 = _displays(decenas);
        unidades = ((int)adc%100)%10;
        d3 = _displays((int)unidades);
        PORTA = (char)adc1;
        __delay_ms(1);
        PORTC = 0;
        PORTC = (char)d1;
        PORTDbits.RD6 = 0;
        PORTDbits.RD5 = 1;
        PORTDbits.RD4 = 1;
        __delay_ms(1);
        PORTC = 0;
        PORTC = (char)d2;
        PORTDbits.RD6 = 1;
        PORTDbits.RD5 = 0;
        PORTDbits.RD4 = 1;
        __delay_ms(1);
        PORTC = 0;
        PORTC = (char)d3;
        PORTDbits.RD6 = 1;
        PORTDbits.RD5 = 1;
        PORTDbits.RD4 = 0;
        __delay_ms(1);
    }
    return;
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISA = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    
    OSCCONbits.IRCF = 0b111; //8Mhz
    OSCCONbits.SCS = 1;
    
    //config tmrm0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b111;
    TMR0 = _tmr0_value;
    
    // configuracion de la interrupcion tmr0
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.PEIE = 0;
    INTCONbits.GIE = 1;
    
    //config ADC
    ANSEL = 0b11000000; //port e como salida
    TRISE = 0b0011;
    ADCON0bits.ADCS = 0b10;
    //ADCON0bits.CHS = 0b0111;
    
    __delay_ms(1);
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADON = 1;
    ADIF = 0;
    PIE1bits.ADIE = 1;
}

int _displays(int a){
    switch(a){
        case 0:
            return 0b11000000;
        case 1:
            return 0b11111001;
        case 2:
            return 0b10100100;
        case 3:
            return 0b10110000;
        case 4:
            return 0b10011001;
        case 5:
            return 0b10010010;
        case 6:
            return 0b10000010;
        case 7:
            return 0b11111000;
        case 8:
            return 0b10000000;
        case 9:
            return 0b10010000;
        default:
            return 0b00000000;
    }
}