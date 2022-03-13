#include "stm32l476xx.h"

#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

// Initializes USARTx
// USART2: UART Communication with Termite
// USART1: Bluetooth Communication with Phone
void Init_USARTx(int x) {
	if(x == 1) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else if(x == 2) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else {
		// Do nothing...
	}
}

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 4 MHz

    I2C_GPIO_Init();
    I2C_Initialization();

	Init_USARTx(2);
	printf("hello\n");

    int i,j;
	uint8_t SlaveAddress;
	uint8_t Data_Receive;
	uint8_t Data_Send;
	int8_t send_error;
	int8_t receive_error;

    // Determine Slave Address
    //
    // Note the "<< 1" must be present because bit 0 is treated as a don't care in 7-bit addressing mode
    SlaveAddress = (82 << 1); // STUB - Fill in correct address 0b1010010 (0x52)
	send_error = I2C_NunchuckInit(I2C1, SlaveAddress);
	if(send_error != 0) printf("error in Nunchuck init %i\n", send_error); else printf("Nunchuck Initialized\n");

	uint8_t nunchuckData[6];
	uint8_t* p_nunchuck = nunchuckData;
	while(1) {	
		
		receive_error = I2C_ReceiveData(I2C1, SlaveAddress, p_nunchuck, 6);

		for(i = 0; i < 6; i++){
			printf("offset 0x0%i: %x\n", i, nunchuckData[i]);
		}
		// Some delay
		for(j = 0; j < 50000; ++j); 
	}
}

