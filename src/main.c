/*
 * ECE 153B - Winter 2022
 *
 * Name(s): Brian Li and Marko Ristic
 * Section:
 * Lab: Final Project
 */

#include "stm32l476xx.h"

#include "I2C.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

// Initializes USARTx
// USART2: UART Communication with Termite
void Init_USARTx(int x) {
	if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;
uint32_t dist = 0;
int32_t tempdist = 0;
uint32_t time4 = 0;
uint32_t time1 = 0;
int state = 0, displayed = 0;


void Input_Capture_Setup() {
	//setup PB6
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	GPIOB->MODER &= ~GPIO_MODER_MODE6;	
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6);
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6);

	//enable timer 4
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;

	//set prescaler to 3
	TIM4->PSC |= 0x0003;

	//enable auto-reload preload + set at max value
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->ARR |= 0xFFFF;

	//set input capture mode to timer input 1 (01)
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;

	//set bits for both rising/falling and enable capture
	TIM4->CCER |= TIM_CCER_CC1P | TIM_CCER_CC1NP | TIM_CCER_CC1E;

	//enable interrupt and DMA requests and update interrupt
	TIM4->DIER |= TIM_DIER_CC1IE | TIM_DIER_CC1DE | TIM_DIER_UIE;

	//enable update generation
	TIM4->EGR |= TIM_EGR_UG;

	//clear update interrupt flag
	TIM4->SR &= ~TIM_SR_UIF;

	//direction of counter and enable counter
	TIM4->CR1 &= ~TIM_CR1_DIR; 
	TIM4->CR1 |= TIM_CR1_CEN;

	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 2);
}

void TIM4_IRQHandler(void) {
	if((TIM4->SR & TIM_SR_UIF) != 0){
		overflowCount++;
		TIM4->SR &= ~(TIM_SR_UIF);
	}else if((TIM4->SR & TIM_SR_CC1IF) != 0){

		if((GPIOB->IDR & GPIO_IDR_ID6) != 0){ //current is a rising signal, do nothing
			overflowCount = 0;
			lastValue = TIM4->CCR1;
		} else { //current is falling signal, calculate time duration
			currentValue = TIM4->CCR1;
			if(overflowCount > 0){ //overflow
				timeInterval = (currentValue - lastValue + (65536*overflowCount));
			} else {
				timeInterval = currentValue - lastValue;
			}
		}
		TIM4->SR &= ~TIM_SR_CC1IF;
	}
}

void Trigger_Setup() {
	//set up PA9
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODE9;	
	GPIOA->MODER |= GPIO_MODER_MODE9_1;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL9);
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0;

	//enable timer 1
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	//set prescaler to 3
	TIM1->PSC |= 0x0003;

	//enable auto-reload preload + set at max value
	TIM1->CR1 |= TIM_CR1_ARPE;
	TIM1->ARR |= 0xFFFF;

	//set CCR to 10
	TIM1->CCR2 |= 0x000A;

	//output compare mode to PWM 1 (0110) and enable output compare preload
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;

	//enable output
	TIM1->CCER |= TIM_CCER_CC2E;

	//main output enable and off-state selection
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR;
	
	//enable update generation
	TIM1->EGR |= TIM_EGR_UG;

	//enable update interrupt and clear interrupt
	TIM1->DIER |= TIM_DIER_UIE;
	TIM1->SR &= ~TIM_SR_UIF;

	//set direction (down) and enable counter
	TIM1->CR1 |= TIM_CR1_DIR | TIM_CR1_CEN;
}

int main(void) {
	//VARIABLE DECLARATIONS
	int i;
	uint8_t SlaveAddress_Nunchuk, SlaveAddress_Display;
	uint8_t Wii_Data_Receive[6] = {0};
	uint8_t Wii_init_send = 0x00;
	int8_t send_error;
	int8_t receive_error;
	
	uint8_t* preceive = Wii_Data_Receive;
	SlaveAddress_Nunchuk = (0x52 << 1);
	SlaveAddress_Display = (0x27 << 1);

	//INITIALIZATIONS
	System_Clock_Init(); // System Clock = 4 MHz
	SysTick_Init();

	// Initialize I2C Buses
	I2C1_GPIO_Init();
	I2C1_Initialization();
	I2C2_GPIO_Init();
	I2C2_Initialization();
	
	// Initialize UART for Termite
	Init_USARTx(2);
	
	// Initialize Ultrasonic Sensor
	Input_Capture_Setup();
	Trigger_Setup();

	//Initialize LCD Display
	LCD_Init(I2C2, SlaveAddress_Display);
	delay(10);
	
	//Initialize Nunchuk
	Nunchuk_Init(I2C1, SlaveAddress_Nunchuk);
	
	while(1) {
		if(state == 0){ //starting screen
			if(displayed == 0){
				LCD_Clear(I2C2, SlaveAddress_Display);
				delay(1);
				LCD_Put_Cur(I2C2, SlaveAddress_Display, 0, 0);
				delay(1);
				LCD_Send_String(I2C2, SlaveAddress_Display, "Back Up Sensor");
				delay(1);
				LCD_Put_Cur(I2C2, SlaveAddress_Display, 1, 0);
				delay(1);
				LCD_Send_String(I2C2, SlaveAddress_Display, "Press C to Start");
				delay(1);

				displayed = 1;
			}
			
			//poll for Nunchuk inputs
			send_error = I2C_SendData(I2C1, SlaveAddress_Nunchuk, &Wii_init_send, 1);
			delay(10);
			receive_error = I2C_ReceiveData(I2C1, SlaveAddress_Nunchuk, preceive, 6);

			if((preceive[5] & 0x02) == 0){ //if C button pressed, switch states
				state = 1;
				displayed = 0;
			}
			delay(300); //needed so state doesn't continuously flip back and forth
			
		} else{ //Distance Display Code
			LCD_Clear(I2C2, SlaveAddress_Display);

			//Poll for Ultrasonic Sensor Measurement
			if(timeInterval > 25000){
				dist = 0x00;
			}
			time4 = TIM4->CNT;
			time1 = TIM1->CNT;
			dist = (timeInterval)/58;

			//Calculate Output Index
			tempdist = 15 - ((dist - 2)*16/100);
			if(tempdist > 12){
				printf("Too close! Drive forward!\n");
			}
			for(i = 0; i < tempdist; i++){
				LCD_Put_Cur(I2C2, SlaveAddress_Display, 0, i);
				delay(1);
				LCD_Send_Data(I2C2, SlaveAddress_Display, 0xFF);
				delay(10);
				LCD_Put_Cur(I2C2, SlaveAddress_Display, 1, i);
				delay(1);
				LCD_Send_Data(I2C2, SlaveAddress_Display, 0xFF);
				delay(10);
				
			}
			
			//poll for Nunchuk inputs
			send_error = I2C_SendData(I2C1, SlaveAddress_Nunchuk, &Wii_init_send, 1);
			delay(10);
			receive_error = I2C_ReceiveData(I2C1, SlaveAddress_Nunchuk, preceive, 6);
			
			if((preceive[5] & 0x02) == 0){ //if C button pressed, switch states
				state = 0;
				displayed = 0;
			}
			delay(300);
		}
	
		for(i = 0; i < 1000; ++i); 
	}
}
