#ifndef __STM32L476R_NUCLEO_I2C_H
#define __STM32L476R_NUCLEO_I2C_H

#include <stdint.h>
#include <cstddef>
#include "stm32l476xx.h"

#define READ_FROM_SLAVE 1
#define WRITE_TO_SLAVE  0

int8_t I2C_Start(I2C_TypeDef * I2Cx, uint32_t DevAddress, uint8_t Size, uint8_t Direction);
void I2C_Stop(I2C_TypeDef * I2Cx);
void I2C_WaitLineIdle(I2C_TypeDef * I2Cx);
int8_t I2C_SendData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size);
int8_t I2C_ReceiveData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size);
void I2C1_GPIO_Init(void);
void I2C1_Initialization(void);
void I2C2_GPIO_Init(void);
void I2C2_Initialization(void);
void CODEC_Initialization(void);

void LCD_Send_Command(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, char cmd);
void LCD_Send_Data(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, char cmd);
void LCD_Init(I2C_TypeDef * I2Cx, uint8_t DeviceAddress);
void LCD_Send_String(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, char *str);
void LCD_Clear (I2C_TypeDef * I2Cx, uint8_t DeviceAddress);
void LCD_Put_Cur(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, int row, int col);

#endif
