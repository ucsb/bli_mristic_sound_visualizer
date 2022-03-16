#include "I2C.h"
#include "SysTimer.h"
#include <stdio.h>
extern void Error_Handler(void);

// Inter-integrated Circuit Interface (I2C)
// up to 100 Kbit/s in the standard mode, 
// up to 400 Kbit/s in the fast mode, and 
// up to 3.4 Mbit/s in the high-speed mode.

// Recommended external pull-up resistance is 
// 4.7 kOmh for low speed, 
// 3.0 kOmh for the standard mode, and 
// 1.0 kOmh for the fast mode
	
//===============================================================================
//                        I2C GPIO Initialization
//===============================================================================
void I2C1_GPIO_Init(void) {
	// [TODO]
	//enable GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	//set mode to alternate 
	GPIOB->MODER &= ~GPIO_MODER_MODE8;	
	GPIOB->MODER |= GPIO_MODER_MODE8_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE7;	
	GPIOB->MODER |= GPIO_MODER_MODE7_1;
	
	//use alternate functions for PB6, PB7 (AF4) for USART2
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8_2;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL7_2;

	//set speeds to very high
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED7;

	//set to open drain output type
	GPIOB->OTYPER |= GPIO_OTYPER_OT8;
	GPIOB->OTYPER |= GPIO_OTYPER_OT7;

	//set to pull-up register (01)
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD7_0;
}

void I2C2_GPIO_Init(void) {
	// [TODO]
	//enable GPIOB
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	//set mode to alternate 
	GPIOB->MODER &= ~GPIO_MODER_MODE10;	
	GPIOB->MODER |= GPIO_MODER_MODE10_1;
	GPIOB->MODER &= ~GPIO_MODER_MODE11;	
	GPIOB->MODER |= GPIO_MODER_MODE11_1;
	
	//use alternate functions for PB10, PB11 (AF4) for USART2
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL10_2;
	GPIOB->AFR[1] |= GPIO_AFRH_AFSEL11_2;

	//set speeds to very high
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED10 | GPIO_OSPEEDR_OSPEED11;

	//set to open drain output type
	GPIOB->OTYPER |= GPIO_OTYPER_OT10;
	GPIOB->OTYPER |= GPIO_OTYPER_OT11;

	//set to pull-up register (01)
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD10_0 | GPIO_PUPDR_PUPD11_0;
}
	
#define I2C_TIMINGR_PRESC_POS	28
#define I2C_TIMINGR_SCLDEL_POS	20
#define I2C_TIMINGR_SDADEL_POS	16
#define I2C_TIMINGR_SCLH_POS	8
#define I2C_TIMINGR_SCLL_POS	0

//===============================================================================
//                          I2C Initialization
//===============================================================================
void I2C1_Initialization(void){
	uint32_t OwnAddr = 0x50;
	
	// [TODO]

	//enable I2C1 clk
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;

	//select system clk as I2C1 clk source (01)
	RCC->CCIPR |= RCC_CCIPR_I2C1SEL_0;

	//reset I2C1
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C1RST;

	//re-enable I2C1
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C1RST;

	//turn off I2C1
	I2C1->CR1 &= ~I2C_CR1_PE;

	//enable analog noise  filter
	I2C1->CR1 &= ~I2C_CR1_ANFOFF;

	//disable digital noise filter
	I2C1->CR1 &= ~I2C_CR1_DNF;

	//enable error interrupts
	I2C1->CR1 |= I2C_CR1_ERRIE;

	//enable clock stretching
	I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;

	//Set the master to operate in 7-bit addressing mode
	I2C1->CR2 &= ~I2C_CR2_ADD10;

	//Enable automatic end mode and NACK generation.
	I2C1->CR2 |= I2C_CR2_AUTOEND | I2C_CR2_NACK;

	//clear timing
	I2C1->TIMINGR &= ~I2C_TIMINGR_PRESC;
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLDEL;
	I2C1->TIMINGR &= ~I2C_TIMINGR_SDADEL;
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLH;
	I2C1->TIMINGR &= ~I2C_TIMINGR_SCLL;

	//set timing
	I2C1->TIMINGR |= (7 << I2C_TIMINGR_PRESC_POS);   //PRESC set to 7, period tPRESC is 0.1us
	I2C1->TIMINGR |= (15 << I2C_TIMINGR_SCLDEL_POS);  //SCLDEL set to 10, originally 
	I2C1->TIMINGR |= (15 << I2C_TIMINGR_SDADEL_POS);  //SDADEL set to 12, originally  
	I2C1->TIMINGR |= (5 << I2C_TIMINGR_SCLH_POS);    //SCLH set to 39  , originally   
	I2C1->TIMINGR |= (5 << I2C_TIMINGR_SCLL_POS);    //SCLL set to 46  , originally    
	// 15
	// 15
	// 15
	// 5?
	// 5?
	//set own addr
	I2C1->OAR1 &= ~(I2C_OAR1_OA1EN);
	I2C1->OAR2 &= ~(I2C_OAR2_OA2EN);
	I2C1->OAR1 &= ~(I2C_OAR1_OA1MODE);
	I2C1->OAR1 &= ~(I2C_OAR1_OA1);
	I2C1->OAR1 |= OwnAddr << 1;
	I2C1->OAR1 |= I2C_OAR1_OA1EN;

	//enable I2C1
	I2C1->CR1 |= I2C_CR1_PE;
}

void I2C2_Initialization(void){
	uint32_t OwnAddr = 0x50;
	
	// [TODO]

	//enable I2C2 clk
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN;

	//select system clk as I2C2 clk source (01)
	RCC->CCIPR |= RCC_CCIPR_I2C2SEL_0;

	//reset I2C1
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_I2C2RST;

	//re-enable I2C1
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_I2C2RST;

	//turn off I2C1
	I2C2->CR1 &= ~I2C_CR1_PE;

	//enable analog noise filter
	I2C2->CR1 &= ~I2C_CR1_ANFOFF;

	//disable digital noise filter
	I2C2->CR1 &= ~I2C_CR1_DNF;

	//enable error interrupts
	I2C2->CR1 |= I2C_CR1_ERRIE;

	//enable clock stretching
	I2C2->CR1 &= ~I2C_CR1_NOSTRETCH;

	//Set the master to operate in 7-bit addressing mode
	I2C2->CR2 &= ~I2C_CR2_ADD10;

	//Enable automatic end mode and NACK generation.
	I2C2->CR2 |= I2C_CR2_AUTOEND | I2C_CR2_NACK;

	//clear timing
	I2C2->TIMINGR &= ~I2C_TIMINGR_PRESC;
	I2C2->TIMINGR &= ~I2C_TIMINGR_SCLDEL;
	I2C2->TIMINGR &= ~I2C_TIMINGR_SDADEL;
	I2C2->TIMINGR &= ~I2C_TIMINGR_SCLH;
	I2C2->TIMINGR &= ~I2C_TIMINGR_SCLL;

	//set timing
	I2C2->TIMINGR |= (1 << I2C_TIMINGR_PRESC_POS);   //PRESC set to 7, period tPRESC is 0.1us
	I2C2->TIMINGR |= (0 << I2C_TIMINGR_SCLDEL_POS);  //SCLDEL set to 10, originally 
	I2C2->TIMINGR |= (0 << I2C_TIMINGR_SDADEL_POS);  //SDADEL set to 12, originally  
	I2C2->TIMINGR |= (10 << I2C_TIMINGR_SCLH_POS);    //SCLH set to 39  , originally   
	I2C2->TIMINGR |= (10 << I2C_TIMINGR_SCLL_POS);    //SCLL set to 46  , originally    
	// 15
	// 15
	// 15
	// 5?
	// 5?
	//set own addr
	I2C2->OAR1 &= ~(I2C_OAR1_OA1EN);
	I2C2->OAR2 &= ~(I2C_OAR2_OA2EN);
	I2C2->OAR1 &= ~(I2C_OAR1_OA1MODE);
	I2C2->OAR1 &= ~(I2C_OAR1_OA1);
	I2C2->OAR1 |= OwnAddr << 1;
	I2C2->OAR1 |= I2C_OAR1_OA1EN;

	//enable I2C1
	I2C2->CR1 |= I2C_CR1_PE;
}

//===============================================================================
//                           I2C Start
// Master generates START condition:
//    -- Slave address: 7 bits
//    -- Automatically generate a STOP condition after all bytes have been transmitted 
// Direction = 0: Master requests a write transfer
// Direction = 1: Master requests a read transfer
//=============================================================================== 
int8_t I2C_Start(I2C_TypeDef * I2Cx, uint32_t DevAddress, uint8_t Size, uint8_t Direction) {	
	
	// Direction = 0: Master requests a write transfer
	// Direction = 1: Master requests a read transfer
	
	uint32_t tmpreg = 0;
	
	// This bit is set by software, and cleared by hardware after the Start followed by the address
	// sequence is sent, by an arbitration loss, by a timeout error detection, or when PE = 0.
	tmpreg = I2Cx->CR2;
	
	tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));
	
	if (Direction == READ_FROM_SLAVE)
		tmpreg |= I2C_CR2_RD_WRN;  // Read from Slave
	else
		tmpreg &= ~I2C_CR2_RD_WRN; // Write to Slave
		
	tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES));
	
	tmpreg |= I2C_CR2_START;
	
	I2Cx->CR2 = tmpreg; 
	
   	return 0;  // Success
}

//===============================================================================
//                           I2C Stop
//=============================================================================== 
void I2C_Stop(I2C_TypeDef * I2Cx){
	// Master: Generate STOP bit after the current byte has been transferred 
	I2Cx->CR2 |= I2C_CR2_STOP;								
	// Wait until STOPF flag is reset
	while( (I2Cx->ISR & I2C_ISR_STOPF) == 0 ); 
}

//===============================================================================
//                           Wait for the bus is ready
//=============================================================================== 
void I2C_WaitLineIdle(I2C_TypeDef * I2Cx){
	// Wait until I2C bus is ready
	while( (I2Cx->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY );	// If busy, wait
}

//===============================================================================
//                           I2C Send Data
//=============================================================================== 
int8_t I2C_SendData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size) {
	int i,j;
	
	if (Size <= 0 || pData == NULL) return -1;
	
	I2C_WaitLineIdle(I2Cx);
	
	if (I2C_Start(I2Cx, DeviceAddress, Size, WRITE_TO_SLAVE) < 0 ) return -1;
	
	// Send Data
	// Write the first data in DR register
	// while((I2Cx->ISR & I2C_ISR_TXE) == 0);
	// I2Cx->TXDR = pData[0] & I2C_TXDR_TXDATA;  

	for (i = 0; i < Size; i++) {
		// TXE is set by hardware when the I2C_TXDR register is empty. It is cleared when the next
		// data to be sent is written in the I2C_TXDR register.
		// while( (I2Cx->ISR & I2C_ISR_TXE) == 0 ); 

		// TXIS bit is set by hardware when the I2C_TXDR register is empty and the data to be
		// transmitted must be written in the I2C_TXDR register. It is cleared when the next data to be
		// sent is written in the I2C_TXDR register.
		// The TXIS flag is not set when a NACK is received.
		//printf("foo%i\n", i);
		//for(j = 0; j < 800; j++);
		while((I2Cx->ISR & I2C_ISR_TXIS) == 0 );
		//printf("foo%i\n", i);
		I2Cx->TXDR = pData[i] & I2C_TXDR_TXDATA;  // TXE is cleared by writing to the TXDR register.
		//printf("foo%i\n", i);
	}
	
	// Wait until TC flag is set 
	while((I2Cx->ISR & I2C_ISR_TC) == 0 && (I2Cx->ISR & I2C_ISR_NACKF) == 0);
	
	if( (I2Cx->ISR & I2C_ISR_NACKF) != 0 ) return -1;

	I2C_Stop(I2Cx);
	return 0;
}


//===============================================================================
//                           I2C Receive Data
//=============================================================================== 
int8_t I2C_ReceiveData(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, uint8_t *pData, uint8_t Size) {
	int i;
	
	if(Size <= 0 || pData == NULL) return -1;

	I2C_WaitLineIdle(I2Cx);

	I2C_Start(I2Cx, DeviceAddress, Size, READ_FROM_SLAVE); // 0 = sending data to the slave, 1 = receiving data from the slave
						  	
	for (i = 0; i < Size; i++) {
		// Wait until RXNE flag is set 	
		while( (I2Cx->ISR & I2C_ISR_RXNE) == 0 );
		pData[i] = I2Cx->RXDR & I2C_RXDR_RXDATA;
	}
	
	// Wait until TCR flag is set 
	while((I2Cx->ISR & I2C_ISR_TC) == 0);
	
	I2C_Stop(I2Cx);
	
	return 0;
}

void LCD_Init(I2C_TypeDef * I2Cx, uint8_t DeviceAddress){
	int i;
	// 4 bit initialisation
	delay(50); // wait for >40ms
	LCD_Send_Command(I2Cx, DeviceAddress, 0x30);
	delay(5);  // wait for >4.1ms
	LCD_Send_Command(I2Cx, DeviceAddress, 0x30);
	delay(1);  // wait for >100us
	LCD_Send_Command(I2Cx, DeviceAddress, 0x30);
	delay(15); // wait for >10ms
	//HAL_Delay(10);
	LCD_Send_Command(I2Cx, DeviceAddress, 0x20);  // 4bit mode
	delay(15); // wait for >10ms
	//HAL_Delay(10);

  // dislay initialisation
	LCD_Send_Command(I2Cx, DeviceAddress, 0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	delay(1);
	//HAL_Delay(1);
	LCD_Send_Command(I2Cx, DeviceAddress, 0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	delay(1);
	//HAL_Delay(1);
	LCD_Send_Command(I2Cx, DeviceAddress, 0x01);  // clear display
	delay(2);
	//HAL_Delay(1);
	//HAL_Delay(1);
	LCD_Send_Command(I2Cx, DeviceAddress, 0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	delay(1);
	//HAL_Delay(1);
	LCD_Send_Command(I2Cx, DeviceAddress, 0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void LCD_Send_Command(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, char cmd){
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	I2C_SendData(I2Cx, DeviceAddress, (uint8_t *) data_t, 4);
	//for(int i = 0; i < 500000; i++);
}

void LCD_Send_Data(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, char data){
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	I2C_SendData(I2Cx, DeviceAddress, (uint8_t *) data_t, 4);
}

void LCD_Send_String(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, char *str)
{
	//LCD_Send_Command(I2Cx, DeviceAddress, 0x80);
	//delay(500);
	while (*str) {LCD_Send_Data (I2Cx, DeviceAddress, *str++); delay(50);}
}

void LCD_Clear (I2C_TypeDef * I2Cx, uint8_t DeviceAddress)
{
	LCD_Send_Command(I2Cx, DeviceAddress, 0x80);
	for (int i=0; i<70; i++)
	{
		//delay(1000);
		LCD_Send_Data(I2Cx, DeviceAddress, ' ');
	}
}

void LCD_Put_Cur(I2C_TypeDef * I2Cx, uint8_t DeviceAddress, int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    LCD_Send_Command(I2Cx, DeviceAddress, col);
}
//from https://controllerstech.com/i2c-lcd-in-stm32/

