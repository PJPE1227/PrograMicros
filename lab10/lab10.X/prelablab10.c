/* 
 * File:   prelablab10.c
 * Author: PJPE
 *
 * Created on 4 de mayo de 2023, 02:04 PM
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
#include <stdio.h>
#include <stdlib.h>
#include<pic.h>
#include<stdint.h>
#include<string.h>
#define _XTAL_FREQ 1000000
/*
 * 
 */
uint8_t cont0 = 0;
uint8_t newOne = 255;
int i = 0;
int c = 0;
int d = 0;
int u = 0;
char new[3];
void setup(void);
void cadena (char txt[]);
void sendChar (uint8_t character);
void asciiCDU(int x);
int asciiNUM(int a);
int asciiMUN(int a);

void __interrupt()isr(void){
    if(PIR1bits.RCIF){
        //TXREG = RCREG;
        /*new[i] = RCREG;
        i++;
        if(i == 3){
            cont0 = 100*(asciiMUN(new[0]));
            cont0 = cont0 + 10*(asciiMUN(new[1]));
            cont0 = cont0 + asciiMUN(new[2]);
            PORTA = cont0;
            __delay_ms(10);
            TXREG = '\r'; //'\n' para fisico
            i = 0;
        }*/
        PORTD = RCREG;
    }
    if(INTCONbits.RBIF){
        if(RB0 == 0){
            cont0++;
            //PORTA = cont0;
            /*__delay_ms(10);
            //TXREG = '\r';
            asciiCDU(cont0);
            __delay_ms(10);
            TXREG = c;
            __delay_ms(10);
            TXREG = d;
            __delay_ms(10);
            TXREG = u;
            __delay_ms(10);
            TXREG = '\r';*/ //'\n' para fisico
        }
        if(RB1 == 0){
            cont0--;
            //PORTA = cont0;
            /*__delay_ms(10);
            //TXREG = '\r';
            asciiCDU(cont0);
            __delay_ms(10);
            TXREG = c;
            __delay_ms(10);
            TXREG = d;
            __delay_ms(10);
            TXREG = u;
            __delay_ms(10);
            TXREG = '\r';*/ //'\n' para fisico
        }
        
        INTCONbits.RBIF = 0;
    }
}

int main(void) {
    setup();
    while(1){
        PORTA = cont0;
        if(cont0 != newOne){
            sendChar(cont0);
            newOne = cont0;
        }
    }
}

void setup(void){
    ANSEL = 0b00000000;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0b00000011;
    TRISD = 0;
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    
    OSCCONbits.IRCF = 0b100; //oscilador 1mhz
    OSCCONbits.SCS = 1;
    
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
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    //configuradion de pullups
    OPTION_REGbits.nRBPU = 0; //enable individual pullups
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    //WPUBbits.WPUB2 = 1;
    
    //configuracion de interrupciones (SIN GIE)
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    //IOCBbits.IOCB2 = 1;
    
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

void sendChar(uint8_t character){
    TXREG = character;
    while(!TXSTAbits.TRMT);
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

int asciiMUN(int a){
    switch(a){
        case 48:
            return 0;
        case 49:
            return 1;
        case 50:
            return 2;
        case 51:
            return 3;
        case 52:
            return 4;
        case 53:
            return 5;
        case 54:
            return 6;
        case 55:
            return 7;
        case 56:
            return 8;
        case 57:
            return 9;
        default:
            return 0;
    }
}