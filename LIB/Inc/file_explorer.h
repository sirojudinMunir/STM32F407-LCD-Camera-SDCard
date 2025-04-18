/*
 * file_explorer.h
 *
 *  Created on: Mar 9, 2025
 *      Author: munir
 */

#ifndef INC_FILE_EXPLORER_H_
#define INC_FILE_EXPLORER_H_

#include "stm32f4xx_hal.h"

#define TEXT_ENTER_SPACE 10
#define TEXT_SPACE	6
#define TEXT_TAB_SPACE TEXT_SPACE*3
#define MAX_CHAR_IN_LINE 40 //40
#define MAX_TEXT_LINE 24 //24
#define MAX_CONTENT	100


typedef void (*CallbackFunction)(void);  // Define a function pointer type

typedef struct {
	uint16_t x, y, color, bg_color;
	char lcd_text_buffer[MAX_TEXT_LINE][MAX_CHAR_IN_LINE];
}text_Typedef;


void text_set_pos (uint16_t x, uint16_t y);
void text_set_color (uint16_t color, uint16_t bg_color);
void text_clear_all (void);
void text_update_all (void);
void lcd_print (char *buffer);

void load_sd_card (void);
void file_explorer(const char *path);

void button_handler(CallbackFunction fnc1, CallbackFunction fnc2);

void show_file (void);

#endif /* INC_FILE_EXPLORER_H_ */
