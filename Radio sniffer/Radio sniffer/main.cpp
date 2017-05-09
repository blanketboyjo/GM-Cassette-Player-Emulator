/*
 * Radio sniffer.cpp
 *
 * Created: 8/24/2016 10:25:08 AM
 * Author : Jordan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"
#define ENABLEPIN			DDRB7
#define SENSEPIN			DDRD2
#define LEDPIN				DDRB5
#define WAITINGFORSTART		0
#define TIMINGLOW			1
#define WAITINGFORNEXTBIT	2
#define MESSAGEREADY		3
#define SENSERISING			3
#define SENSEFALLING		2

uint8_t volatile string[32];
uint8_t volatile stringIndex	=		0;
uint8_t volatile stage			=		0;
uint8_t volatile enabled		=		0;
//Sense pin interrupt
ISR(INT0_vect){
	//Switch case
	switch(stage){
	//Waiting for Start bit
	case WAITINGFORSTART: 
		//Reset timer
		TCNT1 = 0x000;
		//Switch interrupt to look for line rise
		EICRA = SENSERISING;
		//Increment Stage
		stage ++;
	break;
	//Last read was low
	case TIMINGLOW:
		//Check timer
		//If passed 120 uSec
		if(TCNT1 <2500){
			//Data is low
			//Add 48 to String
			string[stringIndex] = 48;
		//Else
		}else{
			//Data is high
			//Add 49 to string
			string[stringIndex] = 49;
		}
		//Increment string index
		stringIndex ++;
		//Reset timer
		TCNT1 = 0x000;
		//Lower timeout flag
		TIFR1 |= (1<<1);
		//Enable timeout interrupt
		TIMSK1 = (1<<OCIE0A);
		//Switch interrupt to look for line fall
		EICRA = SENSEFALLING;
		//Increment Stage
		stage ++;
	break;
	//Waiting for next bit
	case WAITINGFORNEXTBIT:
		//Disable timeout interrupt
		TIMSK1 = 0;
		//Reset timer
		TCNT1 = 0x000;
		//Switch interrupt to look for line rise
		EICRA = SENSERISING;
		//Switch to TIMINGLOW stage
		stage = TIMINGLOW;
	break;
	}
}

//Enable pin interrupt
ISR(PCINT0_vect){
	//If Enable pin is low (aka button pressed)
	if((PINB >>ENABLEPIN)==0){
		//If not enabled
		if(enabled ==0 && stage == WAITINGFORSTART){
			//Enable
			enabled = 1;
			//Configure pin interrupt for fall
			EICRA = SENSEFALLING;
			//Enable sense pin interrupt
			EIMSK = (1<<INT0);
			//Turn on led
			PORTB |= (1<<LEDPIN);
			//clear index
			stringIndex = 0;
		//If enabled
		}else{
			enabled = 0;
			EIMSK = 0;
			TIMSK1 = 0;
			//Turn off led
			PORTB &= ~(1<<LEDPIN);
			stage = WAITINGFORSTART;
		}
	}
}

//Timeout interrupt
ISR(TIMER1_COMPA_vect){
	//Disable timeout interrupt
	TIMSK1 = 0;
	//Switch stage to message complete
	stage = MESSAGEREADY;
}

int main(void)
{
	//Initialize USART
    USART0.init(250000);
	uint8_t message[] = {'s','t','a','r','t','\n'};
	USART0.write(message,6);
	//Configure sense pin
	DDRD &= ~(1<<SENSEPIN);
	//Configure enable pin
	DDRB &= ~(1<<ENABLEPIN);
	//Configure led pin
	DDRB |= (1<<LEDPIN);
	//Set timer compare to time it takes for timeout (extra time given to account for clock speed differences
	OCR1A = 21000;
	//Enable 16bit timer for timing low signal time and timeout
	TCCR1B = 0b00000001;
	//Enable pin change interrupt for enable pin (pcint7)
	PCMSK0 = 0b10000000;
	//Enable pcint0
	PCICR  = (1<<PCIE0);
	//Enable interrupts
	sei();
	enabled = 1;
	//Configure pin interrupt for fall
	EICRA = SENSEFALLING;
	//Enable sense pin interrupt
	EIMSK = (1<<INT0);
	//Turn on led
	PORTB |= (1<<LEDPIN);	
	
    while (1){
		//If message complete
		if(stage == MESSAGEREADY){
			//Send over usart
			USART0.write((uint8_t*)string,(uint8_t)stringIndex);
			uint8_t newline = '\n';
			USART0.write(newline);
			//Clear index
			stringIndex = 0;
			//Set stage to WAITINGFORSTART
			stage = WAITINGFORSTART;
			//If enabled != 1
		}
    }
}

