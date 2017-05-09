/*
 * USART.cpp
 *	This library handles USART0/1 on the ATMEGA328PB. It relies entirely
 *	on the interrupt service routine (ISR) so be sure to run sei() before
 *	attempting to use this library.
 *
 *	Modification log:
 *		4/26/2016 - Created
 *		4/28/2016 - Following code written and tested:
 *			Constructor
 *			init
 *			write (single and array versions)
 *			read
 *			m_interruptStoreData
 *			m_interruptTransmitData
 *			unreadData
 *
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

USART::USART(volatile uint8_t *UCSRA, volatile uint8_t *UCSRB, volatile uint8_t *UCSRC, volatile uint8_t *UBRRH, volatile uint8_t *UBRRL, volatile uint8_t *UDR):
	m_UCSRA(UCSRA),		//Tie class pointer to real regs
	m_UCSRB(UCSRB),
	m_UCSRC(UCSRC),
	m_UBRRH(UBRRH),
	m_UBRRL(UBRRL),
	m_UDR(UDR){
	m_nxtRXBuff = 0;	//Init the buffer indices
	m_nxtTXBuff = 0;
	m_eoRXBuff  = 0;
	m_eoTXBuff  = 0;
}

//This function will enable USART a set speeds (see above for supported speeds)
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
	*m_UCSRB = (1<<RXEN)|(1<<TXEN)|		// Enable RX and TX
			   (1<<RXCIE);				// Enable RX complete interrupt
	*m_UCSRC = (1<<USBS)|(3<<UCSZ0);	// Set system to 2 bit stop bits with a 1 byte character size 
}

//This function will run a soft shutdown of USART (flush the registers)
//Todo add USART close function
void USART::end(void){
	
}

//This function adds a single char (uint8_t) to TXBuff and enables the send interrupt
void USART::write(uint8_t data){
	m_TXBuff[m_eoTXBuff] = data;					//Store data into TXBuff
	m_eoTXBuff = (1 + m_eoTXBuff)% TX_BUFF_LEN;	//Increment index, set to 0 on roll over
	*m_UCSRB |= (1<<UDRIE);							//Enable buffer empty interrupt
}

//This function a char array (uint8_t) to TXBuff and enables the send interrupt
void USART::write(uint8_t data[], uint8_t length){
	for(uint8_t i = 0; i<= length; i++){
		m_TXBuff[m_eoTXBuff] = data[i];					//Store data into TXBuff
		m_eoTXBuff = (1 + m_eoTXBuff)% TX_BUFF_LEN;		//Increment index, set to 0 on roll over		
	}
	*m_UCSRB |= (1<<UDRIE);								//Enable buffer empty interrupt
}

//This function returns one byte of data from RXbuff
uint8_t USART::read(void){
	uint8_t returnIndex = m_nxtRXBuff;
	m_nxtRXBuff = (1+m_nxtRXBuff)%RX_BUFF_LEN;  // Increment read index, set to 0 if roll over
	return m_RXBuff[returnIndex];				//Return byte
}

//This function returns how much unread data is currently in RXBuff
uint8_t USART::unreadData(void){
	return (m_eoRXBuff - m_nxtRXBuff)%RX_BUFF_LEN;	//Return number of unread indices
}


//This function adds data to the read buffer an increments the last index
void USART::m_interruptStoreData(void){
	if(((m_eoRXBuff - m_nxtRXBuff)%RX_BUFF_LEN) >RX_BUFF_LEN){	//Data about to overwrite unread data 
		*m_UDR;													//Flush data to prevent over flow
	}else{
		m_RXBuff[m_eoRXBuff] = *m_UDR;				//Store value of USART data register
		m_eoRXBuff = (m_eoRXBuff + 1)%RX_BUFF_LEN;  //Increment end of RX_BUFF clear to 0 if hits max
	}
}

//This function sends data over USART and increments the writing index
void USART::m_interruptTransmitData(void){
	if(m_eoTXBuff != m_nxtTXBuff){						//If indices don't match
		*m_UDR      = m_TXBuff[m_nxtTXBuff];			//Store next piece of data into UBR reg
		m_nxtTXBuff = (1 + m_nxtTXBuff)%TX_BUFF_LEN;	//Increment index, set to 0 on roll over
	}else{
		*m_UCSRB &= ~(1<<UDRIE);							//Disable Buffer empty interrupt
	}
}


//These tie the ISRs to their respective USART class object 
ISR(USART_UDRE_vect){
	USART0.m_interruptTransmitData();
}

ISR(USART_RX_vect){
	USART0.m_interruptStoreData();
}

//Definition of the External classes of USART
USART USART0(&UCSR0A,&UCSR0B,&UCSR0C,&UBRR0H,&UBRR0L,&UDR0);