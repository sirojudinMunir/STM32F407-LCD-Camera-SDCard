/*
 * bitmap_handler.h
 *
 *  Created on: Feb 1, 2025
 *      Author: munir
 */

#ifndef INC_BITMAP_HANDLER_H_
#define INC_BITMAP_HANDLER_H_

#include <stdint.h>

// BMP Header Structures
#pragma pack(push, 1)  // Ensure no padding between struct members
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMPFileHeader;

typedef struct {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BMPInfoHeader;
#pragma pack(pop)

int save_lcd_to_sd(uint32_t width, uint32_t height);
//void convert_rgb565_to_rgb888(uint16_t *rgb565_buffer, uint8_t *rgb888_buffer, uint32_t pixel_count);
uint16_t convertRGB888toRGB565(uint8_t r, uint8_t g, uint8_t b);
void print_bmp_from_sdcard(uint16_t x_pos, uint16_t y_pos, char *file_name);

#endif /* INC_BITMAP_HANDLER_H_ */
