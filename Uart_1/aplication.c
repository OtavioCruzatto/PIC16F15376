/*
 * File:   aplication.c
 * Author: otavio
 *
 * Created on 9 de Outubro de 2017, 14:32
 */

/**
 * Includes
 */
#include <xc.h>
#include "configurationBits.h"
#include <pic16f15376.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Protótipos de função
 */
void configureGpio();
void configureUart();
void escreverCharUart(char caracter);
void escreverStringUart(char string[]);
int tamanhoDaString(char string[]);
void configureGpio();
char converterIntParaString(int valorParaSerConvertido);   //(-32768 à 32767)
int converterStringParaInt(char stringParaSerConvertida);  //(-32768 à 32767)

/**
 * Função principal
 */
void main() {
    
    configureUart();
    configureGpio();
    
    while(1) {
        escreverStringUart("Teste: ");
        escreverStringUart(converterIntParaString(32767));
        escreverCharUart('\n');
        escreverCharUart('\n');
        __delay_ms(500);
    }
    
}

void configureGpio() {
    TRISDbits.TRISD2 = 0;
    ANSELCbits.ANSC2 = 0;
    PORTDbits.RD2 = 0;
}

/**
 * Função para inicializar a uart com baudrate de 9600
 */
void configureUart() {
    
    INTCONbits.GIE = 1;     // Habilita as interrupções globais
    INTCONbits.PEIE = 1;    // Habilita as interrupções de periféricos
    PIE3bits.RC1IE = 1;     // Habilita a interrupção do RX da uart
    PIR3bits.RC1IF = 0;     // Limpa a flag da interrupção do RX
    
    TRISCbits.TRISC7 = 1;   // Defini o pino RC7 (RX) como entrada
    ANSELCbits.ANSC7 = 0;   // Defini o pino RC7 (RX) como entrada digital
    
    TRISCbits.TRISC6 = 0;   // Defini o pino RC7 (TX) como saída
    ANSELCbits.ANSC6 = 0;   // Defini o pino RC7 (TX) como saída digital
    
    RC6PPS = 0x0F;          // Defini o pino RC6 como TX
    RC7PPS = 0x17;          // Defini o pino RC7 como RX
    
    TX1STAbits.CSRC = 0;    // Somente para modo Síncrono
    TX1STAbits.TX9 = 0;     // Transmissão de 8 bits
    TX1STAbits.TXEN = 1;    // Habilita transmissão
    TX1STAbits.SYNC = 0;    // Modo Assíncrono
    TX1STAbits.SENDB = 0;
    TX1STAbits.BRGH = 1;    // BaudRate - Alta velocidade
    //TX1STAbits.TX9D = 0;
    
    RC1STAbits.SPEN = 1;    // Porta serial habilitada
    RC1STAbits.RX9 = 0;     // Recepção de 8 bits
    RC1STAbits.SREN = 0;    // Somente para modo Síncrono
    RC1STAbits.CREN = 1;    // Habilita recepção contínua
    RC1STAbits.ADDEN = 0;
    
    BAUD1CONbits.ABDOVF = 0;    // Auto-baud timer did not overflow
    BAUD1CONbits.SCKP = 0;      // Idle state for transmit (TX) is a High level
    BAUD1CONbits.BRG16 = 1;     // 16-bit Baud Rate Generator is used
    BAUD1CONbits.WUE = 0;       // USART will continue to sample the Rx pin ? interrupt generated on falling edge; bit cleared in hardware on following rising edge
    BAUD1CONbits.ABDEN = 0;     // Baud rate measurement disabled or completed
    
    SP1BRG = 25;                // Baud Rate: 9600 
}

/**
 * Função para escrever um char na uart.
 * @param caracter
 */
void escreverCharUart(char caracter) {
    TX1REG = caracter;
    while(TX1STAbits.TRMT == 0);
}

/**
 * Função para converter uma variável int em string
 * @param valorParaSerConvertido
 * @return 
 */
char converterIntParaString(int valorParaSerConvertido) {
    char string[7];
    sprintf(string, "%d", valorParaSerConvertido);
    return string;
}

/**
 * Função para converter uma string em um int
 * @param stringParaSerConvertida
 * @return 
 */
int converterStringParaInt(char stringParaSerConvertida) {
    int i = atoi(stringParaSerConvertida);
    return i;
}

/**
 * Função que retorna o tamanho da string passada como parâmetro.
 * @param string
 * @return 
 */
int tamanhoDaString(char string[]) {
    int lenght = 0;
    while(string[lenght] != '\0') {
        lenght++;
    }
    return lenght;
}

/**
 * Função para escrever uma string na uart.
 * @param string
 */
void escreverStringUart(char string[]) {
    int contador = 0;
    int tamanhoDaStr = tamanhoDaString(string);
    
    for(contador = 0; contador < tamanhoDaStr; contador++) {
        escreverCharUart(string[contador]);
    }
}

/**
 * Função para tratamento das interrupções
 */
void interrupt tratamento() {
    
    if(PIE3bits.RC1IE && PIR3bits.RC1IF) {
        
        escreverCharUart(RC1REG);
        escreverStringUart("\n\n");
        
        if(RC1REG == '0') {
            PORTDbits.RD2 = 0;
        }
        else if(RC1REG == '1') {
            PORTDbits.RD2 = 1;
        }
        
        PIR3bits.RC1IF = 0;
    }
    
}

