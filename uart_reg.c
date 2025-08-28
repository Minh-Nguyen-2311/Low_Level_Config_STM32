#include "RCC_Config.h"
#include "Delay.h"

void UARTConfig(void)
{
	/*Steps followed*/
	
	/*1: Enable GPIO clock and UART clock*/
	RCC->APB1ENR |= (1<<17); //Enable USART2
	RCC->APB1ENR |= (1<<0); //Enable GPIOA 
	
	/*2: Config UART pins for alternate functions*/
	GPIOA->MODER |= (2<<4); //Bits (5:4) = 1:0 --> Alternate function for PA2
	GPIOA->MODER |= (2<<6); //Bits (6:7) = 1:0 --> Alternate function for PA3
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6); //Bits (5:4),(6:7) = 1:1 --> High speed for PA2,PA3
	
	GPIOA->AFR[0] |= (7<<8); //Bit (8:9:10:11) = 1:1:1:0 --> AF7 alternate function for PA2
	GPIOA->AFR[0] |= (7<<12); //Bit (12:13:14:15) = 1:1:1:0 --> AF7 altervate function for PA3
	
	/*3: Enable UART by writing UE bit in USART_CR1 to 1*/
	USART2->CR1 = 0x00; //Clear all
	USART2->CR1 |= (1<<13); //UE = 1
	
	/*4: Program the M bit in USART_CR1 to define the word length*/
	USART2->CR1 |= ~(1<<12); //M=0 --> 8-bit word length
	
	/*5: Set baud rate in USART_BRR*/
	USART2->BRR = (7<<0) | (24<<4); //Baud rate 115200 is set
	
	/*6: Enable the Transmitter/Receiver by setup TE/RE bits in USART_CR1*/
	USART2->CR1 |= (1<<2); //RE=1
	USART2->CR1 |= (1<<3); //TE=1
}

void UARTSendChar(char c)
{
	USART2->DR = (uint8_t)c;
	while(!(USART2->SR & (1<<6))); //Wait for TC to set --> Data transmitted
}

void UARTSendString(char *str)
{
	while(*str)
	{
		UARTSendChar(*str++)
	}
}

int main()
{
	SystemClockConfig();
	TIMConfig();
	UARTConfig();
	
	while(1)
	{
		UARTSendString('Geeksforgeeks');
		Delay(500);
	}
}