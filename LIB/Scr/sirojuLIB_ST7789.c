
#include "sirojuLIB_st7789.h"

#define swap_2bit(x) ((x & 0xfc) | ((x << 1) & 2) | ((x >> 1) & 1))

uint32_t inc, ST7789_xSize, ST7789_ySize, ST7789_pixel_count;


extern DMA_HandleTypeDef hdma_memtomem_dma2_stream2;
//extern DMA_HandleTypeDef hdma_memtomem_dma2_stream4;


uint8_t ST7789_ReadData ()
{
	uint8_t ram, temp;
	uint8_t *ptr = (uint8_t*)FSMC_ADDR;
	temp = *ptr;
//	temp = ST7789_DATA;
	ram = swap_2bit(temp);
	return ram;
}

void ST7789_SendCmd(uint8_t x) {
    ST7789_CMD = swap_2bit(x);
}

void ST7789_SendData(uint8_t x) {
    ST7789_DATA = swap_2bit(x);
}

/******************************************************************************************************************************************************/

void ST7789_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	ST7789_SendCmd (ST7789_COLUMN_ADDR);
	ST7789_SendData(x1>>8);
	ST7789_SendData(x1 & 0xFF);
	ST7789_SendData(x2>>8);
	ST7789_SendData(x2 & 0xFF);

	y1 += 80;
	y2 += 80;
	ST7789_SendCmd (ST7789_PAGE_ADDR);
	ST7789_SendData(y1>>8);
	ST7789_SendData(y1 & 0xFF);
	ST7789_SendData(y2>>8);
	ST7789_SendData(y2 & 0xFF);
	ST7789_SendCmd (ST7789_GRAM);
}

void ST7789_SetCursorPositionRead(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	ST7789_SendCmd (ST7789_COLUMN_ADDR);
	ST7789_SendData(x1>>8);
	ST7789_SendData(x1 & 0xFF);
	ST7789_SendData(x2>>8);
	ST7789_SendData(x2 & 0xFF);

	y1 += 80;
	y2 += 80;
	ST7789_SendCmd (ST7789_PAGE_ADDR);
	ST7789_SendData(y1>>8);
	ST7789_SendData(y1 & 0xFF);
	ST7789_SendData(y2>>8);
	ST7789_SendData(y2 & 0xFF);

	ST7789_SendCmd (ST7789_GRAM_READ);
	ST7789_ReadData();//read dummy
}

uint16_t ST7789_ReadPixel(void)
{
	uint16_t data;
    uint16_t R = ST7789_ReadData() >> 2;
    uint16_t G = ST7789_ReadData() >> 2;
    uint16_t B = ST7789_ReadData() >> 2;

    // Convert RGB666 → RGB565
    data = ((B & 0x3E) << 10) | (G << 5) | (R >> 1);
    return data;
}


/******************************************************************************************************************************************************/

void ST7789_Init (uint32_t xSize, uint32_t ySize)
{
	//=============================================================================================
	ST7789_xSize = xSize;
	ST7789_ySize = ySize;
	ST7789_pixel_count = ST7789_xSize * ST7789_ySize;
	//Start Initial -------------------------------------------------------------------------------
	ST7789_SendCmd (ST7789_RESET); 
	HAL_Delay(100);
	ST7789_SendCmd (ST7789_DISPLAY_OFF); 
	//Power Control -------------------------------------------------------------------------------
	ST7789_SendCmd (ST7789_POWER1);
	ST7789_SendData(0x26);
	ST7789_SendCmd (ST7789_POWER2); 
	ST7789_SendData(0x11);
	ST7789_SendCmd (ST7789_VCOM1);
	ST7789_SendData(0x5C); 
	ST7789_SendData(0x4C); 
	ST7789_SendCmd (ST7789_VCOM2);
	ST7789_SendData(0x94);
	//Memory Access Control -----------------------------------------------------------------------
	ST7789_SendCmd (ST7789_MAC);
	ST7789_SendData(0x00);
	ST7789_SendCmd (ST7789_PIXEL_FORMAT);
	ST7789_SendData(0x05);/////
	//Frame Rate ----------------------------------------------------------------------------------
	ST7789_SendCmd (0xB0);
	ST7789_SendData(0x00);
	ST7789_SendData(0x60);
	
	ST7789_SendCmd (0xB3);
	ST7789_SendData(0x00);
	ST7789_SendData(0x00);
	ST7789_SendData(0x00);
	/*
	ST7789_SendCmd (ST7789_FRC);
	ST7789_SendData(0x00);
	ST7789_SendData(0x1B);
	*/
	//Gamma ---------------------------------------------------------------------------------------
	/*
	ST7789_SendCmd (ST7789_3GAMMA_EN);
	ST7789_SendData(0x08);
	ST7789_SendCmd (ST7789_GAMMA);
	ST7789_SendData(0x01);
	
	ST7789_SendCmd (ST7789_PGAMMA);
	ST7789_SendData(0x1F);
	ST7789_SendData(0x1A);
	ST7789_SendData(0x18);
	ST7789_SendData(0x0A);
	ST7789_SendData(0x0F);
	ST7789_SendData(0x06);
	ST7789_SendData(0x45);
	ST7789_SendData(0x87);
	ST7789_SendData(0x32);
	ST7789_SendData(0x0A);
	ST7789_SendData(0x07);
	ST7789_SendData(0x02);
	ST7789_SendData(0x07);
	ST7789_SendData(0x05);
	ST7789_SendData(0x00);
	ST7789_SendCmd (ST7789_NGAMMA);
	ST7789_SendData(0x00);
	ST7789_SendData(0x25);
	ST7789_SendData(0x27);
	ST7789_SendData(0x05);
	ST7789_SendData(0x10);
	ST7789_SendData(0x09);
	ST7789_SendData(0x3A);
	ST7789_SendData(0x78);
	ST7789_SendData(0x4D);
	ST7789_SendData(0x05);
	ST7789_SendData(0x18);
	ST7789_SendData(0x0D);
	ST7789_SendData(0x38);
	ST7789_SendData(0x3A);
	ST7789_SendData(0x1F);
	*/
	//DDRAM ---------------------------------------------------------------------------------------
	ST7789_SendCmd (ST7789_COLUMN_ADDR);
	ST7789_SendData(0x00); 
	ST7789_SendData(0x00); 
	ST7789_SendData(0x00);
	ST7789_SendData(0xEF); 
	ST7789_SendCmd (ST7789_PAGE_ADDR); 
	ST7789_SendData(0x00); 
	ST7789_SendData(0x00); 
	ST7789_SendData(0x00); 
	ST7789_SendData(0xEF); 
	ST7789_SendCmd (ST7789_TEARING_OFF);
	//ST7789_SendCmd (ST7789_TEARING_ON);
	ST7789_SendCmd (ST7789_DISPLAY_INVERSION);
	ST7789_SendData(0x00); 
	ST7789_SendCmd (ST7789_Entry_Mode_Set); 
	ST7789_SendData(0x07);
	///
	//Display -------------------------------------------------------------------------------------
	ST7789_SendCmd (ST7789_DFC);
	ST7789_SendData(0x0a);
	ST7789_SendData(0x82);
	ST7789_SendData(0x27);
	ST7789_SendData(0x00);
	//---------------------------------------------------------------------------------------------
	ST7789_SendCmd (0x0B);
	ST7789_SendData(0x00);
	ST7789_SendData(0x00);
	
	ST7789_SendCmd (ST7789_SLEEP_OUT);
	HAL_Delay(100);
	ST7789_SendCmd (ST7789_DISPLAY_ON);
	HAL_Delay(100);
	ST7789_SendCmd (ST7789_GRAM);
	HAL_Delay(200);

	ST7789_SendCmd (ST7789_INVON);

	ST7789_SendCmd (ST7789_MEMCONTROL);
	ST7789_SendData (ST7789_MADCTL_MV | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);
}

/******************************************************************************************************************************************************/

void ST7789_DrawPixel (uint16_t x, uint16_t y, uint16_t color) 
{
	if (x > 0 && x < 240 && y > 0 && y < 240){
		ST7789_SetCursorPosition(x, y, x, y);
		ST7789_SendData(color>>8);
		ST7789_SendData(color&0xFF);
	}
}

/******************************************************************************************************************************************************/

void ST7789_Fill(uint16_t color) 
{
#if 0
	uint8_t data[2];
	data[1] = ~((color >> 8) & 0xff);
	data[2] = ~(color & 0xff);
	color = ((color >> 8) & 0xff) | ((color << 8) & 0xff00);
	color = (color & 0xfc) | ((color << 1) & 2) | ((color >> 1) & 1);
	ST7789_SetCursorPosition (0, 0, 239, 239);
	for (uint32_t i = 0; i < ST7789_pixel_count; i++)
	{
		ST7789_SendData(data[1]);
		ST7789_SendData(data[2]);
//	}
#else
	uint8_t color_temp[2];
	color_temp[0] = swap_2bit(color >> 8);
	color_temp[1] = swap_2bit(color & 0xff);
	color =	(color_temp[0] << 8) | color_temp[1];

	ST7789_SetCursorPosition (0, 0, 239, 239);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream2, (uint32_t)&color, FSMC_ADDR, 240*240);
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream2, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
#endif
}

void ST7789_Fill_rectangle (uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
	uint8_t color_temp[2];
	color_temp[0] = swap_2bit(color >> 8);
	color_temp[1] = swap_2bit(color & 0xff);
	color =	(color_temp[0] << 8) | color_temp[1];

	ST7789_SetCursorPosition(x, y, w+x-1, h+y-1);
    HAL_DMA_Start(&hdma_memtomem_dma2_stream2, (uint32_t)&color, FSMC_ADDR, 240*240);
    HAL_DMA_PollForTransfer(&hdma_memtomem_dma2_stream2, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
}

void ST7789_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
{
	uint8_t line;
	ST7789_SetCursorPosition(x, y, 5+x, 7+y);
	for (int8_t j = 0; j<8; j++)
	{
		for (uint8_t i = 0; i < 6; i++)
		{
			if (i == 0) line = 0x0;
			else {
				line = pgm_read_byte(font1+(c*5)+i-1) >> j;
			}
			if (line & 0x1){
				ST7789_SendData(color >> 8);
				ST7789_SendData(color & 0xff);
			}
			else{
				ST7789_SendData(bg >> 8);
				ST7789_SendData(bg & 0xff);
			}
		}
	}
}

void ST7789_printText(char text[], int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size)
{
	int16_t offset;
	offset = size*6;
	for(uint16_t i=0; (i<40) && (text[i]!=0); i++)
		ST7789_drawChar(x+(offset*i), y, text[i],color,bg, size);
}

void lcd_print_char_to_buffer (uint8_t *buffer, char c, int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size){
	uint8_t line;
	for (uint8_t i = 0; i < 6; i++)
	{
		if (i == 5) line = 0x0;
		else line = pgm_read_byte(font1+(c*5)+i);

		for (int8_t j = 0; j<8; j++)
		{
			uint8_t x_pos, y_pos;
			x_pos = (x+i)*size;
			y_pos = (y+j)*size;
			if (line & 0x1){
				buffer[x_pos*2 + y_pos*2*240 + 1] = (color >> 8) & 0xff;
				buffer[x_pos*2 + y_pos*2*240] = color & 0xff;
			}
			else{
				if (bg != 0){
					buffer[x_pos*2 + y_pos*2*240 + 1] = (bg >> 8) & 0xff;
					buffer[x_pos*2 + y_pos*2*240] = bg & 0xff;
				}
			}
			line >>= 1;
		}
	}
}

void lcd_print_str_to_buffer (uint8_t *buffer, char *str, int16_t x, int16_t y, uint16_t color, uint16_t bg, uint8_t size){
	int16_t offset;
	offset = size*6;
	for(uint16_t i=0; (i<40) && (str[i]!=0); i++)
		lcd_print_char_to_buffer (buffer, str[i], x+(offset*i), y, color, bg, size);
}

void ST7789_printImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint8_t *data, RGB565_mode_t mode)
{
    ST7789_SetCursorPosition(x, y, w + x - 1, h + y - 1);

    uint16_t totalPixels = w * h;
    const uint8_t *ptr = data;

    if (mode == _MSB_FIRST) {
        for (uint16_t i = 0; i < totalPixels; i++) {
            ST7789_SendData(*ptr++);
            ST7789_SendData(*ptr++);
        }
    } else { // _LSB_FIRST
        for (uint16_t i = 0; i < totalPixels; i++) {
            uint8_t high = *ptr++;
            uint8_t low  = *ptr++;
            ST7789_SendData(low);
            ST7789_SendData(high);
        }
    }
}


