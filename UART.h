/*
 * UART.h
 *
 * Created: 27/06/2016 20:52:05
 *  Author: LuanaCristina
 */ 


#ifndef UART_H_
#define UART_H_

void USART_Transmit( unsigned char data );
void USART_Init(unsigned int ubrr);

#endif /* UART_H_ */