/*
 * File:   aplication.c
 * Author: otavio
 *
 * Created on 11 de Outubro de 2017, 11:17
 */

#include <xc.h>
#include "configurationBits.h"
#include <pic16f15376.h>
#include <stdio.h>

void configureADC();
int lerADC();

void configureUart();
void escreverCharUart(char caracter);
char converterIntParaString(int valorParaSerConvertido);
void escreverStringUart(char string[]);
int tamanhoDaString(char string[]);

void main() {
    
    int valorLidoDoADC = 0;
    
    __delay_ms(500);
    configureADC();
    
    __delay_ms(500);
    configureUart();
    
    __delay_ms(500);
    escreverStringUart("\n\nInicio\n\n");
    
    while(1) {
        
        valorLidoDoADC = lerADC();
        
        escreverStringUart("ADC: ");
        escreverStringUart(converterIntParaString(valorLidoDoADC));
        escreverCharUart('\n');
        
        __delay_ms(500);
        
    }
    
}

void configureADC() {
    TRISAbits.TRISA0 = 1;       // Define RA0 como entrada
    ANSELAbits.ANSA0 = 1;       // Define RA0 como entrada analógica
    ADCON1bits.ADCS = 0b100;    // Seleciona a fonte de clock do ADC: Fosc/4 
    ADCON1bits.ADPREF = 0b00;   // Seleciona a referência de tensão: Vref = Vdd
    ADCON0bits.CHS = 0b000000;  // Seleciona o canal: RA0
    ADCON1bits.ADFM = 1;        // Justifica os registradores do ADC à direita
    ADCON0bits.ADON = 1;        // Habilita o ADC
    PIR1bits.ADIF = 0;          // Limpa a flag de interrupção do ADC
    __delay_ms(10);             // Delay exigido no datasheet
}

int lerADC() {
    int valorLido = 0;                  // Declara um inteiro que guardará o valor do ADC
    ADCON0bits.GOnDONE = 1;             // Inicia a conversão
    while(PIR1bits.ADIF == 0);          // Aguarda o fim da conversão
    valorLido = ((ADRESH<<8) | ADRESL); // Desloca os dois bits mais significativos em 8 posições para a esquerda e soma com os 8 bits menos significativos
    PIR1bits.ADIF = 0;                  // Limpa a flag de interrupção do ADC
    return valorLido;                   // Retorna o valor lido do ADC
}

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

void escreverCharUart(char caracter) {
    TX1REG = caracter;
    while(TX1STAbits.TRMT == 0);
}

char converterIntParaString(int valorParaSerConvertido) {
    char string[7];
    sprintf(string, "%d", valorParaSerConvertido);
    return string;
}

void escreverStringUart(char string[]) {
    int contador = 0;
    int tamanhoDaStr = tamanhoDaString(string);
    
    for(contador = 0; contador < tamanhoDaStr; contador++) {
        escreverCharUart(string[contador]);
    }
}

int tamanhoDaString(char string[]) {
    int lenght = 0;
    while(string[lenght] != '\0') {
        lenght++;
    }
    return lenght;
}


