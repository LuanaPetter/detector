/*
 * Memoria.c
 *
 * Created: 27/06/2016 21:16:27
 *  Author: LuanaCristina
 */ 
#include <avr/io.h>

//Fun��o para escrever na mem�ria EEPROM, conforme o Datasheet do microcontrolador ATmega328P
void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
	/** Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/** Set up address and Data Registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/** Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/** Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}

//Fun��o para escrever na mem�ria EEPROM, conforme o Datasheet do microcontrolador ATmega328P
unsigned char EEPROM_read(unsigned int uiAddress)
{
	/** Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/** Set up address register */
	EEAR = uiAddress;
	/** Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/** Return data from Data Register */
	return EEDR;
}