/*
 * lcd.h
 *
 *  Created on: Jul 17, 2025
 *      Author: Administrator
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f4xx_hal.h"
#include "dht11.h"

/*
 * RS => D2
 * RW => D3
 * D4 => D5
 * D5 => D6
 * D6 => D7
 * D7 => D0
 * EN => D4
 */

#define LCD_GPIO_PORT	GPIOD
#define LCD_GPIO_RS		GPIO_PIN_2
#define LCD_GPIO_RW		GPIO_PIN_3
#define LCD_GPIO_D4		GPIO_PIN_5
#define LCD_GPIO_D5		GPIO_PIN_6
#define LCD_GPIO_D6		GPIO_PIN_7
#define LCD_GPIO_D7		GPIO_PIN_0
#define LCD_GPIO_EN		GPIO_PIN_4

#define LCD_CMD_DISPLAY_CLEAR 		0x01
#define LCD_CMD_DISPLAY_ON_CURSOR_ON	0x0E
#define LCD_CMD_4_BIT_2_LINE_5X8	0x28
#define LCD_RETURN_HOME				0x02
#define LCD_CMD_INCADD       		0x06



void LCD_Init(void);
void LCD_Send_CMD(uint8_t cmd);
void LCD_Send_String(uint8_t *str);
void LCD_Clear(void);
#endif /* INC_LCD_H_ */
