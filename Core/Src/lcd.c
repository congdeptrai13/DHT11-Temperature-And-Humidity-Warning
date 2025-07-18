/*
 * lcd.c
 *
 *  Created on: Jul 17, 2025
 *      Author: Administrator
 */
#include "lcd.h"

void GPIO_LCD_Init(void);
void LCD_Send_Nibble(uint8_t nibble);
void LCD_Enable(void);
/*
 * RS => D2
 * RW => D3
 * D4 => D5
 * D5 => D6
 * D6 => D7
 * D7 => D0
 * EN => D4
 */

void LCD_Clear(void){
	LCD_Send_CMD(LCD_CMD_DISPLAY_CLEAR); //0x00000001
	delay_us(2000);
}
void LCD_Init(){
	GPIO_LCD_Init();

	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	HAL_Delay(40);

	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	LCD_Send_Nibble(0x3);

	HAL_Delay(5);

	LCD_Send_Nibble(0x3);

	delay_us(150);

	LCD_Send_Nibble(0x3);

	LCD_Send_Nibble(LCD_RETURN_HOME);

	//set to 4 bit
	LCD_Send_CMD(LCD_CMD_4_BIT_2_LINE_5X8); //0x00101000

	//	Display on and cursor on
	LCD_Send_CMD(LCD_CMD_DISPLAY_ON_CURSOR_ON); // 0x00001000


	// clear Display
	LCD_Clear();
	// Entry mode set
	LCD_Send_CMD(LCD_CMD_INCADD); //0x00000110
}


void GPIO_LCD_Init(){
	if (LCD_GPIO_PORT == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
	else if (LCD_GPIO_PORT == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
	else if (LCD_GPIO_PORT == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
	else if (LCD_GPIO_PORT == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = LCD_GPIO_RS| LCD_GPIO_RW | LCD_GPIO_D4 | LCD_GPIO_D5 | LCD_GPIO_D6 | LCD_GPIO_D7 | LCD_GPIO_EN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void LCD_Send_Nibble(uint8_t nibble){
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D4, ((nibble >> 0) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D5, ((nibble >> 1) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D6, ((nibble >> 2) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_D7, ((nibble >> 3) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	// enable pin
	LCD_Enable();
}
void LCD_Send_CMD(uint8_t cmd){
    // RS = 0 (command), RW = 0 (write)
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);

	LCD_Send_Nibble((cmd >> 4) & 0x0F);
	LCD_Send_Nibble(cmd & 0x0F);
}

void LCD_Send_Data(uint8_t data){
	 // RS = 1 (data), RW = 0 (write)
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_RW, GPIO_PIN_RESET);
	LCD_Send_Nibble((data >> 4) & 0x0F);
	LCD_Send_Nibble(data & 0x0F);
}

void LCD_Send_String(uint8_t *str){
	while(*str != '\0'){
		LCD_Send_Data(*str);
		str++;
	}
}

void LCD_Enable(void){
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_SET);
	delay_us(1);
	HAL_GPIO_WritePin(LCD_GPIO_PORT, LCD_GPIO_EN, GPIO_PIN_RESET);
	delay_us(50);
}
