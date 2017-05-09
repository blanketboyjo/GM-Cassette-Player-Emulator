/*
 * USART.cpp
 *
 *	Modification log:
 *		Created: 4/26/2016
 *	Known Problems:
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

#include "USART.h"

USART::USART(volatile uint8_t *UCSRA, volatile uint8_t *UCSRB, volatile uint8_t *UCSRC, volatile uint8_t *UBRRH, volatile uint8_t *UBRRL, volatile uint8_t *UDR){
	m_UCSRA = UCSRA;
	m_UCSRB = UCSRB;
	m_UCSRC = UCSRC;
	m_UBRRH = UBRRH;
	m_UBRRL = UBRRL;
	m_UDR   = UDR;

}

void USART::init(uint32_t speed){
	uint16_t setting = 207;	//UBRR default; 9600 baud
	switch (speed){			//check for recognized speeds
		case 250000:		//  Speed: 250000
			setting = 7;		//    UBRR setting for 250000
		break;				//    Exit switch
		case 115200:		//  Speed: 115200
			setting = 16;		//    UBRR setting for 115200
		break;				//    Exit switch
		case 57600:			//  Speed: 57600
			setting = 34;		//    UBRR setting for 57600
		break;				//    Exit switch
	}
	*m_UBRRH = (uint8_t) (setting>>8);	// Shift UBRR down 8 and store in UBRRH
	*m_UBRRL = (uint8_t)  setting;		// Store lower 8 bits into UBRRL*/
	*m_UCSRA = (1<<1);					// Double speed
	*m_UCSRB = (1<<RXEN0)|(1<<TXEN0);	// Enable RX and TX
	*m_UCSRC = (1<<USBS0)|(3<<UCSZ00);	// TODO annotate this
}

void USART::end(void){
	
}


void USART::write(uint8_t data){
	while(!(*m_UCSRA & (1<<UDRE0)));
	*m_UDR = data;
}

void USART::write(uint8_t data[], uint8_t length){
	for(int i = 0; i<length;i++){
		while(!(*m_UCSRA & (1<<UDRE0)));
		*m_UDR = data[i];
	}
}

USART USART0(&UCSR0A,&UCSR0B,&UCSR0C,&UBRR0H,&UBRR0L,&UDR0);
USART USART1(&UCSR1A,&UCSR1B,&UCSR1C,&UBRR1H,&UBRR1L,&UDR1);