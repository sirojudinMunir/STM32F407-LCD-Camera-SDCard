/*
 * app_lcd.c
 *
 *  Created on: Feb 2, 2025
 *      Author: munir
 */

#include "app_lcd.h"
#include "main.h"
#include "sirojuLIB_st7789.h"

extern TIM_HandleTypeDef htim14;

void lcd_init (void){
//	HAL_TIM_Base_Start_IT(&htim14);
//	HAL_TIM_OC_Start_IT(&htim14, TIM_CHANNEL_1);

	  LCD_BL_GPIO_Port->BSRR = LCD_BL_Pin;

	HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
	ST7789_Init(240, 240);
}

void lcd_set_backlight (float brighness){
	TIM14->CCR1 = brighness * 2.55;
}
