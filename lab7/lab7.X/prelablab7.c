/*
 * File:   prelab7.c
 * Author: PJPE
 *
 * Created on 30 de marzo de 2023, 11:55 PM
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

#include "configPWM.h"

#define _tmr0_value 225
#define _XTAL_FREQ 500000
int y = 0;
int cont0 = 0;
int manualPWM = 0;
void setup(void);

int mapear(int valor, int rango_min, int rango_max, int nuevo_min, int nuevo_max);

void __interrupt() isr(void){
    if(T0IF){
        if(cont0 == 20){
            cont0 = 0;
        }
        if(cont0 <= manualPWM){
            PORTCbits.RC3 = 1;
        }else{
            PORTCbits.RC3 = 0;
        }
        cont0++;
        TMR0 = _tmr0_value;
        T0IF = 0;
    }
    if(PIR1bits.ADIF){
        if(ADCON0bits.CHS == 0b0000){
            PWM_DUTY(1, ADRESH);
            //y = mapear(ADRESH,0 ,500, 7, 25);
            //CCPR1L = y;
            //CCP1CONbits.DC1B1 = y & 0b01;
            //CCP1CONbits.DC1B0 = (y>>7);
        }else if(ADCON0bits.CHS == 0b0001){
            PWM_DUTY(2, ADRESH);
            //y = mapear(ADRESH, 0, 500, 7, 25);
            //CCPR2L = y;
            //CCP2CONbits.DC2B1 = y & 0b01;
            //CCP2CONbits.DC2B0 = (y>>7);
        }else if(ADCON0bits.CHS == 0b0010){
            y = mapear(ADRESH, 0, 500, 1 ,20);
            manualPWM = y;
        }
        PIR1bits.ADIF = 0;
    }
    
}

void main(void) {
    setup();
    while(1){
        if(ADCON0bits.CHS == 0b0000){
            __delay_ms(10);
            ADCON0bits.CHS = 0b0001;
        }else if(ADCON0bits.CHS == 0b0001){
            __delay_ms(10);
            ADCON0bits.CHS = 0b0010;
        }else if(ADCON0bits.CHS == 0b0010){
            __delay_ms(10);
            ADCON0bits.CHS = 0b0000;
        }
        
        if(ADCON0bits.GO == 0){
            __delay_ms(10);
            ADCON0bits.GO = 1;
        }
    }
    return;
}

int mapear(int valor, int rango_min, int rango_max, int nuevo_min, int nuevo_max){
    int nuevo_valor = nuevo_min + (valor - rango_min)*(nuevo_max - nuevo_min)/(rango_max - rango_min);
    return nuevo_valor;
}

void setup(void){

    ANSEL = 0b00000111;
    ANSELH = 0;
    
    TRISA = 0b00000111;
    TRISC = 0x00;
    TRISB = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0x00;
    
    //configuracion del osciclador a 500khz
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    
    //config tmr0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS = 0b001;//1:4
    TMR0 = _tmr0_value;
    
    //configuracicon adc
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.CHS = 0b0000;
    ADCON0bits.ADON = 1;
    __delay_us(50);
    
    // habilitar interrupciones
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    //configuracion del PWM
    PWM_SETUP(1, 0.02);
    PWM_SETUP(2, 0.02);
    
    //TRISCbits.TRISC2 = 1;           // RC2/CCP1 como entrada
    //TRISCbits.TRISC1 = 1;           // RC1/CCP2 como entrada
    //PR2 = 155;                      // config del periodo 
    //CCP1CONbits.P1M = 0;            // config modo PWM ccp1
    //CCP1CONbits.CCP1M = 0b00001100;
    //CCP2CONbits.CCP2M = 0b00001100;
    
    //CCPR1L = 0x0f;                  // ciclo de trabajo inicial
    //CCPR2L = 0x0f;
    //CCP1CONbits.DC1B1 = 0;
    //CCP1CONbits.DC1B0 = 0;
    //CCP2CONbits.DC2B1 = 0;
    //CCP2CONbits.DC2B0 = 0;
    
    //PIR1bits.TMR2IF = 0;            // apagamos la bandera
    //T2CONbits.T2CKPS = 0b11;        // prescaler 1:16
    //T2CONbits.TMR2ON = 1;
    
    //while(!PIR1bits.TMR2IF);        // se espera un ciclo del tmr2
    //PIR1bits.TMR2IF = 0;
    
    //TRISCbits.TRISC2 = 0;           // salida del pwm
    //TRISCbits.TRISC1 = 0;
    
    return;
}