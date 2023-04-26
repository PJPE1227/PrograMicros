/* 
 * File:   prelab9.c
 * Author: PJPE
 *
 * Created on 20 de abril de 2023, 11:36 PM
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
#define _XTAL_FREQ 1000000
#define dirEEPROM 0x04

/*
 * 
 */
int y = 0;
uint8_t potValue;
uint8_t botonPrevState;

void setup(void);
void writeEEPROM(uint8_t data, uint8_t adress);
uint8_t readEEPROM(uint8_t adress);

void __interrupt()isr(void){
    if(INTCONbits.RBIF){
        //PORTB = PORTB;
        
        if(RB0 == 0){
            //continue;
            PORTB = PORTB;
            //INTCONbits.RBIF = 0;
            PORTE = 0b01;
        }
        PORTE = 0b11;
        INTCONbits.RBIF = 0;
    }
    if(ADIF){
        potValue = ADRESH;
        PORTC = potValue;
        PIR1bits.ADIF = 0;
    }
}

int main(void) {
    setup();
    while(1){
        ADCON0bits.CHS = 0b0010;
        if(ADCON0bits.GO == 0){
            __delay_ms(10);
            ADCON0bits.GO = 1;
        }
        
        
        PORTD = readEEPROM(dirEEPROM);
        if(RB1 == 0){
            INTCONbits.RBIF = 0;
            PORTE = 0b010;
            SLEEP();
        }
        
        if(RB2 == 0){
            //PORTB = PORTB;
            botonPrevState = 1;
            //INTCONbits.RBIF = 1;
        }
        if(RB2 == 1 && botonPrevState == 1){
            writeEEPROM(potValue, dirEEPROM);
            botonPrevState = 0;
        }
        
        
    }
}

void setup(void){
    ANSEL = 0b00000100;
    ANSELH = 0;
    
    TRISA = 0b00000100;
    TRISB = 0b00000111;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    PORTA = 0;
    PORTB = 0x00;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    OSCCONbits.IRCF = 0b100; //oscilador 1mhz
    OSCCONbits.SCS = 1;
    
    
    //configuracicon adc
    ADCON1bits.ADFM = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.CHS = 0b0010;
    __delay_us(50);
    ADCON0bits.ADON = 1;
    
    
    //configuraion de interrupciones
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
     //configuradion de pullups
    OPTION_REGbits.nRBPU = 0; //enable individual pullups
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    WPUBbits.WPUB2 = 1;
    
    //configuracion de interrupciones (SIN GIE)
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCBbits.IOCB0 = 1;
    //IOCBbits.IOCB1 = 1;
    IOCBbits.IOCB2 = 1;
    
    return;
}

void writeEEPROM(uint8_t data, uint8_t adress){
    EEADR = adress;
    EEDAT = data;
    
    EECON1bits.EEPGD = 0; //Escribir a memoria de datos
    EECON1bits.WREN = 1; //Habilitar escritura a EEPROM
    
    INTCONbits.GIE = 0; //deshabiblitar interrupciones
    
    EECON2 = 0x55;      //secuencia oligatoria
    EECON2 = 0xAA;
    EECON1bits.WR = 1;  //habilitar escritura
    
    INTCONbits.GIE = 1; //habilitar interrupciones
    EECON1bits.WREN = 0;//deshabilitar escritura de EEPROM
}

uint8_t readEEPROM(uint8_t adress){
    EEADR = adress;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    return EEDAT;
}