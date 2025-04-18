
#include "sirojuLIB_ov7670_SCCB.h"

I2C_HandleTypeDef SCCB_i2c;

uint8_t SCCB_write (uint8_t addr, uint8_t data)
{
	if (HAL_I2C_Mem_Write (&SCCB_i2c, SCCB_ADDR_W, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100) ==HAL_OK)
	{
		return 1;
	}
	else return 0;
}

void ov7670_config(I2C_HandleTypeDef *ov7670_i2c, GPIO_TypeDef* cam_rst_gpio, uint16_t cam_rst_pin)
{
	memcpy (&SCCB_i2c, ov7670_i2c, sizeof (*ov7670_i2c));
	
	HAL_GPIO_WritePin(cam_rst_gpio, cam_rst_pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(cam_rst_gpio, cam_rst_pin, GPIO_PIN_SET);
	HAL_Delay(10);
	SCCB_write(12, 0x80);
	HAL_Delay(20);

	for(int i = 0; i < (sizeof (OV7670_reg) / 2); i++)
	{
		SCCB_write(OV7670_reg[i][0], OV7670_reg[i][1]);
		HAL_Delay (1);
	}
}

