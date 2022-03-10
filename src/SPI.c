#include "SPI.h"

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly. 
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

void SPI2_GPIO_Init(void) {
	//start GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	GPIOB->MODER &= ~(GPIO_MODER_MODE13 | GPIO_MODER_MODE14 | GPIO_MODER_MODE15);
	GPIOB->MODER |= GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1 | GPIO_MODER_MODE15_1; //ALT FUNC MODE
	
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT13 | GPIO_OTYPER_OT14 | GPIO_OTYPER_OT15); //reset output to push pull
	
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED13 | GPIO_OSPEEDR_OSPEED14 | GPIO_OSPEEDR_OSPEED15; //set speed to very-high

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD13 | GPIO_PUPDR_PUPD14 | GPIO_PUPDR_PUPD15); //no pull-up, no pull-down
	
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL13_0 | GPIO_AFRH_AFSEL13_2; //SPI2_SCK 
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL14_0 | GPIO_AFRH_AFSEL14_2; //SPI2_MISO
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL15_0 | GPIO_AFRH_AFSEL15_2; //SPI2_MOSI (all are AF5)
}

void SPI2_Init(void){
	// TODO: initialize SPI2 peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; //SPI2 enabled
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST; 
	RCC->APB1RSTR1 &= ~(RCC_APB1RSTR1_SPI2RST); //set and reset to clear peripheral
	SPI2->CR1 &= ~(SPI_CR1_SPE); //SPI disable
	SPI2->CR1 &= ~(SPI_CR1_RXONLY);//full duplex
	SPI2->CR1 &= ~(SPI_CR1_BIDIMODE); //2-line unidirectional
	SPI2->CR1 &= ~(SPI_CR1_BIDIOE); //disabling output in bidirectioal mode (RECEIVE ONLY MODE)
	SPI2->CR1 &= ~(SPI_CR1_LSBFIRST); // MSB first
	SPI2->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_DS_3; //16-bit frame
	SPI2->CR2 &= ~(SPI_CR2_FRF); //motorola mode baby
	SPI2->CR1 &= ~(SPI_CR1_CPOL); // clock low polarity
	SPI2->CR1 &= ~(SPI_CR1_CPHA); //first clock phase is capture
	SPI2->CR1 &= ~(SPI_CR1_BR); //Baud rate = f/2 [000] 2MHz
	SPI2->CR1 &= ~(SPI_CR1_CRCEN); //hardware CRC disabled
	SPI2->CR1 |= (SPI_CR1_MSTR); //MASTER mode
	SPI2->CR1 |= (SPI_CR1_SSM); //enabling SSM
	SPI2->CR2 |= SPI_CR2_NSSP; //enabling NSS pulse gen
	SPI2->CR2 &= ~(SPI_CR2_FRXTH); //setting FIFO to 1/2
	SPI2->CR1 |= SPI_CR1_SPE; //SPI enable
}
 
void SPI_Transfer_Byte(SPI_TypeDef* SPIx, uint8_t write_data, uint8_t* read_data) {
	// TODO: perform SPI transfer
	// int i;
	// GPIOA->ODR ^= GPIO_ODR_OD5;
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = write_data; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_BSY) != 0); //wait for busy to be unset
	
	while((SPIx->SR & SPI_SR_RXNE) == 0); //wait for receive not empty flag to be set
	
	// for(i = 0; i < 5000000; i++);
	// GPIOA->ODR ^= GPIO_ODR_OD5;
	*read_data = (uint8_t)*((volatile uint8_t*)&SPIx->DR); //receive data from SPIx->DR

}
