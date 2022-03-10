#include "SPI.h"


void SPI2_GPIO_Init(void) {
	//start GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	GPIOB->MODER &= ~(GPIO_MODER_MODE13 |  GPIO_MODER_MODE15);
	GPIOB->MODER |= GPIO_MODER_MODE13_1 | GPIO_MODER_MODE15_1; //ALT FUNC MODE
	
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT13 | GPIO_OTYPER_OT15); //reset output to push pull
	
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED13 | GPIO_OSPEEDR_OSPEED15; //set speed to very-high

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD13 | GPIO_PUPDR_PUPD15); //no pull-up, no pull-down
	
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL13_0 | GPIO_AFRH_AFSEL13_2; //SPI2_SCK 
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL15_0 | GPIO_AFRH_AFSEL15_2; //SPI2_MOSI (all are AF5)
}

void SPI2_Init(void){
	// TODO: initialize SPI2 peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; //SPI2 enabled
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST; 
	RCC->APB1RSTR1 &= ~(RCC_APB1RSTR1_SPI2RST); //set and reset to clear peripheral
	SPI2->CR1 &= ~(SPI_CR1_SPE); //SPI disable
	SPI2->CR1 &= ~(SPI_CR1_BIDIMODE); //2-line unidirectional
	SPI2->CR1 |= SPI_CR1_RXONLY; //RECEIVE ONLY MODE 
	SPI2->CR1 &= ~(SPI_CR1_BIDIOE); //disabling output in bidirectioal mode
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

void SPI_Receive_Waveform(SPI_TypeDef *SPIx, uint16_t* read_data){
	while((SPIx->SR & SPI_SR_BSY) != 0); //wait for busy to be unset
	while((SPIx->SR & SPI_SR_RXNE) == 0); //wait for receive not empty flag to be set
	*read_data = (uint16_t)*((volatile uint16_t*)&SPIx->DR); //receive data from SPIx->DR

}