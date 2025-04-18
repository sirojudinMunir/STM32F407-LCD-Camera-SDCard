/*
 * app_camera.h
 *
 *  Created on: Feb 1, 2025
 *      Author: munir
 */

#ifndef INC_APP_CAMERA_H_
#define INC_APP_CAMERA_H_

#include "main.h"

//#define USB_CAMERA_STREAM

#define CAMERA_WIDTH  320
#define CAMERA_HEIGHT 240
#define LCD_WIDTH     240
#define LCD_HEIGHT    240
#define WIDTH  240
#define HEIGHT 240

typedef enum {
	NONE, RED, GREEN, BLUE
}color_t;


#define CAMERA_ENABLE() (CAM_EN_GPIO_Port->BSRR=CAM_EN_Pin)

#define CAMERA_DISABLE() (CAM_EN_GPIO_Port->BSRR=CAM_EN_Pin<<16)

#define CAMERA_START(buff,size) (HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,buff,size))

#define CAMERA_STOP() (HAL_DCMI_Stop(&hdcmi))

void camera_init (void);

void camera_crop (uint32_t x0, uint32_t y0, uint32_t x_size, uint32_t y_size);

_Bool color_filter (color_t color, uint16_t pixel);

void cam_filter_color (color_t filter);

void open_cam (void);

void close_cam (void);

void cam_take_picture (void);

#endif /* INC_APP_CAMERA_H_ */
