#ifndef __STM32L476R_NUCLEO_SPI_H
#define __STM32L476R_NUCLEO_SPI_H

#include "stm32l476xx.h"

void SPI2_GPIO_Init(void);
void SPI1_GPIO_Init(void);
void SPI2_Init(void);
void SPI1_Init(void);
void SPI_Receive_Waveform(SPI_TypeDef *SPIx, uint16_t* read_data);
void SPI_Transmit_Color(SPI_TypeDef *SPIx, uint8_t g, uint8_t r, uint_8t b);
void SPI_Set_Green(SPI_TypeDef *SPIx);
void SPI_Set_Off(SPI_TypeDef *SPIx);

#endif 
