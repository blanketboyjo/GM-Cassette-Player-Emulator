/*
 * USART0.c
 *
 * Created: 4/25/2016 10:59:27 AM
 *  Author: Jordan
 */ 
#include "USART0.h"

USART0::USART0(void){
	
}

void USART0::init(uint32_t speed){
	// TODO determine speed setting for UBRR
	// TODO set speed setting for USART
	// TODO init USART code
}

void USART0::write(uint8_t data){
	// TODO write function for sending single char/uint8_t of data	
}

void USART0::write(uint8_t data[], uint8_t length){
	// TODO write function for sending char/uint8_t lists of data
}

uint8_t USART0::dataAvailable(void){
	// TODO Write the math for counting how much data is in buffer
	return 0;
}

void USART0::usartReceiveHandler(void){
	// TODO Write a complete ISR for loading the data buffer 
}
