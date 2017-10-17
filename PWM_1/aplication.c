/*
 * File:   aplication.c
 * Author: otavio
 *
 * Created on 10 de Outubro de 2017, 15:46
 */

/*
 *
 *                        4 * (TMR2 prescaler) * (PR2)
 *         PWM(periodo) = ----------------------------
 *                                    Fosc
 * 
 * PR2: Timer2 Module Period Register
 * TMR2: Holding Register for the 8-bit TMR2 Register
 * 
 * Exemplo:
 * 
 * 
 *                        4 * 1 * 50
 *        PWM(periodo) = ------------ = 200us
 *                          1000000
 * 
 *                           1
 *        PWM(freq) = -------------- = 5kHz
 *                      PWM(periodo)     
 */

#include <xc.h>
#include <pic16f15376.h>
#include "configurationBits.h"

void configurePwm();
void setarDutyCycle(float porcentagem);

void main(void) {
    
    configurePwm();
    
    while(1) {
        
        int porcentagem = 0;
        
        for(porcentagem = 0; porcentagem <= 100; porcentagem++) {
            setarDutyCycle(porcentagem);
            __delay_us(1);
        }
        
        for(porcentagem = 100; porcentagem >= 0; porcentagem--) {
            setarDutyCycle(porcentagem);
            __delay_us(1);
        }
        
    }
    
}

/**
 * Função para setar a porcentagem do ciclo ativo do PWM
 * @param porcentagem
 */
void setarDutyCycle(float porcentagem) {
    
    if(porcentagem <= 100) {
        
        float dutyCycle = (4 * (porcentagem/100) * (PR2 + 1));
    
        if(dutyCycle <= 255) {
            CCPR1H = 0b00000000;
            CCPR1L = dutyCycle;
        }
        else if(dutyCycle <= 511) {
            CCPR1H = 0b00000001;
            CCPR1L = (dutyCycle - 256);
        }
        else if(dutyCycle <= 767) {
            CCPR1H = 0b00000010;
            CCPR1L = (dutyCycle - 512);
        }
        else if(dutyCycle <= 1023) {
            CCPR1H = 0b00000011;
            CCPR1L = (dutyCycle - 768);
        }
        
    }
    
}

void configurePwm() {
    
    TRISCbits.TRISC2 = 1;   // Define o pino RC2 como entrada
    RC2PPS = 0x09;          // Define o pino RC2 como a saída do PWM (CCP1)
    ANSELCbits.ANSC2 = 0;   // Define o pino RC2 como digital
    
    PR2 = 50;               // Seta o valor do período
    
    CCP1CONbits.CCP1EN = 1;     // Habilita o módulo CCP1
    CCP1CONbits.FMT = 0;        // Define o alinhamento a direita
    CCP1CONbits.MODE = 0b1100;  // Seleciona o modo PWM no módulo CCP1
    
    CCPR1L = 0;                 // Zera o valor do Duty Cycle
    CCPR1H = 0;                 // Zera o valor do Duty Cycle
    
    PIE4bits.TMR2IE = 0;        // Desabilita a interrupção do Timer2
    PIR4bits.TMR2IF = 0;        // Limpa a flag de interrupção do Timer2
    
    T2CLKCONbits.CS = 0b0001;   // Seleciona a fonte de clock do Timer2: Fosc/4
    T2CONbits.CKPS = 0b000;     // Define o Prescaler do Timer2: 1:1
    T2CONbits.T2ON = 1;         // Inicia o Timer2
    
    while(PIR4bits.TMR2IF == 0);    // O código fica travado aqui até o primeiro estouro do Timer2
    
    TRISCbits.TRISC2 = 0;       // Define o pino RC2 como saída
    
}

