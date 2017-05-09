/*
 * PhoneControlls.h
 * 
 * Currently phone uses PD4 as control pin
 *
 * Created: 9/12/2016 2:48:27 PM
 *  Author: Jordan
 */ 


#ifndef PHONECONTROLLS_H_
#define PHONECONTROLLS_H_
static void PhoneInit(void){
	DDRD |= (1<<PORTD4);
}

static void PhoneForward(void){//Todo: phone forward via interrupts
	
}

static void PhoneReverse(void){//Todo: phone reverse via interrupts
	
}




#endif /* PHONECONTROLLS_H_ */