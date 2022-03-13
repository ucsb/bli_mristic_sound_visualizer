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
    int j = 0;
    int k = 0;
    *(a) = 0;
    int mag;
    //Note that the microphone's L/R pin should be connected to Vdd (3.3v)
	while(1) { //sampling wave mag from mic]
        if(k%256){
            SPI_LED_Reset(SPI1);
        }else if(k%4){
            SPI_Set_Green(SPI1);
        }
        else {
            SPI_Set_Off(SPI1);
        }
        k++;
        //^^ TESTING CODE
        /*
        //vvv READING FROM MIC AND DISPLAY WAVEFORM CODE
        SPI_Receive_Waveform(SPI2, &receive);
        //decode receive to get magnitude value.
        mag = 0;
        for(i = 0; i < 16; i++){
            mag += receive%2;
            receive = receive/2;
        }
        mag = mag/2; 
        //mag should now be equal to a value E [0,8]
        //we can have 0 not be displayed on the LED (lowest possible val), and 1-8 correspond to height of max LED
        for(i = 1; i < 32; i++){
            *(a + i) = *(a);
        }
        *(a) = mag;
        //^^^ updating array of LED heights
        //vvv Sequence of display instructions
        for(i = 31; i >= 0; i--){
            if(i%2) { //normal behavior display up to mag,
                for(j = 0; j < 8; j++){
                    if(j < *(a+i)){
                        SPI_Set_Green(SPI1);
                    } else[
                        SPI_Set_Off(SPI1);
                    ]
                }
            } else{ //inverse as going from top to bottom instead of bottom to top
                for(j = 0; j < 8; j++){
                    if(j < 8 - *(a+i)){
                        SPI_Set_Off(SPI1);
                    } else{
                        SPI_Set_Green(SPI1);
                    }
                }
            }
        }
        SPI_LED_Reset(SPI1); //send reset signal so board updates
        */
	}

}