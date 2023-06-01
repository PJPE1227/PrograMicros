/* 
 * File:   lastpro.c
 * Author: PJPE
 *
 * Created on 19 de mayo de 2023, 11:24 AM
 */
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

#define _XTAL_FREQ 2000000
#include <xc.h>
#include <pic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t pot1 = 0;
uint8_t pulse_width1 = 0;
uint8_t pot2 = 0;
uint8_t pulse_width2 = 0;

void setup(void);
void __interrupt()isr(void);
/*
 * 
 */
int main(void) {
    setup();
    while(1){
        GO = 1;
        __delay_us(10);
        pulse_width1 = (pot1 >> 1) + 125;
        pulse_width2 = (pot2 >> 1) + 125;
        DC1B1 = pulse_width1 & 2;
        DC1B0 = pulse_width1 & 1;
        CCPR1L = pulse_width1 >> 2;
        DC2B1 = pulse_width2 & 2;
        DC2B0 = pulse_width2 & 1;
        CCPR2L = pulse_width2 >> 2;
    }
   
}
void setup(){
    ANSEL = 3;
    ANSELH = 0;
    TRISA = 3;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    
    OSCCON = 0b01010000;
    
    ADCON1bits.ADFM = 0;
    ADCON0 = 0b01000001;
    
    TRISC = 6;
    PR2 = 249;
    CCP1M3 = 1;
    CCP1M2 = 1;
    CCPR1L = 32;
    
    CCP2M3 = 1;
    CCP2M2 = 1;
    CCPR1L = 32;
    
    TMR2IF = 0;
    T2CON = 3;
    T2CONbits.TMR2ON = 1;
    while(TMR2IF==0){
    }
    
    TRISC = 0;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}

void __interrupt()isr(void){
    if(ADIF == 1){
        if(CHS0 == 0){
            pot1 = ADRESH;
            CHS0 = 1;
        }else if(CHS0 == 1){
            pot2 = ADRESH;
            CHS0 = 0;
        }
        ADIF = 0;
    }
}