/* 
 * File:   test.c
 * Author: PJPE
 *
 * Created on 14 de abril de 2023, 02:17 AM
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

/*
 * 
 */
int y = 0;
void setup(void);
void __interrupt()isr(void){
    if(PIR1bits.ADIF){
        PORTB = ADRESH;
        //PORTB = y;
        PIR1bits.ADIF = 0;
    }
}

int main(void) {
    setup();
    ADCON0bits.GO = 1;
    while(1){
        if(ADCON0bits.GO == 0){
            __delay_us(50);
            ADCON0bits.GO = 1;
        }
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
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    return;
}

