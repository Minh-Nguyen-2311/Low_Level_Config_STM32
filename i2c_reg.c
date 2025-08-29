#include "I2C.h"
#include "RCC_Config.h"

void I2C_Config(void)
{
	/*1: Enable I2C clock and GPIO clock*/
	RCC->APB1ENR |= (1<<21); //Enable I2C clock
	RCC->AHB1ENR |= (1<<1); //Enable GPIOB clock
	
	/*2: Configure I2C pin for alternate functions*/
	GPIOB->MODER |= (2<<16) | (2<<18); //Bit (17:16), (19:18) = 1:0 --> Alternate function for PB8, PB9
	GPIOB->OTYPER |= (3<<8); //Bit (8:9) = 1:1 --> Output open drain
	GPIOB->OSPEEDR |= (3<<16) | (3<<18);  // Bits (17:16), (19:18) = 1:1 --> High Speed for PB8, PB9
	GPIOB->PUPDR |= (1<<16) | (1<<18);  // Bits (17:16), (19:18)= 0:1 --> Pull up for PB8, PB9
	GPIOB->AFR[1] |= (4<<0) | (4<<4);  // Bits (3:2:1:0), (7:6:5:4) = 0:1:0:0 --> AF4 for PB8, PB9
	
	/*3: Reset I2C*/
	I2C1->CR1 |= (1<<15);
	I2C1->CR1 &= ~(1<<15);
	
	/*4: Program peripheral input clock in I2C_CR2 register*/
	I2C1->CR2 |= (45<<0); 
	
	/*5: Configure the clock control register*/
	I2C1->CCR = (225<<0);
	
	/*6: Configure the rise time register*/
	I2C1->TRISE = 46;
	
	/*7: Configure I2C_CR1 register to enable peripheral*/
	I2C1->CR1 |= (1<<0);
}

void I2C_Start(void)
{
	I2C1->CR1 |= (1<<10); //Enable ACK
	I2C1->CR1 |= (1<<8); //Enable START
	//Wait for SB bit to set
	while(!(I2C1->SR1 & (1<<0)));
}

void I2C_Write (uint8_t data)
{	
	while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & (1<<2)));  // wait for BTF bit to set
}

void I2C_Address (uint8_t Address)
{
	I2C1->DR = Address;  //  send the address
	while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
}
	
void I2C_Stop (void)
{
	I2C1->CR1 |= (1<<9);  // Stop I2C
}