/*
 * USART.h
 *
 * Created: 9/12/2016 2:06:20 PM
 *  Author: Jordan
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
class USART{
	#define RXEN  4			//Offset for RX enable
	#define TXEN  3			//Offset for TX enable
	#define UDRIE 5			//Offset for UDR empty interrupt enable
	#define RXCIE 7			//Offset for RX complete interrupt enable
	#define USBS  3			//Offset for Stop bit selection
	#define UCSZ0 1			//Offset for Character transmission length
	#define RX_BUFF_LEN 32	//Length of RX buffer
	#define TX_BUFF_LEN 32	//Length of TX buffer

	public:
	USART(volatile uint8_t *UCSRA,		//Pointer to UCSRA
	volatile uint8_t *UCSRB,			//Pointer to UCSRB
	volatile uint8_t *UCSRC,			//Pointer to UCSRC
	volatile uint8_t *UBRRH,			//Pointer to UBRRL
	volatile uint8_t *UBRRL,			//Pointer to UBRRH
	volatile uint8_t *UDR);				//Pointer to UDR
	void init(uint32_t speed);					//Setup USART with a set speed
	void end(void);								//Shutdown USART
	void write(uint8_t data);					//Add a character to TXBuff
	void write(uint8_t data[], uint8_t length); //Add a character string to TXBuff
	uint8_t read(void);							//Read a single byte from the RXBuff
	uint8_t unreadData(void);						//Returns number of new data in RXBuff
	void m_interruptTransmitData(void);			//Handler for actual data transmission
	void m_interruptStoreData(void);			//Handler for actual data receive
	private:	//All variables made private to protect from outside access
	volatile uint8_t  	*m_UCSRA;				//Pointer to USART control A reg
	volatile uint8_t 	*m_UCSRB;				//Pointer to USART control B reg
	volatile uint8_t 	*m_UCSRC;				//Pointer to USART control C reg
	volatile uint8_t 	*m_UBRRH;				//Pointer to upper speed reg
	volatile uint8_t 	*m_UBRRL;				//Pointer to lower speed reg
	volatile uint8_t 	*m_UDR;					//Pointer to data reg
	volatile uint8_t	m_nxtRXBuff;			//Index of RXBuff for next read call
	volatile uint8_t	m_eoRXBuff;				//Index of RXBuff for new data
	volatile uint8_t	m_nxtTXBuff;			//Index of TXBuff for next send routine
	volatile uint8_t	m_eoTXBuff;				//Index of TXBuff for new data
	volatile uint8_t	m_TXBuff[TX_BUFF_LEN];	//Buffer for TX
	volatile uint8_t	m_RXBuff[RX_BUFF_LEN];	//Buffer for RX
};

extern USART USART0;			//Declares the front end objects for later usage

#endif /* USART_H_ */