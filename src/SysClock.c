#include "SysClock.h"

void System_Clock_Init(void){
	
	RCC->CR &= RCC_CR_MSION;
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_6; //setting MSI to 4MHz

	//RCC->CFGR &= ~RCC_CFGR_HPRE;  // AHB prescaler = 1; SYSCLK not divided
	//RCC->CFGR &= ~RCC_CFGR_PPRE1; // APB high-speed prescaler (APB1) = 1, HCLK not divided
	//RCC->CFGR &= ~RCC_CFGR_PPRE2; // APB high-speed prescaler (APB2) = 1, HCLK not divided
  	RCC->CR |= RCC_CR_MSIRGSEL; // Replace with value that will select range in RCC->CR
	RCC->CR |= RCC_CR_MSION;  // enable  MSI
	//RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
	
	// Selecting MSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW; //MSI as clock src

	while((RCC->CFGR & RCC_CFGR_SWS) != 0);
}
