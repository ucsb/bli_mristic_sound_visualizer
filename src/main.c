#include "stm32l476xx.h"

#include "I2C.h"
#include "SPI.h"
#include "SysClock.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>

#define SPI_MASTER SPI2
#define SPI_MASTER SPI1
#define SPI_SLAVE SPI3 //not needed

uint16_t receive = 0;
int *a; //a maintains the LED height 

int main(void) {
    System_Clock_Init();   // System Clock = 4MHz
	
	SPI2_GPIO_Init();
	SPI2_Init();
    SPI1_GPIO_Init();
    SPI1_Init();
	int i = 0;
    int mag;
    //Note that the microphone's L/R pin should be connected to Vdd (3.3v)
	while(1) { //sampling wave mag from mic]
        if(i%256){
            SPI_LED_Reset(SPI1);
        }else if(i%4){
            SPI_Set_Green(SPI1);
        }
        else {
            SPI_Set_Off(SPI1);
        }
        i++;
        /*SPI_Receive_Waveform(SPI2, &receive);
        //decode receive to get magnitude value.
        mag = 0;
        for(i = 0; i < 16; i++){
            mag += receive%2;
            receive = receive/2;
        }
        mag = mag/2;
        for(i = 1; i < 32; i++){
            *(a + i) = *(a);
        }
        *(a) = mag;
        //mag should now be equal to a value E [0,8]
        //we can have 0 not be displayed on the LED (lowest possible val), and 1-8 correspond to height of max LED
        */
	}

}