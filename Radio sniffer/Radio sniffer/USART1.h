/*
 * USART1.h
 *
 * Created: 4/25/2016 10:03:02 AM
 *  Author: Jordan
 */ 


#ifndef USART1_H_
#define USART1_H_
#define RECEIVE_BUFFER_SIZE 32

class USART1
{
public:
	USART1(void);	
	void init(uint32_t speed);
	void write(uint8_t data);
	void write(uint8_t data[], uint8_t length);
	uint8_t dataAvailable(void);
private:
	volatile uint8_t receivedData[RECEIVE_BUFFER_SIZE];
	volatile uint8_t loadingIndex, readingIndex;
	void usartReceiveHandler(void);
};

extern USART1 Serial1;


#endif /* USART1_H_ */