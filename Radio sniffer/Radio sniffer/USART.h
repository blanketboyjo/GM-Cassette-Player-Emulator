/*
 * USART.h
 *	This library serves as the back bone for USART0/1 on an Atmel
 *  Atmega328PB. It is designed to work with a 16mHz oscillator;
 *  this library will not function on a different frequency unless
 *  modified by the user. 
 *  The following frequencies are supported:
 *		* 9600   baud
 *		* 57600  baud
 *		* 115200 baud
 *		* 250000 baud
 *  Should the user input an unsupported baud, the system will
 *  default to 9600 baud.
 *  This library runs USART in 2x mode.
 *
 *  Modification log:
 *		Created: 4/26/2016
 *  Known Problems:
 *		
 *  Author: Jordan Jones
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

class USART{
public:
	inline USART(volatile uint8_t *UCSRA,		//Pointer to UCSRA
			volatile uint8_t *UCSRB,			//Pointer to UCSRB
			volatile uint8_t *UCSRC,			//Pointer to UCSRC
			volatile uint8_t *UBRRH,			//Pointer to UBRRL
			volatile uint8_t *UBRRL,			//Pointer to UBRRH
			volatile uint8_t *UDR);				//Pointer to UDR
			
	void init(uint32_t speed);					//Setup USART with a set speed
	void end(void);								//Shutdown USART
	void write(uint8_t data);					//Send a single byte
	void write(uint8_t data[], uint8_t length); //Send an array of bytes with a set length
	
protected:
	volatile uint8_t  	*m_UCSRA;		//Pointers to USART registers
	volatile uint8_t 	*m_UCSRB;
	volatile uint8_t 	*m_UCSRC;
	volatile uint8_t 	*m_UBRRH;
	volatile uint8_t 	*m_UBRRL;
	volatile uint8_t 	*m_UDR;
};

extern USART USART0;			//Declares the front end objects for later usage
extern USART USART1;			//Declares the front end objects for later usage




#endif /* USART_H_ */