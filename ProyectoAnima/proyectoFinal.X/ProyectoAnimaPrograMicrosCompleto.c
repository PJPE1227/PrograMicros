/* 
 * File:   protest4.c
 * Author: PJPE
 *
 * Created on 30 de mayo de 2023, 11:08 AM
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

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <pic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint16_t pot0 = 0;
uint16_t pot1 = 0;
uint16_t pot2 = 0;
uint16_t pot3 = 0;
uint8_t servo = 0;

uint16_t conPot0 = 0;
uint16_t conPot1 = 0;
uint16_t conPot2 = 0;
uint16_t conPot3 = 0;

int cont = 0;

void setup(void);
void __interrupt()isr(void);
uint16_t concat_bits(uint16_t x, uint16_t y);
void delay_pulse(uint16_t);
uint16_t mapear(uint16_t valor, uint16_t rango_min, uint16_t rango_max, uint16_t nuevo_min, uint16_t nuevo_max);

/*
 * 
 */
int main(void) {
    setup();
    
    while(1){
        GO = 1;
        __delay_ms(50);
        DC1B1 = (uint8_t) pot0 & 2;
        DC1B0 = (uint8_t) pot0 & 1;
        CCPR1L = (uint8_t)(pot0 >> 2);
       
    }
    
}

uint16_t mapear(uint16_t valor, uint16_t rango_min, uint16_t rango_max, uint16_t nuevo_min, uint16_t nuevo_max){
    uint16_t nuevo_valor = nuevo_min + (((valor - rango_min)*(nuevo_max - nuevo_min))/(rango_max - rango_min));
    return nuevo_valor;
}

void setup(void){
    ANSEL = 0b00001111;
    ANSELH = 0;
    TRISA = 0b00001111;
    TRISB = 0b00000001;
    TRISC = 128;
    TRISD = 0;
    TRISE = 0;
    
    OSCCON = 0b01100000;
    
    ADCON1bits.ADFM = 0;
    ADCON0 = 0b01000001;
    
    TMR0 = 8;
    OPTION_REGbits.PS = 0b101;
    PSA = 0;
    T0CS = 0;
    TMR0IF = 0;
    
    TRISCbits.TRISC2 = 1;
    PR2 = 249;
    CCP1M3 = 1;
    CCP1M2 = 1;
    CCPR1L = 0;
    
    TMR2IF = 0;
    T2CON = 3;
    T2CONbits.TMR2ON = 1;
    while(TMR2IF == 0){
    }
    TRISC = 128;
    
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;
    ADIE = 1;
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    __delay_us(50);
    
    //configuracion TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 103;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1;
    
    OPTION_REGbits.nRBPU = 0; //enable individual pullups
    WPUBbits.WPUB0 = 1;
    WPUBbits.WPUB1 = 1;
    WPUBbits.WPUB2 = 1;
    WPUBbits.WPUB3 = 1;
    
    //configuracion de interrupciones (SIN GIE)
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    IOCBbits.IOCB0 = 1;
    
    //configuraion de interrupciones
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}

uint16_t concat_bits(uint16_t x, uint16_t y){
    uint16_t z = 0;
    z = (x << 2)|(y>>6);
    return z;
}

void delay_pulse(uint16_t time){
    while(time>0){
        time--;
        __delay_us(1);
    }
}

void __interrupt()isr(void){
    if(TMR0IF){
        TMR0 = 8;
        OPTION_REGbits.PS = 0b101;
        PSA = 0;
        if(servo == 0){
            RD0 = 1;
            delay_pulse(40+(pot1>>3));
            RD0 = 0;
            servo++;
        }
        if(servo == 1){
            RD1 = 1;
            delay_pulse(40+(pot2>>3));
            RD1 = 0;
            servo++;
        }
        if(servo == 2){
            RD2 = 1;
            delay_pulse(40+(pot3>>3));
            RD2 = 0;
            servo = 0;
        }
        TMR0IF = 0;
    }
    
    if(ADIF == 1){
        if(RA6 == 0 && RA7 == 0){
            if(CHS0 == 0 && CHS1 == 0){
                pot0 = concat_bits(ADRESH, ADRESL);
                CHS0 = 1;
            }else if(CHS0 == 1 && CHS1 == 0){
                pot1 = concat_bits(ADRESH, ADRESL);
                CHS0 = 0;
                CHS1 = 1;
            }else if(CHS0 == 0 && CHS1 == 1){
                pot2 = concat_bits(ADRESH, ADRESL);
                CHS0 = 1;
            }else if(CHS0 == 1 && CHS1 == 1){
                pot3 = concat_bits(ADRESH, ADRESL);
                CHS0 = 0;
                CHS1 = 0;
            }
        }
        ADIF = 0;
    }
    
    if(PIR1bits.RCIF){
        TXREG = RCREG;
        if(RA6 == 0 && RA7 == 1){
            if(RCREG == 120){
                conPot1++;

                if(conPot1 == 19){
                    conPot1 = 18;
                }
                pot1 = mapear(conPot1, 0, 18, 230, 800);
            }
            if(RCREG == 111){
                conPot1--;
                if(conPot1 >= 256){
                    conPot1 = 0;
                }
                pot1 = mapear(conPot1, 0, 18, 230, 800);
            }
            if(RCREG == 97){
                conPot0++;

                if(conPot0 == 19){
                    conPot0 = 18;
                }
                pot0 = mapear(conPot0, 0, 18, 230, 800);
            }
            if(RCREG == 115){
                conPot0--;
                if(conPot0 >= 256){
                    conPot0 = 0;
                }
                pot0 = mapear(conPot0, 0, 18, 230, 800);
            }
            if(RCREG == 113){
                conPot2++;

                if(conPot2 == 19){
                    conPot2 = 18;
                }
                pot2 = mapear(conPot2, 0, 18, 230, 800);
            }
            if(RCREG == 119){
                conPot2--;
                if(conPot2 >= 256){
                    conPot2 = 0;
                }
                pot2 = mapear(conPot2, 0, 18, 230, 800);
            }
            if(RCREG == 100){
                conPot3++;

                if(conPot3 == 19){
                    conPot3 = 18;
                }
                pot3 = mapear(conPot3, 0, 18, 230, 800);
            }
            if(RCREG == 102){
                conPot3--;
                if(conPot3 >= 256){
                    conPot3 = 0;
                }
                pot3 = mapear(conPot3, 0, 18, 230, 800);
            }
        }
        
        if(RA6 == 1 && RA7 == 0){
            if(RCREG == 116){
                conPot1++;

                if(conPot1 == 19){
                    conPot1 = 18;
                }
                pot1 = mapear(conPot1, 0, 18, 230, 800);
            }
            if(RCREG == 121){
                conPot1--;
                if(conPot1 >= 256){
                    conPot1 = 0;
                }
                pot1 = mapear(conPot1, 0, 18, 230, 800);
            }
            if(RCREG == 122){
                conPot0++;

                if(conPot0 == 19){
                    conPot0 = 18;
                }
                pot0 = mapear(conPot0, 0, 18, 230, 800);
            }
            if(RCREG == 117){
                conPot0--;
                if(conPot0 >= 256){
                    conPot0 = 0;
                }
                pot0 = mapear(conPot0, 0, 18, 230, 800);
            }
            if(RCREG == 112){
                conPot2++;

                if(conPot2 == 19){
                    conPot2 = 18;
                }
                pot2 = mapear(conPot2, 0, 18, 230, 800);
            }
            if(RCREG == 103){
                conPot2--;
                if(conPot2 >= 256){
                    conPot2 = 0;
                }
                pot2 = mapear(conPot2, 0, 18, 230, 800);
            }
            if(RCREG == 114){
                conPot3++;

                if(conPot3 == 19){
                    conPot3 = 18;
                }
                pot3 = mapear(conPot3, 0, 18, 230, 800);
            }
            if(RCREG == 110){
                conPot3--;
                if(conPot3 >= 256){
                    conPot3 = 0;
                }
                pot3 = mapear(conPot3, 0, 18, 230, 800);
            }
        }
        
        PIR1bits.RCIF = 0;
        
    }
    
    if((INTCONbits.RBIF)){
        if(RB0 == 0){
            cont++;
            if(cont == 3){
                cont = 0;
            }
            if(cont == 0){
                RA6 = 0;
                RA7 = 0;
            }else if(cont == 1){
                RA6 = 0;
                RA7 = 1;
            }else if(cont == 2){
                RA6 = 1;
                RA7 = 0;
            }
        }
        INTCONbits.RBIF = 0;
    }
}
