/*
 * dht11.h
 *
 *  Created on: Jul 16, 2025
 *      Author: Administrator
 */

#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32f4xx_hal.h"

typedef struct{
	uint8_t i_humi;
	uint8_t f_humi;
	uint8_t i_temp;
	uint8_t f_temp;
	uint8_t checksum;
}DHT11_Data_t;

typedef struct{
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
}DHT11_Handle_t;

void DHT11_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t DHT11_ReadData(DHT11_Data_t* data);
void delay_us(uint32_t us);
#endif /* INC_DHT11_H_ */
