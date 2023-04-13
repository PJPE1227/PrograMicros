/* 
 * File:   configPWM.h
 * Author: PJPE
 *
 * Created on 5 de abril de 2023, 01:15 PM
 */

#ifndef CONFIGPWM_H
#define	CONFIGPWM_H

void PWM_SETUP(char channel, float periodo_ms);
void PWM_DUTY(char channel, float duty);

#endif	/* CONFIGPWM_H */

