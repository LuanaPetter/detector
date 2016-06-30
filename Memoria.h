/*
 * Memoria.h
 *
 * Created: 27/06/2016 21:16:40
 *  Author: LuanaCristina
 */ 


#ifndef MEMORIA_H_
#define MEMORIA_H_

void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);

#endif /* MEMORIA_H_ */