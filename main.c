/*
 \ detecta.c
 \ Created: 26/06/2016 01:14:11
 \ Author : LuanaCristina
 */ 

#include <avr/io.h>
#include "Arduino.h"  
#include "stdio.h"  
#include "UART.h"     
#include "Memoria.h"  

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

/**define que representa o estado inicial*/
#define ESTADO_1 1   
/**define que representa o estado 2*/
#define ESTADO_2 2 
/**define que representa o estado 3*/
#define ESTADO_3 3   

/**Fun��o que cont�m o switch/case com a m�quina de estados*/
void ExecutaMaquinaDeEstados(void);  


int pino_led_verde = 7;
int pino_led_vermelho = 10;
int pino_buzzer = 6;
int pino_sensor = 9;

int contador = 0; /*!<v�riavel que guardar� a contagem de detec��es*/
volatile char ControleMaquinaEstado;  /*!<vari�vel respons�vel por armazenar o estado atual da m�quina de estados*/
int address = 0; /*!<v�riavel do endere�o*/



/** \brief ser� executado apenas ao ligar o Arduino*/
void setup() {
	/**
		\details Define o pino do sensor como entrada e os pinos dos leds e buzzer como sa�da
		\details O contador receber� o valor que est� guardado em dois endere�os na mem�ria EEPROM do arduino
	*/
	pinMode(pino_sensor, INPUT);
	pinMode(pino_led_verde, OUTPUT);
	pinMode(pino_led_vermelho, OUTPUT);
	pinMode(pino_buzzer, OUTPUT);
	ControleMaquinaEstado = ESTADO_1; 
	int parte1 = EEPROM_read(address); 
	int parte2 = EEPROM_read(address+1); 
	contador = (parte1 * 256) + parte2; 
}

void loop() { 
	/**
		\details Execu��o da fun��o para configura��o da transmiss�o.
		\details Execu��o da M�quina de estados
	*/
	USART_Init(MYUBRR); 
    ExecutaMaquinaDeEstados(); 
}

/** \brief Fun��o de execu��o da m�quina de estados via switch/case*/
 void ExecutaMaquinaDeEstados(void)
 {	 
	 /**
		\details Estado_1: Estado que fica aguardando detec��o
		\details	Sem movimento detectado: Mant�m o led verde ligado, led vermelho e buzzer desligados, enquanto o sensor aguarda uma detec��o
		\details	Quando sensor tiver feito uma detec��o, passar� pro pr�ximo estado.
		\details Estado_2: Estado que incrementa o contador e guarda valor na mem�ria
		\details	Movimento detectado: Liga led vermelho e desliga led verde.
		\details	Aguarda o objeto sair da frente do sensor(caso esteja parado), efetua a contagem e aciona o buzzer.
		\details	O valor ser� salvo em dois endere�os, pois cada endere�o tem 1byte. Utilizando dois endere�os o valor m�ximo que poder� ser lido ser� 32767, limitado agora pelo valor do tipo da vari�vel utilizada na declara��o do contador.
		\details Estado_3:Estado que mostra o valor do contador
	*/
	 switch(ControleMaquinaEstado)
	 {
		case ESTADO_1: 
		{
			digitalWrite(pino_led_verde, HIGH); 
			digitalWrite(pino_led_vermelho, LOW); 			
			digitalWrite(pino_buzzer, LOW); 
			while( digitalRead(pino_sensor) == 1){ 
				if (digitalRead(pino_sensor) == 0){ 
					ControleMaquinaEstado = ESTADO_2; 
					break;
				 }
			 }
		}
		case ESTADO_2:
		{
			digitalWrite(pino_led_verde, LOW); 
			digitalWrite(pino_led_vermelho, HIGH); 
			while( digitalRead(pino_sensor) == 0){ 
				if (digitalRead(pino_sensor) == 1){ 
					ControleMaquinaEstado = ESTADO_3; 
					break;
				}
			}
			digitalWrite(pino_buzzer, HIGH); 
			contador = contador + 1;
			EEPROM_write(address,contador/256); 
			EEPROM_write(address+1, contador%256); 
			delay(100);			 
		}
		case ESTADO_3: 
		{		
			printf("\nContador: %d\n",contador);
			delay(100);		  
			ControleMaquinaEstado = ESTADO_1;
			break;
		 }
	 }
 }