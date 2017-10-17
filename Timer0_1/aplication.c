/*
 * File:   aplication.c
 * Author: otavio
 *
 * Created on 9 de Outubro de 2017, 10:34
 */

/*
 *  Funcionamento: Timer com 8 bits
 *                 O registrador TMR0L conta at� o valor escrito no registrador TMR0H e estoura.
 *                 No momento do estouro do Timer0, o registradoe TMR0L � zerado, a flag de interrup��o do Timer0 sinaliza.
 * 
 */

/*
 *                      C�lculos
 * 
 *               4 * TMR0H * Prescaler * Postscaler
 *        t(s) = ----------------------------------
 *                             Fosc
 * 
 * 
 *      Exemplo (com o c�digo abaixo):
 * 
 *               4 * 127 * 16 * 10
 *       t(s) = ------------------- = 0.08128s = 81.28ms
 *                    1000000
 */

#include <xc.h>
#include "configurationBits.h"

void configureTimer0();
void configureInterrupts();
void configureGpio();

void main() {
    
    configureGpio();
    configureInterrupts();
    configureTimer0();
    
    while(1);
    
}

void configureTimer0() {
    
    T0CON1bits.T0ASYNC = 0;         // Sincroniza��o da entrada: S�ncrono
    T0CON1bits.T0CS = 0b010;        // Fonte de clock: Fosc/4
    T0CON1bits.T0CKPS = 0b0100;     // Prescaler: 1:16
    
    T0CON0bits.T0OUTPS = 0b1001;    // Postscaler: 1:10
    T0CON0bits.T016BIT = 0;         // Resolu��o do Timer0: 8 bits 
    
    TMR0H = 0x7F;                   // Defini valor m�ximo de contagem: 127
    TMR0L = 0x00;                   // Defini valor inicial de contagem: 0
    
    T0CON0bits.T0EN = 1;            // Habilita o Timer0
  
}

void configureInterrupts() {
    
    INTCONbits.GIE = 1;             // Habilita interrup��es globais
    PIE0bits.TMR0IE = 1;            // Habilita interrup��o de estouro do Timer0
    PIR0bits.TMR0IF = 0;            // Limpa a flag de interrup��o do Timer0
    
}

void configureGpio() {
    
    TRISDbits.TRISD2 = 0;           // Defini o pino RD2 como sa�da
    PORTDbits.RD2 = 0;              // Coloca o pino RD2 em n�vel l�gico baixo
    ANSELDbits.ANSD2 = 0;           // Defini o pino RD2 como pino Digital
    
}

void interrupt tratamento() {
   
    if(PIR0bits.TMR0IF) {           // Testa se a interrup��o foi do Timer0
        
        if(PORTDbits.RD2 == 1) {
            PORTDbits.RD2 = 0;
        }
        else {
            PORTDbits.RD2 = 1;
        }
     
        PIR0bits.TMR0IF = 0;        // Limpa a flag de interrup��o do Timer0
       
    }

}
