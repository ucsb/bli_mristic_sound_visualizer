#include "SPI.h"
#define MAX 1
void SPI2_GPIO_Init(void) {
	//start GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	GPIOB->MODER &= ~(GPIO_MODER_MODE13 |  GPIO_MODER_MODE14);
	GPIOB->MODER |= GPIO_MODER_MODE13_1 | GPIO_MODER_MODE14_1; //ALT FUNC MODE
	
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT13 | GPIO_OTYPER_OT14); //reset output to push pull
	
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED13 | GPIO_OSPEEDR_OSPEED14; //set speed to very-high

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD13 | GPIO_PUPDR_PUPD14); //no pull-up, no pull-down
	
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL13_0 | GPIO_AFRH_AFSEL13_2; //SPI2_SCK 
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL14_0 | GPIO_AFRH_AFSEL14_2; //SPI2_MISO
}

void SPI1_GPIO_Init(void) {
	//using PA5,6,7 for SPI1 SCK, MISO, & MOSI
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	GPIOB->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7);
	GPIOB->MODER |= GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1; //ALT FUNC MODE
	
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7); //reset output to push pull
	
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7; //set speed to very-high

	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7); //no pull-up, no pull-down
	
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL5_0 | GPIO_AFRH_AFSEL5_2; //SPI1_SCK 
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL6_0 | GPIO_AFRH_AFSEL6_2; //SPI1_MISO
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL7_0 | GPIO_AFRH_AFSEL7_2; //SPI1_MOSI (all are AF5)
	
}

void SPI2_Init(void){
	// TODO: initialize SPI2 peripheral
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN; //SPI2 enabled
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_SPI2RST; 
	RCC->APB1RSTR1 &= ~(RCC_APB1RSTR1_SPI2RST); //set and reset to clear peripheral
	SPI2->CR1 &= ~(SPI_CR1_SPE); //SPI disable
	SPI2->CR1 &= ~(SPI_CR1_RXONLY); //full fuplex
	SPI2->CR1 |= SPI_CR1_BIDIMODE; //1-line bidirectional
	SPI2->CR1 &= ~(SPI_CR1_BIDIOE); //disabling output in bidirectioal mode (RECEIVE ONLY)
	SPI2->CR1 &= ~(SPI_CR1_LSBFIRST); // MSB first
	SPI2->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_DS_3; //16-bit frame
	SPI2->CR2 &= ~(SPI_CR2_FRF); //motorola mode baby
	SPI2->CR1 &= ~(SPI_CR1_CPOL); // clock low polarity
	SPI2->CR1 &= ~(SPI_CR1_CPHA); //first clock phase is capture
	SPI2->CR1 &= ~(SPI_CR1_BR); 
	SPI2->CR1 |= SPI_CR1_BR_2//f = 24MHz -> set presc to 16/2^4 (100), giving us f = 1.5MHz
	SPI2->CR1 &= ~(SPI_CR1_CRCEN); //hardware CRC disabled
	SPI2->CR1 |= (SPI_CR1_MSTR); //MASTER mode
	SPI2->CR1 |= (SPI_CR1_SSM); //enabling SSM
	SPI2->CR2 |= SPI_CR2_NSSP; //enabling NSS pulse gen
	SPI2->CR2 &= ~(SPI_CR2_FRXTH); //setting FIFO to 1/2
	SPI2->CR1 |= SPI_CR1_SPE; //SPI enable
}

void SPI1_Init(void){
	// TODO: initialize SPI1 peripheral
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //SPI1 enabled
	RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; 
	RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI1RST); //set and reset to clear SPi1 peripheral
	SPI1->CR1 &= ~(SPI_CR1_SPE); //SPI disable
	SPI1->CR1 &= ~(SPI_CR1_RXONLY); //full fuplex
	SPI1->CR1 |= SPI_CR1_BIDIMODE; //1-line bidirectional
	SPI1->CR1 |= SPI_CR1_BIDIOE; //disabling input in bidirectioal mode (TRANSMIT ONLY)
	SPI1->CR1 &= ~(SPI_CR1_LSBFIRST); // MSB first
	SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2 ; //8-bit frame
	SPI1->CR2 &= ~(SPI_CR2_FRF); //motorola mode baby
	SPI1->CR1 &= ~(SPI_CR1_CPOL); // clock low polarity
	SPI1->CR1 &= ~(SPI_CR1_CPHA); //first clock phase is capture
	SPI1->CR1 &= ~(SPI_CR1_BR); 
	SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1//Baud rate = f/8 [011] 2MHz, f = 24MHz, set Baud Rate to f/8 (2^3) = 3MHz
	SPI1->CR1 &= ~(SPI_CR1_CRCEN); //hardware CRC disabled
	SPI1->CR1 |= (SPI_CR1_MSTR); //MASTER mode
	SPI1->CR1 |= (SPI_CR1_SSM); //enabling SSM
	SPI1->CR2 |= SPI_CR2_NSSP; //enabling NSS pulse gen
	SPI1->CR2 |= SPI_CR2_FRXTH; //setting FIFO to 1/4
	SPI1->CR1 |= SPI_CR1_SPE; //SPI enable

}

void SPI_Receive_Waveform(SPI_TypeDef *SPIx, uint16_t* read_data){
	while((SPIx->SR & SPI_SR_BSY) != 0); //wait for busy to be unset
	while((SPIx->SR & SPI_SR_RXNE) == 0); //wait for receive not empty flag to be set
	*read_data = (uint16_t)*((volatile uint16_t*)&SPIx->DR); //receive data from SPIx->DR
}

//1 = 110
//0 = 100
/* void SPI_Transmit_Color(SPI_TypeDef *SPIx, uint8_t g, uint8_t r, uint8_t b){
	uint8_t g1, r1, b1;
	g1 = 0;
	r1 = 0;
	b1 = 0;
	//so as every 3 bits = bit, need to send 24 x 3 = 72 bits, 72/8 = 9 SPI transfers
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	g1 = g>>5;
	*((volatile uint8_t*)&SPIx->DR) = ; //write data to SPIx->DR
} */

//11011011 = 3 + 24 + 64 + 128 = 219
//01101101 = 1 + 12 + 32 + 64 = 109
//10110110 = 6 + 16 + 32 + 128 = 182
void SPI_Set_Green(SPI_TypeDef *SPIx) {
	int i;
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 146; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 109; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 182; //write data to SPIx->DR
	for(i = 0; i < 2; i++){ //setting red & blue to 0
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 146; //write data to SPIx->DR
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 73; //write data to SPIx->DR
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 36; //write data to SPIx->DR
	}
}

void SPI_Set_Blue(SPI_TypeDef *SPIx) {
	int i;
	for(i = 0; i < 2; i++){ //setting green & red equal to 0
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 146; //write data to SPIx->DR
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 73; //write data to SPIx->DR
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 36; //write data to SPIx->DR
	}
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 146; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 109; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 182; //write data to SPIx->DR
}

void SPI_Set_Red(SPI_TypeDef *SPIx) {
	//set green to 0
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 146; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 73; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 36; //write data to SPIx->DR
	//set red to max
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 146; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 109; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 182; //write data to SPIx->DR
	//set blue to 0
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 146; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 73; //write data to SPIx->DR
	while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
	*((volatile uint8_t*)&SPIx->DR) = 36; //write data to SPIx->DR
} 


//10010010 = 2 + 16 + 128 = 146
//01001001 = 1 + 8 + 64 = 73
//00100100 = 4 + 32 = 36
void SPI_Set_Off(SPI_TypeDef *SPIx) {
	int i;
	for(i = 0; i < 3; i++){ //setting green, red, & blue to 0
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 146; //write data to SPIx->DR
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 73; //write data to SPIx->DR
		while((SPIx->SR & SPI_SR_TXE) == 0); //wait for transmit buffer empty flag to be set
		*((volatile uint8_t*)&SPIx->DR) = 36; //write data to SPIx->DR
	}
}
void SPI_LED_Reset(SPI_TypeDef *SPIx){
	int i;
	for(i = 0; i < 19){
		while((SPIx->SR & SPI_SR_TXE) == 0);
		*((volatile uint8_t*)&SPIx->DR) = 0;
	}
}