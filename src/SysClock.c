#include "SysClock.h"

void System_Clock_Init(void) {
	RCC->CR |= 9UL<<4;
	RCC->CR &= ~(3UL << 5); //setting MSI to 24MHz
	RCC->CR |= RCC_CR_MSION;  // enable  MSI
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
	
	// Selecting MSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW; //MSI as clock src

	while((RCC->CFGR & RCC_CFGR_SWS) != 0);
}
