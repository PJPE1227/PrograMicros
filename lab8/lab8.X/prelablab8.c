/*
 * File:   prelablab8.c
 * Author: PJPE
 *
 * Created on 13 de abril de 2023, 11:39 PM
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
#include <string.h>
#define _XTAL_FREQ 1000000

const char data[] = "hello world";
int cont = 1;
int y = 0;
int c = 0;
int d = 0;
int u = 0;
void setup(void);

void cadena (char txt[]);
void asciiCDU(int x);
void adc();
int asciiNUM(int a);

void __interrupt() isr(void){
    if(PIR1bits.RCIF){
        //PORTB = RCREG;
        TXREG = RCREG;
        //if(RCREG == 13){
        //    cont = 1;
        //}
        if(RCREG == 120){
            __delay_ms(50);
            TXREG = '\r'; //'\n' para fisico
            asciiCDU(y);
            __delay_ms(50);
            TXREG = c;
            __delay_ms(50);
            TXREG = d;
            __delay_ms(50);
            TXREG = u;
            __delay_ms(50);
            TXREG = '\r'; //'\n' para fisico
            TXREG = '\r'; //'\n' para fisico
            cont = 1;
        }
        if(RCREG == 111){
            __delay_ms(50);
            TXREG = '\r'; //'\n' para fisico
            cadena("ingrese un caracter: ");
            while(RCIF == 0);
            TXREG = RCREG;
            PORTB = RCREG;
            __delay_ms(50);
            TXREG = '\r'; //'\n' para fisico
            TXREG = '\r'; //'\n' para fisico
            cont = 1;
        }
    }
    if(ADIF){
        y = ADRESH;
        PIR1bits.ADIF = 0;
    }
}

int main(void) {
    setup();
    
    while (1) {
        __delay_ms(50);
        if(PIR1bits.TXIF){
            if(cont == 1){
                cadena("1. Leer POT, ingrese x");
                TXREG = '\r'; //'\n' para fisico
                cadena("2. Enviar ASCII, ingrese o");
                TXREG = '\r'; //'\n' para fisico
                cont = 0;
            }
        }
        ADCON0bits.CHS = 0b0010;
        if(ADCON0bits.GO == 0){
            __delay_ms(10);
            ADCON0bits.GO = 1;
        }
        //PORTB = y;
    }
}

void setup(void){
    ANSEL = 0b00000100;
    ANSELH = 0;
    
    TRISA = 0b00000100;
    TRISB = 0;
    PORTB = 0x00;
    
    OSCCONbits.IRCF = 0b100; //oscilador 1mhz
    OSCCONbits.SCS = 1;
    
    //configuracicon adc
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.CHS = 0b0010;
    ADCON0bits.ADON = 1;
    __delay_us(50);
    
    //configuracion TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 25;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;
    
    //configuraion de interrupciones
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    return;
}

void cadena(char txt[]){
    int cont0 = 0;
    while(txt[cont0]!=0){
        cont0++;
    }
    for(int i = 0; i < cont0; i++){
        __delay_ms(50);
        TXREG = txt[i];
    }
    return;
}

void asciiCDU(int x){
    int centenas = x/100;
    int decenas = (x%100)/10;
    int unidades = (x%100)%10;
    
    c = asciiNUM(centenas);
    d = asciiNUM(decenas);
    u = asciiNUM(unidades);
    return;
}

int asciiNUM(int a){
    switch(a){
        case 0:
            return 48;
        case 1:
            return 49;
        case 2:
            return 50;
        case 3:
            return 51;
        case 4:
            return 52;
        case 5:
            return 53;
        case 6:
            return 54;
        case 7:
            return 55;
        case 8:
            return 56;
        case 9:
            return 57;
        default:
            return 0;
    }
}