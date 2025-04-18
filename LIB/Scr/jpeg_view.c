
#include "jpeg_view.h"
#include "sirojuLIB_st7789.h"
#include "file_explorer.h"

struct jpeg_decompress_struct cinfo;
typedef struct RGB
{
  uint8_t B;
  uint8_t G;
  uint8_t R;
}RGB_typedef;
struct jpeg_error_mgr jerr;

RGB_typedef *RGB_matrix;

uint16_t RGB16PixelColor;
static uint8_t *rowBuff;

uint8_t jpeg_decode(JFILE *file, uint8_t *rowBuff, int posx, int posy, UINT *iw, UINT *ih)
{
    uint32_t line_counter = 0;
    uint32_t i = 0, xc = 0;
    uint8_t offset = 1;
    JSAMPROW buffer[1] = { rowBuff };
    UINT lcdWidth = ST7789_WIDTH, lcdHeight = ST7789_HEIGHT;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file);
    jpeg_read_header(&cinfo, TRUE);

    if (cinfo.image_width > 6000) return 0;

    float scale_x = (float)cinfo.image_width / lcdWidth;
    float scale_y = (float)cinfo.image_height / lcdHeight;
    float scale = (scale_x > scale_y) ? scale_x : scale_y;

    cinfo.scale_num = 1;
    cinfo.scale_denom = (scale <= 8) ? (1 << (31 - __builtin_clz((int)scale))) : 8;

    cinfo.dct_method = JDCT_IFAST;
    jpeg_start_decompress(&cinfo);

    if (cinfo.output_width > lcdWidth)
    {
    	offset = cinfo.output_width / lcdWidth;
    	if (cinfo.output_width % lcdWidth > lcdWidth/4) offset++;
    }

//    if (posx <0 || posy < 0)
//    {
//    	posx=(lcdWidth-(cinfo.output_width*(offset-1)/offset))/2;
//    	posy=(lcdHeight-(cinfo.output_height*(offset-1)/offset))/2;
//    }
//    offset = (cinfo.output_width > lcdWidth) ? (cinfo.output_width + lcdWidth - 1) / lcdWidth : 1;

//    if (posx < 0 || posy < 0) {
//        posx = (lcdWidth - cinfo.output_width / offset) / 2;
//        posy = (lcdHeight - cinfo.output_height / offset) / 2;
//    }

//    // Hitung dimensi gambar setelah scaling
//    uint16_t output_width = cinfo.image_width / scale;
//    uint16_t output_height = cinfo.image_height / scale;
//
//    // Hitung posisi tengah di layar
//    posx = (lcdWidth - output_width) / 2;
//    posy = (lcdHeight - output_height) / 2;

    *iw = cinfo.image_width;
    *ih = cinfo.image_height;

    ST7789_Fill(0x0000);
    uint16_t lcd_buffer_jpg[ST7789_WIDTH];

    while (cinfo.output_scanline < cinfo.output_height && line_counter < lcdHeight - posy) {
    	(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		RGB_matrix=(RGB_typedef*)buffer[0];

        uint16_t *ptr = lcd_buffer_jpg;
        for (i = 0, xc = 0; i < cinfo.output_width && xc < lcdWidth; i += offset, xc++) {
            *ptr++ = (uint16_t)((RGB_matrix[i].R & 0xF8) >> 3 | (RGB_matrix[i].G & 0xFC) << 3 | (RGB_matrix[i].B & 0xF8) << 8);
        }

		for(i=0; i < offset-1 && cinfo.output_scanline < cinfo.output_height; i++)
    	(void) jpeg_read_scanlines(&cinfo, buffer, 1);

        ST7789_printImage(posx, line_counter + posy, xc, 1, (uint8_t*)lcd_buffer_jpg, _LSB_FIRST);
		line_counter++;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 1;
}

void read_jpg (char *fn){
    FIL fil;

    static uint8_t *rowBuff;
  	rowBuff = JMALLOC(2048);
    if(f_open(&fil, fn, FA_READ) == FR_OK){
		jpeg_decode(&fil, rowBuff, 0, 0, NULL, NULL);
    	f_close(&fil);
    }
    else{
    	lcd_print ("File open Error");
        text_update_all ();
    }
    JFREE(rowBuff);
}

void jpeg_screen_view(char* fn, int px, int py, UINT *iw, UINT *ih)
{
  FIL file;

//  uint32_t oldBaudRate;

//  char sf[256];



  	rowBuff = JMALLOC(2048);

//  sprintf(sf, "%s%s", path, fn);
//  if (f_mount(&fs, "", 0) != FR_OK) {
//	  JFREE(rowBuff);
//	  return;
//  }
  if(f_open(&file, fn, FA_READ) == FR_OK)
  {
    jpeg_decode(&file,rowBuff,px,py, iw, ih);
    f_close(&file);
  }
  else
  {
	  ST7789_printText("File open Error", 5, 0, 0x0000, 0xffff, 1);
//	  sprintf(sf, "%s\nFile open Error!!", sf);
//	  ILI9341_WriteString(0, 0, "File open Error", Font_7x10, ILI9341_RED, ILI9341_WHITE);
  }
//  f_mount(&fs, "", 0);
  JFREE(rowBuff);

}
