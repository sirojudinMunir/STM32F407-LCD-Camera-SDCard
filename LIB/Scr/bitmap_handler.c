/*
 * bitmap_handler.c
 *
 *  Created on: Feb 1, 2025
 *      Author: munir
 */

#include "bitmap_handler.h"
#include "ff.h"   // Library FatFs untuk akses SD Card
#include <string.h>
#include "sirojuLIB_st7789.h"
#include "file_explorer.h"

// Ukuran gambar
#define MAX_WIDTH  240
#define MAX_HEIGHT 240


uint16_t convertRGB888toRGB565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Function to convert RGB565 to RGB888
void convert_rgb565_to_rgb888(uint16_t *rgb565, uint8_t *rgb888, uint32_t width) {
    for (uint32_t i = 0; i < width; i++) {
        uint16_t pixel = rgb565[i];

        // Konversi RGB565 ke RGB888 (endianness diperhatikan)
        uint8_t r = (pixel >> 11) & 0x1F;  // Ambil 5 bit merah
        uint8_t g = (pixel >> 5) & 0x3F;   // Ambil 6 bit hijau
        uint8_t b = (pixel >> 0) & 0x1F;   // Ambil 5 bit biru

        // Ubah ke 8-bit masing-masing warna
        rgb888[i * 3 + 0] = (r << 3) | (r >> 2);  // Red
        rgb888[i * 3 + 1] = (g << 2) | (g >> 4);  // Green
        rgb888[i * 3 + 2] = (b << 3) | (b >> 2);  // Blue
    }
}


int save_lcd_to_sd(uint32_t width, uint32_t height) {
    UINT bw;
    FRESULT res;
	FIL fil;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    uint32_t row_size = (width * 3 + 3) & ~3;  // Align row to 4 bytes
    uint32_t img_size = row_size * height;
    uint32_t file_size = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + img_size;

    // Buffers
    uint16_t rgb565_buff[240];  // Static buffer untuk LCD data
    uint8_t rgb888_buff[240*3];  // Static buffer untuk BMP data

    // Fill BMP file header
    fileHeader.bfType = 0x4D42;  // 'BM'
    fileHeader.bfSize = file_size;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;
    fileHeader.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    // Fill BMP info header
    infoHeader.biSize = sizeof(BMPInfoHeader);
    infoHeader.biWidth = width;
    infoHeader.biHeight = -height;  // Negative for top-down BMP
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = img_size;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

//    // Open BMP file on SD card
//    res = f_open(&fil, "image.bmp", FA_WRITE | FA_CREATE_ALWAYS);
//    if (res != FR_OK) {
//    	lcd_print ("failed to create a file\n");
//        text_update_all ();
//        return 0;  // Failed to open file
//    }

    char filename[32];
    uint8_t file_index = 0;
    // Buat folder jika belum ada
    res = f_mkdir("DCMI");
    if (res != FR_OK && res != FR_EXIST) {
        return 0;  // Gagal membuat folder
    }

    // Cari nama file yang belum ada
    do {
        if (file_index == 0) {
            sprintf(filename, "DCMI/image.bmp");
        } else {
            sprintf(filename, "DCMI/image%d.bmp", file_index);
        }
        file_index++;
        res = f_stat(filename, NULL);
    } while (res == FR_OK);  // Loop sampai menemukan nama file yang belum ada

    // Open file untuk menulis
    res = f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (res != FR_OK) {
        return 0;
    }

    // Write BMP headers
    f_write(&fil, &fileHeader, sizeof(BMPFileHeader), &bw);
    if (bw != sizeof(BMPFileHeader)){
        f_close(&fil);
    	lcd_print ("write file header failed\n");
        text_update_all ();
        return 0;
    }
    f_write(&fil, &infoHeader, sizeof(BMPInfoHeader), &bw);
    if (bw != sizeof(BMPInfoHeader)){
        f_close(&fil);
    	lcd_print ("write info header failed\n");
        text_update_all ();
        return 0;
    }
    ST7789_SetCursorPositionRead(0, 0, width-1, height-1);
    // Write pixel data (top-down BMP format)
    for (int32_t y = 0; y < height; y++) {
        // Read one row of pixels from LCD
        for (uint32_t x = 0; x < width; x++) {
            rgb565_buff[x] = ST7789_ReadPixel();  // Read RGB565 from LCD
        }

        // Convert RGB565 to RGB888
        convert_rgb565_to_rgb888(rgb565_buff, rgb888_buff, width);

        // Zero out padding bytes (if needed)
        uint32_t padding = row_size - (width * 3);
        for (uint32_t p = 0; p < padding; p++) {
            rgb888_buff[width * 3 + p] = 0;
        }

        // Write row to file
        f_write(&fil, rgb888_buff, row_size, &bw);
        if (bw != row_size){
            f_close(&fil);
        	lcd_print ("write pixel data failed\n");
            text_update_all ();
            return 0;
        }
        f_sync(&fil);
    }

    // Close file
    f_close(&fil);
    return 1;
}

#if 0
void print_bmp_from_sdcard(uint16_t x_pos, uint16_t y_pos, char *file_name) {
    FIL file;
    UINT bytesRead;
    uint8_t buffer[MAX_WIDTH * 2];
    uint16_t rgb565;
    FRESULT res;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    res = f_open(&file, file_name, FA_READ);
    if (res == FR_OK) {
        f_read(&file, &fileHeader, sizeof(BMPFileHeader), &bytesRead);
        f_read(&file, &infoHeader, sizeof(BMPInfoHeader), &bytesRead);
        uint32_t dataOffset = fileHeader.bfOffBits;  // Get pixel data offset
        f_lseek(&file, dataOffset);  // Move to pixel data

        int32_t row_stride = (infoHeader.biWidth * 3 + 3) & ~3;  // BMP row size is aligned to 4 bytes
        uint8_t row_buffer[row_stride];

        _Bool order = 1;

        if (infoHeader.biHeight < 0) {
        	infoHeader.biHeight = -infoHeader.biHeight;
        	order = 0;
        }

        if (infoHeader.biWidth > MAX_WIDTH) infoHeader.biWidth = MAX_WIDTH;
        if (infoHeader.biHeight > MAX_HEIGHT) infoHeader.biHeight = MAX_HEIGHT;


        for (int y = 0; y < infoHeader.biHeight; y++) {
            int index = 0;
            f_read(&file, row_buffer, row_stride, &bytesRead); // Read one row
            for (int x = 0; x < infoHeader.biWidth; x++) {
                int pixel_offset = x * 3;
                rgb565 = convertRGB888toRGB565(row_buffer[pixel_offset + 2], row_buffer[pixel_offset + 1], row_buffer[pixel_offset]);
                buffer[index++] = rgb565 & 0xff;       // LSB first
                buffer[index++] = (rgb565 >> 8) & 0xff; // MSB second
            }
            ST7789_printImage(x_pos, ((order)? infoHeader.biHeight-y-1 : y) + y_pos, infoHeader.biWidth, 1, buffer, _LSB_FIRST);
        }
        f_close(&file);
    }
}
#else

#define LCD_WIDTH  240
#define LCD_HEIGHT 240
#define BACKGROUND_COLOR 0x0000  // Black color in RGB565

void print_bmp_from_sdcard(uint16_t x_pos, uint16_t y_pos, char *file_name) {
    FIL file;
    UINT bytesRead;
    uint8_t buffer[LCD_WIDTH * 2];
    uint16_t rgb565;
    FRESULT res;

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    res = f_open(&file, file_name, FA_READ);
    if (res == FR_OK) {
        f_read(&file, &fileHeader, sizeof(BMPFileHeader), &bytesRead);
        f_read(&file, &infoHeader, sizeof(BMPInfoHeader), &bytesRead);

        // Validate BMP file
        if (fileHeader.bfType != 0x4D42) {
            f_close(&file);
            return; // Not a BMP file
        }
        if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 16 && infoHeader.biBitCount != 32) {
            f_close(&file);
            return; // Unsupported BMP format (must be 24-bit, 16-bit, or 32-bit)
        }
        if (infoHeader.biCompression != 0) {
            f_close(&file);
            return; // Compressed BMPs are not supported
        }

        uint32_t dataOffset = fileHeader.bfOffBits;  // Get pixel data offset
        f_lseek(&file, dataOffset);  // Move to pixel data

        int32_t bmp_width = infoHeader.biWidth;
        int32_t bmp_height = infoHeader.biHeight;
        _Bool order = 1;

        if (bmp_height < 0) {
            bmp_height = -bmp_height;
            order = 0;
        }

        float x_ratio = (bmp_width > LCD_WIDTH) ? (float)bmp_width / LCD_WIDTH : 1;
        float y_ratio = (bmp_height > LCD_HEIGHT) ? (float)bmp_height / LCD_HEIGHT : 1;
        float scale_factor = (x_ratio > y_ratio) ? x_ratio : y_ratio;
        int scaled_width = bmp_width / scale_factor;
        int scaled_height = bmp_height / scale_factor;

        int32_t row_stride = (bmp_width * (infoHeader.biBitCount / 8) + 3) & ~3;  // BMP row size aligned to 4 bytes
        uint8_t row_buffer[row_stride];

        // Clear the screen with background color before drawing the image

        x_pos = (LCD_WIDTH - scaled_width) / 2;
        y_pos = (LCD_HEIGHT - scaled_height) / 2;
        for (int y = 0; y < scaled_height; y++) {
            int bmp_y = (int)(y * scale_factor);
            if (bmp_y >= bmp_height) bmp_y = bmp_height - 1; // Prevent overflow
            f_lseek(&file, dataOffset + (order ? (bmp_height - bmp_y - 1) : bmp_y) * row_stride);
            f_read(&file, row_buffer, row_stride, &bytesRead);
            int index = 0;

            for (int x = 0; x < scaled_width; x++) {
                int bmp_x = (int)(x * scale_factor);
                if (bmp_x >= bmp_width) bmp_x = bmp_width - 1; // Prevent overflow
                int pixel_offset = bmp_x * (infoHeader.biBitCount / 8);

                if (infoHeader.biBitCount == 32) {
                    rgb565 = convertRGB888toRGB565(row_buffer[pixel_offset + 2], row_buffer[pixel_offset + 1], row_buffer[pixel_offset]);
                    // Ignore the alpha channel (row_buffer[pixel_offset + 3])
                } else if (infoHeader.biBitCount == 24) {
                    rgb565 = convertRGB888toRGB565(row_buffer[pixel_offset + 2], row_buffer[pixel_offset + 1], row_buffer[pixel_offset]);
                } else { // 16-bit BMP
                    rgb565 = row_buffer[pixel_offset] | (row_buffer[pixel_offset + 1] << 8);
                }

                buffer[index++] = rgb565 & 0xff;       // LSB first
                buffer[index++] = (rgb565 >> 8) & 0xff; // MSB second
            }
            ST7789_printImage(x_pos, y + y_pos, scaled_width, 1, buffer, _LSB_FIRST);
        }
        f_close(&file);
    }
}



#endif

