#include "UART.h"

void UART1_Init(void) {
	// [TODO]
	
	//enable USART1 clk
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	//select system clk as USART1 clk source (01)
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;
}

void UART2_Init(void) {
	// [TODO]

	//enable USART2 clk
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

	//select system clk as USART2 clk source (01)
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
}

void UART1_GPIO_Init(void) {
	// [TODO]
	
	//enable GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	//set mode to alternate
	GPIOB->MODER &= ~GPIO_MODER_MODE6;	
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE7;	
	GPIOB->MODER |= GPIO_MODER_MODE7_1;
	
	//use alternate functions for PB6, PB7 (AF7) for USART2
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_0 | GPIO_AFRL_AFSEL6_1 | GPIO_AFRL_AFSEL6_2;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_0 | GPIO_AFRL_AFSEL7_1 | GPIO_AFRL_AFSEL7_2;

	//set speeds to very high
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7;

	//set to push pull output type
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT6;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT7;

	//set to pull-up register (01)
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0;
}

void UART2_GPIO_Init(void) {
	// [TODO]
	
	//enable GPIOA
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//set mode to alternate
	GPIOA->MODER &= ~GPIO_MODER_MODE2;	
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE3;	
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	
	//use alternate functions for PA2, PA3 (AF7) for USART2
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2;

	//set speeds to very high
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2 | GPIO_OSPEEDR_OSPEED3;

	//set to push pull output type
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;

	//set to pull-up register (01)
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD3_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	// [TODO]

	//disable USARTx
	USARTx->CR1 &= ~USART_CR1_UE;

	//set word length to 8 (M[1:0] = 00)
	USARTx->CR1 &= ~USART_CR1_M;

	//oversample to 16
	USARTx->CR1 &= ~USART_CR1_OVER8;

	//stop bits to 1
	USARTx->CR2 &= ~USART_CR2_STOP;

	//baud rate to 9600 (USARTDIV = 80MHz/9600 = 8333 (0x208D))
	USARTx->BRR |= 0x208D;

	//enable transmitter and receiver
	USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;

	//enable USART
	USARTx->CR1 |= USART_CR1_UE;

}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}
