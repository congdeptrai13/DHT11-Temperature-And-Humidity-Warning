/*
 * dht11.c
 *
 *  Created on: Jul 16, 2025
 *      Author: Administrator
 */

#include "dht11.h"
#include "core_cm4.h" // hoặc core_cm7.h nếu STM32F7
uint8_t DHT11_ReadByte_Interval(void);
static DHT11_Handle_t dht11;
void GPIO_Output_Init();
void GPIO_Input_Init();

void DWT_Init(void) {
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;     // Bật DWT
    DWT->CYCCNT = 0;                                     // Reset bộ đếm
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;                 // Enable bộ đếm
}
void delay_us(uint32_t us) {
    uint32_t clk_cycle_start = DWT->CYCCNT;
    // STM32F4 chạy 168MHz => 1 us = 168 cycles
    uint32_t delay_cycles = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - clk_cycle_start) < delay_cycles);
}

void DHT11_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	dht11.GPIO_Pin = GPIO_Pin;
	dht11.GPIOx = GPIOx;
	DWT_Init();
}

void DHT11_Start(){
	GPIO_Output_Init();
	HAL_GPIO_WritePin(dht11.GPIOx, dht11.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(dht11.GPIOx, dht11.GPIO_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(dht11.GPIOx, dht11.GPIO_Pin, GPIO_PIN_SET);
	GPIO_Input_Init();
	while(HAL_GPIO_ReadPin(dht11.GPIOx, dht11.GPIO_Pin));
	while(!HAL_GPIO_ReadPin(dht11.GPIOx, dht11.GPIO_Pin));
	while(HAL_GPIO_ReadPin(dht11.GPIOx, dht11.GPIO_Pin));
}

uint8_t DHT11_ReadBit(void){
	uint8_t result = 0;
	while(!HAL_GPIO_ReadPin(dht11.GPIOx, dht11.GPIO_Pin));
	delay_us(50);
	if(HAL_GPIO_ReadPin(dht11.GPIOx, dht11.GPIO_Pin)){
		result = 1;
	}
	while(HAL_GPIO_ReadPin(dht11.GPIOx, dht11.GPIO_Pin)){
    	// đợi cho high kết thúc
    }
	return result;
}

uint8_t DHT11_ReadData(DHT11_Data_t* data){
	DHT11_Start();
	data->i_humi = DHT11_ReadByte_Interval();
	data->f_humi = DHT11_ReadByte_Interval();
	data->i_temp = DHT11_ReadByte_Interval();
	data->f_temp = DHT11_ReadByte_Interval();
	data->checksum = DHT11_ReadByte_Interval();
	//check sum but optional
	uint8_t sum = data->i_humi + data->f_humi + data->i_temp + data->f_temp;
	if (sum == data->checksum){
		return 1;
	} else {
		return 0;
	}
}

uint8_t DHT11_ReadByte_Interval(void){
	uint8_t value = 0;
	for(uint8_t i = 0; i < 8;i++){
		value <<= 1;
		value |= DHT11_ReadBit();
	}
	return value;
}

void GPIO_Output_Init()
{
	if (dht11.GPIOx == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
	else if (dht11.GPIOx == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
	else if (dht11.GPIOx == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
	else if (dht11.GPIOx == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = dht11.GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(dht11.GPIOx, &GPIO_InitStruct);
}

void GPIO_Input_Init()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /*Configure GPIO pin : PD0 */
  GPIO_InitStruct.Pin = dht11.GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(dht11.GPIOx, &GPIO_InitStruct);
}
