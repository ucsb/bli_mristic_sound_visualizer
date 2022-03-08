#include "stm32l476xx.h"

#include "I2C.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

int main(void) {
    // Initialization
    System_Clock_Init(); // Switch System Clock = 16 MHz

    I2C_GPIO_Init();
    I2C_Initialization();

    int i;
	uint8_t SlaveAddress;
	uint8_t Data_Receive;
	uint8_t Data_Send;
	int8_t send_error;
	int8_t receive_error;

    // Determine Slave Address
    //
    // Note the "<< 1" must be present because bit 0 is treated as a don't care in 7-bit addressing mode
    SlaveAddress = (82 << 1); // STUB - Fill in correct address 0b1010010 (0x52)
    Data_Send = 0x40;
	send_error = I2C_SendData(I2C1, SlaveAddress, &Data_Send, sizeof(Data_Send));
    Data_Send = 0x00;
	send_error = I2C_SendData(I2C1, SlaveAddress, &Data_Send, sizeof(Data_Send));
    Data_Send = 0x00;
	send_error = I2C_SendData(I2C1, SlaveAddress, &Data_Send, sizeof(Data_Send));
	while(1) {	
		
		
		// [TODO] - Get Temperature
		// 
		// First, send a command to the sensor for reading the temperature
		// Next, get the measurement
		//printf("start\n");
		Data_Send = 0x00;
		send_error = I2C_SendData(I2C1, SlaveAddress, &Data_Send, sizeof(Data_Send));
		//printf("send_error: %i\n", send_error);

		receive_error = I2C_ReceiveData(I2C1, SlaveAddress, &Data_Receive, sizeof(Data_Receive));
		//printf("receive_error: %i\n", receive_error);

		// [TODO] - Print Temperature to Termite
		printf("Temperature: %iC\n", Data_Receive);
		// Some delay
		for(i = 0; i < 50000; ++i); 
	}
}

