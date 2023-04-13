/*
 * File:   configPWM.c
 * Author: PJPE
 *
 * Created on 12 de abril de 2023, 11:33 PM
 */


#include <xc.h>
#include "configPWM.h"
#define _XTAL_FREQ 500000

void PWM_SETUP(char channel, float periodo_ms){
    PR2 = (unsigned char)((_XTAL_FREQ*periodo_ms)/(4*16)-1);
    PIR1bits.TMR2IF = 0;
    T2CONbits.T2CKPS = 0b11;
    T2CONbits.TMR2ON = 1;
    while(!PIR1bits.TMR2IF);
    
    PIR1bits.TMR2IF = 0;
    
    if(channel == 1){
        TRISCbits.TRISC1 = 1; // CCP1 como entrada
        CCP1CON = 0; // Apagar CCP1 inicialmente
        CCP1CONbits.P1M = 0; // Modo de single output
        CCP1CONbits.CCP1M = 0b1100; // Modo PWM para CCP1
        CCP1CONbits.DC1B1 = 0;
        CCP1CONbits.DC1B0 = 0;
        TRISCbits.TRISC1 = 0; // Habilitar salida en RC1
    } else if(channel == 2){
        TRISCbits.TRISC2 = 1; // CCP2 como entrada
        CCP2CON = 0; // Apagar CCP2 inicialmente
        CCP2CONbits.CCP2M = 0b1100; // Modo PWM para CCP2
        CCP2CONbits.DC2B1 = 0;
        CCP2CONbits.DC2B0 = 0;
        TRISCbits.TRISC2 = 0; // Habilitar salida en RC2
    }
    return;
}

void PWM_DUTY(char channel, float duty){
    unsigned int duty_var = (unsigned int)7 + (duty - 0)*(25 - 7)/(500 - 0);
    
    if(channel == 1){
        CCPR1L = duty_var;
        CCP1CONbits.DC1B = duty_var & 0b11;
    } else if (channel == 2){
        CCPR2L = duty_var;
        CCP2CONbits.DC2B0 = duty_var & 0b01;
        CCP2CONbits.DC2B1 = duty_var & 0b10;
    }
    return;
}
