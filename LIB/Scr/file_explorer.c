/*
 * file_explorer.c
 *
 *  Created on: Mar 9, 2025
 *      Author: munir
 */

#include "file_explorer.h"
#include "stdio.h"
#include "stdlib.h"
#include "main.h"
#include "ff.h"
#include "sirojuLIB_st7789.h"
#include "bitmap_handler.h"
#include "jpeg_view.h"
#include "app_camera.h"
#include "app_lcd.h"


extern SD_HandleTypeDef hsd;

FATFS fs;         // FatFs work area

text_Typedef text_format;

_Bool button_state = 0, button_long_press_state = 0;
uint32_t button_tick;

uint16_t cursor = 0, last_cursor = 0;
char name_list[MAX_CONTENT][64];
FSIZE_t size_list[MAX_CONTENT];
uint16_t num_of_list;
_Bool load_sdcard_flag = 1;

void text_set_pos (uint16_t x, uint16_t y){
	text_format.x = x;
	text_format.y = y;
}

void text_get_pos (uint16_t *x, uint16_t *y){
	*x = text_format.x;
	*y = text_format.y;
}

void text_set_color (uint16_t color, uint16_t bg_color) {
	text_format.color = color;
	text_format.bg_color = bg_color;
}

void text_clear_all (void){
	ST7789_Fill (text_format.bg_color);
    text_set_pos (0, 0);
    memset (text_format.lcd_text_buffer, '\0', sizeof(text_format.lcd_text_buffer));
}

void text_update_all (void){
	uint16_t cur_temp = cursor;
	if (cur_temp > MAX_TEXT_LINE-1) cur_temp = MAX_TEXT_LINE-1;

    for (uint16_t y = 0, y_pos = 0; y < MAX_TEXT_LINE; y++, y_pos += TEXT_ENTER_SPACE) {
        for (uint16_t x = 0, x_pos = 0; x < MAX_CHAR_IN_LINE; x++, x_pos += TEXT_SPACE) {
        	if (y == cur_temp && num_of_list > 0) text_set_color (0x0000, 0x07e0);
        	else text_set_color (0x07e0, 0x0000);
            char c = text_format.lcd_text_buffer[y][x] ?: ' ';
            ST7789_drawChar(x_pos, y_pos, c, text_format.color, text_format.bg_color, 1);
        }
    }
}

void lcd_print(char *buffer) {
    char *ptr = buffer;  // Pointer to iterate over buffer
    while (*ptr) {
    	_Bool print_char = 0;
        if (*ptr == '\n') {
            text_format.x = 0;
            text_format.y += 1;
        }
        else if (*ptr == '\r') {
            // Ignore carriage return
        }
        else if (*ptr == '\t') {
            text_format.x += 3;
        }
        else {
        	print_char = 1;
        }

        if (text_format.x > MAX_CHAR_IN_LINE-1) {
            text_format.x = 0;
            text_format.y += 1;
        }

        if (text_format.y > MAX_TEXT_LINE) {
            text_format.y = MAX_TEXT_LINE;
//            LED_BUILTIN_GPIO_Port->BSRR = LED_BUILTIN_Pin;

            // Shift text buffer up (fast memory move)
            memmove(text_format.lcd_text_buffer,
                    text_format.lcd_text_buffer[1],
                    (MAX_TEXT_LINE-1) * MAX_CHAR_IN_LINE);

            memset(text_format.lcd_text_buffer[MAX_TEXT_LINE-1], '\0', MAX_CHAR_IN_LINE);
        }
        if (print_char){
            if (text_format.y == MAX_TEXT_LINE){
                text_format.lcd_text_buffer[MAX_TEXT_LINE-1][text_format.x] = *ptr;
            }
            else{
                text_format.lcd_text_buffer[text_format.y][text_format.x] = *ptr;
            }

            text_format.x += 1;
        }

        ptr++;  // Move to the next character
    }

    // **Optimized LCD Update**
//    text_update_all ();
}

void load_sd_card (void){
//	FATFS fs;
    HAL_SD_CardInfoTypeDef info;
    DWORD free_clusters, free_sectors;
    FATFS *pfs;
    char lcd_str_buffer[255];

	if (HAL_GPIO_ReadPin(SD_DETECT_GPIO_Port, SD_DETECT_Pin)){
		lcd_print("SD Card not detected\n");
		return;
	}
	else{
		lcd_print("SD Card detected\n");
	}

	if (f_mount(&fs, "/", 1) != FR_OK) {
		lcd_print("mount failed\n");
		return;
	}

	HAL_SD_GetCardInfo(&hsd, &info);

    // Hitung kapasitas total SD card dalam GB
    uint64_t total_size = (uint64_t)info.BlockNbr * info.BlockSize;
    double total_size_gb = (double)total_size / (1024 * 1024 * 1024);
    sprintf(lcd_str_buffer, "Total Size: %.2f GB\n", total_size_gb);
    lcd_print(lcd_str_buffer);

    // Dapatkan kapasitas yang tersedia dalam GB
    if (f_getfree("/", &free_clusters, &pfs) == FR_OK) {
        free_sectors = free_clusters * pfs->csize;
        uint64_t free_size = (uint64_t)free_sectors * 512;
        double free_size_gb = (double)free_size / (1024 * 1024 * 1024);

        sprintf(lcd_str_buffer, "Free Space: %.2f GB\n", free_size_gb);
        lcd_print(lcd_str_buffer);
    } else {
    	lcd_print("Failed to get free space\n");
    }
    text_update_all ();
}
#if 0
void file_explorer(const char *path) {
    char buffer[255];  // Buffer for printing
    FRESULT res;
//    DIR dir;
    FILINFO fno;

    res = f_opendir(&dir, path);  // Open the directory
    if (res == FR_OK) {
        while (1) {
            res = f_readdir(&dir, &fno);  // Read directory entry
            if (res != FR_OK || fno.fname[0] == 0) break;  // No more files

            // Ignore "." and ".." entries
            if (fno.fname[0] == '.' && (fno.fname[1] == 0 || fno.fname[1] == '.')) continue;

            snprintf(buffer, sizeof(buffer), "%s %s\n",
                (fno.fattrib & AM_DIR) ? "[DIR]" : "\t", fno.fname);
            lcd_print(buffer);

            // If entry is a folder, explore it recursively
            if (fno.fattrib & AM_DIR) {
                char new_path[255];
                snprintf(new_path, sizeof(new_path), "%s/%s", path, fno.fname);
                file_explorer(new_path);  // Recursive call
            }
        }
        f_closedir(&dir);
    } else {
    	lcd_print("SD Read Error!\n");
    }
}
#endif
void button_handler(CallbackFunction fnc1, CallbackFunction fnc2) {
    if (!HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin)) {
        if (!button_state) {
        	button_state = 1;
        }
    	if (HAL_GetTick() - button_tick > 1000){
    		if (!button_long_press_state){
    			button_long_press_state = 1;
        		if (fnc2) fnc2();
    		}
    	}
    	else{
    		if (button_long_press_state){
    			button_long_press_state = 0;
    		}
    	}
    }
    else {
    	if (button_state){
        	button_state = 0;
        	if (HAL_GetTick() - button_tick < 1000){
                if (fnc1) fnc1();
        	}
    	}
    	button_tick = HAL_GetTick();
    }
}

/********************************************************************************************************/

void read_sdcard(const char *path, char **list, FSIZE_t *size_list, uint16_t *num_of_list) {
    FRESULT res;
    DIR dir;
    FILINFO fno;
    uint16_t num = 0;

    res = f_opendir(&dir, path);  // Open the directory
    if (res == FR_OK) {
        while (1) {
            res = f_readdir(&dir, &fno);  // Read directory entry
            if (res != FR_OK || fno.fname[0] == 0) break;  // No more files

            // Ignore "." and ".." entries
            if (fno.fname[0] == '.' && (fno.fname[1] == 0 || fno.fname[1] == '.')) continue;

            // Ensure list[*num_of_list] is a valid pointer (should be allocated before calling this function)
            strcpy(list[num], fno.fname);
            size_list[num] = fno.fsize;
            num++;  // Correct increment
            if (num >= MAX_CONTENT){
            	num = MAX_CONTENT;
            	break;
            }
        }
        *num_of_list = num;
        f_closedir(&dir);
    } else {
        lcd_print("SD Read Error!\n");
        text_update_all ();
    }
}

uint32_t text_file_length = 0;
uint8_t file_handle = 0, last_file_handle = 0;

void test_sd_read_txt(char *path) {
    char read_data[2];  // Chunk size (adjust as needed)
    UINT bytes_read;
    FRESULT res;
	FIL fil;

    text_set_pos (0, 0);
    memset (text_format.lcd_text_buffer, '\0', sizeof(text_format.lcd_text_buffer));

    _Bool start = 0, calc_len = 0;
    uint16_t first_line_len = 0;
    // Open the file for reading
    res = f_open(&fil, path, FA_READ);
    if (res == FR_OK) {
    	res = f_lseek(&fil, text_file_length);
    	if (res != FR_OK) return;

        while (1) {
            res = f_read(&fil, read_data, 1, &bytes_read);
            if (res != FR_OK || bytes_read == 0) {
                if (!start) {
                	text_file_length = 0;
                	test_sd_read_txt(path);
                }
            	break;  // Stop if error or end of file
            }
            if (!start) start = 1;

            if (!calc_len){
            	first_line_len += bytes_read;
                if (strstr(read_data, "\n")) calc_len = 1;
                uint32_t len = strlen (text_format.lcd_text_buffer[0]);
                if (len >= MAX_CHAR_IN_LINE-1) calc_len = 1;
            }

            uint16_t x, y;
            text_get_pos (&x, &y);
            if (y == MAX_TEXT_LINE-1 && x == 0){
            	if (strstr(read_data, "\n")) break;
            }
            uint32_t len = strlen (text_format.lcd_text_buffer[MAX_TEXT_LINE-1]);
            if (len > 0){
            	if (len >= MAX_CHAR_IN_LINE) break;
            	if (strstr(read_data, "\n")) break;
            	while (len + bytes_read < MAX_CHAR_IN_LINE){
            		bytes_read--;
            	}
            	if (bytes_read == 0) break;
            }

            read_data[bytes_read] = '\0';  // Null-terminate the chunk
            lcd_print(read_data);  // Display on LCD

        }
        text_file_length += first_line_len;

        f_close(&fil);  // Close the file
    } else {
        lcd_print("File Open Error!\n");
        text_update_all ();
    }

    text_update_all ();
}

char path[32] = "0:";
int path_len;
char debug_str[64];

void show_content (){
    text_set_pos (0, 0);
    memset (text_format.lcd_text_buffer, '\0', sizeof(text_format.lcd_text_buffer));

    char fname[32];
    char content[255];
    uint16_t start = 0, end;
    end = ((num_of_list+1 > MAX_TEXT_LINE)? MAX_TEXT_LINE : num_of_list+1);
    if (cursor >= MAX_TEXT_LINE){
    	start = cursor - MAX_TEXT_LINE + 1;
    	if (start + MAX_TEXT_LINE < num_of_list+1){
    		end = MAX_TEXT_LINE + start;
    	}
    	else{
    		end = num_of_list+1;
    	}
    }

    for (uint16_t i = start; i < end; i++) {
        if (i == start) {
        	lcd_print(" ...");
        	lcd_print(path);
        	lcd_print("\n");
//        	sprintf (content," start:%d end:%d\n", start, end);
//        	lcd_print (content);
        }
        else{
        	strcpy(fname, name_list[i-1]);

        	sprintf (content, "%s", fname);
        	text_set_pos (0, i-start);
        	lcd_print (content);

        	if (strstr(name_list[i-1], ".app")){
            	sprintf (content, "[APP]");
        	}
        	else if (!strstr(name_list[i-1], ".")){
            	sprintf (content, "[DIR]");
        	}
        	else{
        		char size_str[16];
#if 0
        		if (size_list[i-1] < 1024){
        			sprintf (size_str, "%d Byte", (int)size_list[i-1]);
        		}
        		else{
        			sprintf (size_str, "%.2f KB", (float)size_list[i-1] / 1024.0);
        		}
#else
    			sprintf (size_str, "%.2f KB", (float)size_list[i-1] / 1024.0);
#endif
//            	sprintf (content, "%32s  %s\n", fname, size_str);
            	sprintf (content, "%s", size_str);
        	}
        	text_set_pos (29, i-start);
        	lcd_print (content);
        }
    }
    if (num_of_list == 0) lcd_print("This Folder is Empty\n");
    text_update_all ();
}

int next = 0;
_Bool toogle_cam = 0;
void enter(void);

void move_cursor(void) {
	if (file_handle == 0){
	    cursor++;
	    if (cursor >= num_of_list+1) cursor = 0;
		last_cursor = cursor;
	    if (num_of_list) show_content();
	}
	else if (file_handle == 1){
    	load_sdcard_flag = 1;
	}
	else if (file_handle == 2){
		//searching file
		cursor = last_cursor;
		while (1){
		    cursor++;
		    if (strlen(name_list[cursor-1]) <= 0) cursor = 1;
		    if (strstr(name_list[cursor-1], ".bmp")) break;
		}
		last_cursor = cursor;

		//delete last file path
		int ln = strlen(path);
		while (ln>2){
			_Bool stop = 0;
			if (path[ln] == '/') stop = 1;
			path[ln--] = '\0';
			if (stop) break;
		}
    	path_len = strlen(path);
    	path_len += sprintf (path + path_len, "/%s", name_list[cursor-1]);

    	text_clear_all ();
    	load_sdcard_flag = 1;
		file_handle = 0;

	}
	else if (file_handle == 100){
		cam_take_picture();
    	lcd_set_backlight (0);
    	HAL_Delay(100);
		open_cam();
    	lcd_set_backlight (50);
//		if (!toogle_cam){
//			toogle_cam = 1;
//		}
//		else{
//			toogle_cam = 0;
//		}
	}
}

void enter(void){
	if (file_handle == 2) cursor = 0;
	else if (file_handle == 100){
		close_cam();
		toogle_cam = 0;
	}
	if (sizeof(name_list[cursor-1]) > 0){
    	load_sdcard_flag = 1;
        text_set_color (0x07e0, 0x0000);
    	text_clear_all ();
    	if (cursor == 0){
    		int ln = strlen(path);
    		while (ln>2){
    			_Bool stop = 0;
    			if (path[ln] == '/') stop = 1;
    			path[ln--] = '\0';
    			if (stop) break;
    		}
    	}
    	else{
	    	path_len = strlen(path);
	    	path_len += sprintf (path + path_len, "/%s", name_list[cursor-1]);
    	}

		cursor = 0;
    	num_of_list = 0;
    	text_file_length = 0;
	}
}

void show_file (void){
	button_handler (move_cursor, enter);

	if (load_sdcard_flag){
		load_sdcard_flag = 0;

	    char *list_ptrs[MAX_CONTENT];
	    for (uint16_t i = 0; i < MAX_CONTENT; i++) {
	        list_ptrs[i] = name_list[i];
	    }
	    last_file_handle = file_handle;
	    if (!strstr(path, ".")){
	    	file_handle = 0;
	    	memset (name_list, '\0', sizeof (name_list));

		    //show cam.app
		    if (strlen(path) <= 3){
		    	strcpy(list_ptrs[0], "CAM.app");
		    	size_list[0] = 0;
			    read_sdcard(path, &list_ptrs[1], &size_list[1], &num_of_list);
			    num_of_list++;
		    }
		    else{
			    read_sdcard(path, list_ptrs, size_list, &num_of_list);
		    }

		    show_content();
	    }
	    else if (strstr(path, ".TXT") || strstr(path, ".txt")){
	    	file_handle = 1;
	    	test_sd_read_txt(path);
	    }
	    else if (strstr(path, ".STR") || strstr(path, ".str")){
	    	file_handle = 1;
	    	test_sd_read_txt(path);
	    }
	    else if (strstr(path, ".BMP") || strstr(path, ".bmp")){
	    	file_handle = 2;
	    	lcd_set_backlight (0);
	    	print_bmp_from_sdcard(0, 0, path);
	    	lcd_set_backlight (50);
	    }
	    else if (strstr(path, ".JPG") || strstr(path, ".jpg")){
	    	file_handle = 3;
	    	read_jpg(path);
	    }
	    else if (strstr(path, ".APP") || strstr(path, ".app")){
	    	if (strstr(path, "CAM")){
		    	file_handle = 100;
		    	open_cam();
	    	}
		    else{
		        lcd_print("this APP not support yet!\n");
		        text_update_all ();
		    }
	    }
	    else if (strstr(path, ".BIN") || strstr(path, ".bin")){
	    	if (f_rename(path, "firmware.bin") == FR_OK) {
		        lcd_print("UPDATING FIRMWARE...\n");
		        text_update_all ();
		        HAL_Delay(1000);
		    	NVIC_SystemReset();
	    	}
    	}
	    else{
	        lcd_print("File not support!\n");
	        text_update_all ();
	    }
	}
}


