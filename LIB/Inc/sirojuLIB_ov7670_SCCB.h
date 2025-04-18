
#ifndef __SIROJULIB_OV7670_SCCB_H
#define __SIROJULIB_OV7670_SCCB_H

#include "stm32f4xx_hal.h"
#include <string.h>

#define SCCB_ADDR_W 0x42


static const uint8_t OV7670_reg[][2] = {

		  /* Color mode related */
		  {0x12, 0x14},   // QVGA, RGB
		  {0x8C, 0x00},   // RGB444 Disable
		  {0x40, 0x10 + 0xc0},   // RGB565, 00 - FF
		  {0x3A, 0x04 + 8},   // UYVY (why?)
		  {0x3D, 0x80 | 0x00},   // gamma enable, UV auto adjust, UYVY
		  {0xB0, 0x84}, // important

		  /* clock related */
		  {0x0C, 0x04},  // DCW enable ------------------------
		  {0x3E, 0x19},  // manual scaling, pclk/=2
		  {0x70, 0x3A},  // scaling_xsc
		  {0x71, 0x35},  // scaling_ysc
		  {0x72, 0x11}, // down sample by 2
		  {0x73, 0xf1}, // DSP clock /= 2

		  /* windowing (empirically decided...) */

		  {0x17, 0x16},   // HSTART
		  {0x18, 0x04},   // HSTOP
		  {0x32, 0xa4},   // HREF
		  {0x19, 0x02},   // VSTART =  14 ( = 3 * 4 + 2)
		  {0x1a, 0x7a},   // VSTOP  = 494 ( = 123 * 4 + 2)
		  {0x03, 0x0a},   // VREF (VSTART_LOW = 2, VSTOP_LOW = 2)
		  //{0x1e, 0x30},

		  /*color gain*/
		  {0x00, 0x00},
		  {0x01, 0x82},
		  {0x02, 0x95},

		  //{0x13, 0x0D},//com8

		  /* color matrix coefficient */
		#if 1
		  {0x4f, 0xb3},
		  {0x50, 0xb3},
		  {0x51, 0x00},
		  {0x52, 0x3d},
		  {0x53, 0xa7},
		  {0x54, 0xe4},
		  {0x58, 0x9e},
		#else
		  {0x4f, 0x80},
		  {0x50, 0x80},
		  {0x51, 0x00},
		  {0x52, 0x22},
		  {0x53, 0x5e},
		  {0x54, 0x80},
		  {0x58, 0x9e},
		#endif

		  /* 3a */
		//  {0x13, 0x84},
		//  {0x14, 0x0a},   // AGC Ceiling = 2x
		//  {0x5F, 0x2f},   // AWB B Gain Range (empirically decided)
		//                  // without this bright scene becomes yellow (purple). might be because of color matrix
		//  {0x60, 0x98},   // AWB R Gain Range (empirically decided)
		//  {0x61, 0x70},   // AWB G Gain Range (empirically decided)
		  {0x41, 0x38},   // edge enhancement, de-noise, AWG gain enabled


		  /* gamma curve */
		#if 1
		  {0x7b, 16},
		  {0x7c, 30},
		  {0x7d, 53},
		  {0x7e, 90},
		  {0x7f, 105},
		  {0x80, 118},
		  {0x81, 130},
		  {0x82, 140},
		  {0x83, 150},
		  {0x84, 160},
		  {0x85, 180},
		  {0x86, 195},
		  {0x87, 215},
		  {0x88, 230},
		  {0x89, 244},
		  {0x7a, 16},
		#else
		  /* gamma = 1 */
		  {0x7b, 4},
		  {0x7c, 8},
		  {0x7d, 16},
		  {0x7e, 32},
		  {0x7f, 40},
		  {0x80, 48},
		  {0x81, 56},
		  {0x82, 64},
		  {0x83, 72},
		  {0x84, 80},
		  {0x85, 96},
		  {0x86, 112},
		  {0x87, 144},
		  {0x88, 176},
		  {0x89, 208},
		  {0x7a, 64},
		#endif

		  /* fps */
		//  {0x6B, 0x4a}, //PLL  x4
		  {0x11, 0x00}, // pre-scalar = 1/1
};

uint8_t SCCB_write (uint8_t addr, uint8_t data);
void ov7670_config(I2C_HandleTypeDef *ov7670_i2c, GPIO_TypeDef* cam_rst_gpio, uint16_t cam_rst_pin);

#endif
