/*
 * app_camera.c
 *
 *  Created on: Feb 1, 2025
 *      Author: munir
 */

#include "app_camera.h"
#include "sirojuLIB_ov7670_SCCB.h"

#include "sirojuLIB_st7789.h"
#include "bitmap_handler.h"

extern DCMI_HandleTypeDef hdcmi;
extern I2C_HandleTypeDef hi2c2;
extern uint8_t frame_x0, frame_y0, frame_x1, frame_y1;
extern uint8_t camera_buffer[] __attribute__((aligned(32)));
extern uint8_t frame_color_mask[] __attribute__((section(".ccmram")));

void camera_init (void){
	ov7670_config (&hi2c2, CAM_RST_GPIO_Port, CAM_RST_Pin);
}

void camera_crop (uint32_t x0, uint32_t y0, uint32_t x_size, uint32_t y_size){
	HAL_DCMI_ConfigCrop(&hdcmi, x0, y0, x_size, y_size);
	HAL_DCMI_EnableCrop(&hdcmi);
}

_Bool color_filter (color_t color, uint16_t pixel) {
	_Bool result = 0;
	uint8_t r = pixel >> 11;
	uint8_t g = (pixel >> 5) & 0x3f;
	uint8_t b = pixel & 0x1f;
	r = r << 3;
	g = g << 2;
	b = b << 3;

	if (color == RED){
    	if (r > (g + 25) && r > (b + 25) && r > 50) result = 1;
	}
	else if (color == GREEN){
    	if (g > (r + 100) && g > (b + 100) && g > 50) result = 1;
	}
	else if (color == BLUE){
    	if (b > (r + 0) && b > (g + 0) && b > 10) result = 1;
	}
	return result;
}

void cam_filter_color (color_t filter){
	uint8_t x0 = 255;
	uint8_t y0 = 255;
	uint8_t x1 = 0;
	uint8_t y1 = 0;
	for (int y = 0; y < HEIGHT-3; y+=3) {
		for (int x = 0; x < WIDTH-3; x+=3) {
			int index;
			uint16_t pixel_cam[9];
			_Bool result_filter = 1;
			for (uint8_t n = 0; n < 9; n++){
				index = ((y + n/3) * WIDTH + x + (n%3)) * 2;
				pixel_cam[n] = camera_buffer[index + 1] << 8 | camera_buffer[index];
				if (color_filter (filter, pixel_cam[n])){
					frame_color_mask[index/16] |= (1 << ((index/2) % 8));
				}
				else{
					frame_color_mask[index/16] &= ~(1 << ((index/2) % 8));
					result_filter = 0;
				}
//				result_filter = result_filter && color_filter (filter, pixel_cam[n]);

			}
			if (result_filter){
				if (x0 > x) x0 = x;
				if (y0 > y) y0 = y;
				if (x1 < x) x1 = x;
				if (y1 < y) y1 = y;

			}
		}
	}
	frame_x0 = x0;
	frame_y0 = y0;
	frame_x1 = x1;
	frame_y1 = y1;
}

void open_cam (void){
	CAMERA_ENABLE();
	ST7789_SetCursorPosition (0, 0, LCD_WIDTH-1, LCD_HEIGHT-1);
	CAMERA_START((uint32_t)&camera_buffer[0], LCD_WIDTH/2);
	LED_BUILTIN_GPIO_Port->BSRR = LED_BUILTIN_Pin<<16;
}

void close_cam (void){
	CAMERA_STOP();
	LED_BUILTIN_GPIO_Port->BSRR = LED_BUILTIN_Pin<<16;
	CAMERA_DISABLE();
}

void cam_take_picture (void){
	CAMERA_STOP();
	int ret = save_lcd_to_sd (LCD_WIDTH, LCD_HEIGHT);
	if (ret == 1){
		LED_BUILTIN_GPIO_Port->BSRR = LED_BUILTIN_Pin;
	}
	else{
		LED_BUILTIN_GPIO_Port->BSRR = LED_BUILTIN_Pin<<16;
	}
	CAMERA_DISABLE();
}

#ifdef USB_CAMERA_STREAM

uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);

void SendImageUSB(uint8_t *data, uint32_t length) {
    uint32_t header = 0xA5A5A5A5;

    // Kirim header
    while (CDC_Transmit_FS((uint8_t *)&header, sizeof(header)) == USBD_BUSY);

    // Kirim data gambar
    while (length > 0) {
        uint32_t chunkSize = (length > 128) ? 128 : length; // Maksimum paket USB FS
        while (CDC_Transmit_FS(data, chunkSize) == USBD_BUSY); // Tunggu hingga endpoint USB siap
        data += chunkSize;
        length -= chunkSize;
    }
}

void send_image_with_frame() {
    uint32_t header = 0xA5A5A5A5;  // Header untuk sinkronisasi frame
    uint32_t offset = 0;

    // Kirim header
    while (CDC_Transmit_FS((uint8_t *)&header, sizeof(header)) == USBD_BUSY);

    for (int y = 0; y < HEIGHT; y++) {
    	int index;
    	if (y > frame_y0 && y < frame_y1){
            index = (y * WIDTH + frame_x0) * 2;  // Hitung indeks piksel
            camera_buffer[index + 1] = 0xF8; // Warna merah RGB565
            camera_buffer[index] = 0x00;
            index = (y * WIDTH + frame_x1) * 2;  // Hitung indeks piksel
            camera_buffer[index + 1] = 0xF8; // Warna merah RGB565
            camera_buffer[index] = 0x00;
    	}
    	else if (y == frame_y0 || y == frame_y1){
    		for (int x = frame_x0; x <= frame_x1; x++){
                index = (frame_y0 * WIDTH + x) * 2;  // Hitung indeks piksel
                camera_buffer[index + 1] = 0xF8; // Warna merah RGB565
                camera_buffer[index] = 0x00;
                index = (frame_y1 * WIDTH + x) * 2;  // Hitung indeks piksel
                camera_buffer[index + 1] = 0xF8; // Warna merah RGB565
                camera_buffer[index] = 0x00;
    		}
    	}
//        for (int x = 0; x < WIDTH; x++) {
//            if (((x == frame_x0 || x == frame_x1) && y >= frame_y0 && y <= frame_y1) ||
//                ((y == frame_y0 || y == frame_y1) && x >= frame_x0 && x <= frame_x1)) {
//                int index = (y * WIDTH + x) * 2;  // Hitung indeks piksel
//                camera_buffer[index + 1] = 0xF8; // Warna merah RGB565
//                camera_buffer[index] = 0x00;
//            }
//        }

        uint32_t length = WIDTH *2;
        while (length > 0){
        	uint32_t chunk_size = (length > 128) ? 128 : length;
            while (CDC_Transmit_FS(camera_buffer+offset, chunk_size) == USBD_BUSY);
            offset += chunk_size;
            length -= chunk_size;
        }
    }
}
#endif
