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

/**Função que contém o switch/case com a máquina de estados*/
void ExecutaMaquinaDeEstados(void);  


int pino_led_verde = 7;
int pino_led_vermelho = 10;
int pino_buzzer = 6;
int pino_sensor = 9;

int contador = 0; /*!<váriavel que guardará a contagem de detecções*/
volatile char ControleMaquinaEstado;  /*!<variável responsável por armazenar o estado atual da máquina de estados*/
int address = 0; /*!<váriavel do endereço*/



/** \brief será executado apenas ao ligar o Arduino*/
void setup() {
	/**
		\details Define o pino do sensor como entrada e os pinos dos leds e buzzer como saída
		\details O contador receberá o valor que está guardado em dois endereços na memória EEPROM do arduino
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
		\details Execução da função para configuração da transmissão.
		\details Execução da Máquina de estados
	*/
	USART_Init(MYUBRR); 
    ExecutaMaquinaDeEstados(); 
}

/** \brief Função de execução da máquina de estados via switch/case*/
 void ExecutaMaquinaDeEstados(void)
 {	 
	 /**
		\details Estado_1: Estado que fica aguardando detecção
		\details	Sem movimento detectado: Mantém o led verde ligado, led vermelho e buzzer desligados, enquanto o sensor aguarda uma detecção
		\details	Quando sensor tiver feito uma detecção, passará pro próximo estado.
		\details Estado_2: Estado que incrementa o contador e guarda valor na memória
		\details	Movimento detectado: Liga led vermelho e desliga led verde.
		\details	Aguarda o objeto sair da frente do sensor(caso esteja parado), efetua a contagem e aciona o buzzer.
		\details	O valor será salvo em dois endereços, pois cada endereço tem 1byte. Utilizando dois endereços o valor máximo que poderá ser lido será 32767, limitado agora pelo valor do tipo da variável utilizada na declaração do contador.
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