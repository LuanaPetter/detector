/*
 * UART.c
 *
 * Created: 27/06/2016 20:51:48
 *  Author: LuanaCristina
 */ 
#include <avr/io.h>
#include "stdio.h"

static char output[128];
static char ind = 0;

//Funções para transmissão dos dados, conforme o Datasheet do microcontrolador ATmega328P

void USART_Transmit(unsigned char data)
{
	/** Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));
	/** Put data into buffer, sends the data */
	UDR0 = data;
}

static int putchar_buf(int c, FILE *__stream)
{
	output[ind++%128] = c;
	USART_Transmit(c);
	return c;
}

static FILE mystdout = FDEV_SETUP_STREAM(putchar_buf, NULL,_FDEV_SETUP_WRITE);

//Configuração para iniciar a comunicação
void USART_Init(unsigned int ubrr)
{
	//Definição da taxa de transmissão
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	//Ativação do receptor e transmissor
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	stdout = &mystdout;
}

