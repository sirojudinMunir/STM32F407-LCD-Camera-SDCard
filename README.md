# STM32F407 LCD-Camera-SDCard File Viewer and Capture System

This project is an embedded system based on STM32F407 capable of reading and displaying various types of files from an SD card to a 240x240 pixel LCD screen. Additionally, the system can access a camera to capture images and save them in `.bmp` format. With USB Mass Storage Class (MSC) support, the device can function as a card reader when connected to a PC.

## Features

- Display files from SD card to LCD:
  - `.txt` and `.str` – Display text content
  - `.jpg` and `.bmp` – Display images on the screen

- Camera functionality:
  - Capture images from a camera module
  - Save captured images as `.bmp` files to the SD card

- USB MSC support:
  - Access the SD card directly from a PC via USB
  - No need to remove the SD card for file transfer

## Demo
https://youtube.com/shorts/tMKX52nq0iI?si=W2R6wjYyFYJ_C0jJ

## Peripherals

- SDIO 4-bit, for SD card access  
- FSMC 8-bit, for interfacing with 240x240 RGB LCD (ST7789)  
- DCMI 8-bit, for OV7670 camera module  
- USB FS + MSC, for accessing SD card as mass storage  
- Push Button  

## Hardware Requirements

- STM32F407VGT6 Board ([schematic](https://github.com/sirojudinMunir/STM32F407-LCD-Camera-SDCard/blob/master/STM32-CAM_schematics.pdf))
- 240x240 pixel SPI LCD (ST7789)
- SD card (FAT32 format)
- OV7670 camera module
- USB cable (for MSC and power)

## System Workflow

1. Insert an SD card containing files into the system.
2. The system displays a list of available files on startup.
3. Press the push button to move the cursor, long press to open folders/files.
4. Text and image files are displayed on the LCD screen.
5. Select **CAM.app** to launch the camera view.
6. The camera feature allows capturing and saving images.
7. When connected to a PC via USB, the system appears as external storage.

## Notes

- The SD card must be formatted with FAT32 file system.
- This project uses a [Custom Bootloader](https://github.com/sirojudinMunir/STM32F407-CustomBootloader) to simplify firmware updates. If you do not wish to use this bootloader, modify the flash memory origin address in the [linker script](https://github.com/sirojudinMunir/STM32F407-LCD-Camera-SDCard/blob/master/STM32F407VGTX_FLASH.ld) to `0x8000000`.

## USB MSC Mode

When the main application is running and the device is connected to a PC via USB, the SD card will appear as a **USB Mass Storage Device**, making it easy to transfer files without physically removing the SD card from the system.

## Author

**Moh Sirojudin Munir**  
Embedded systems developer specializing in STM32  
[LinkedIn](https://www.linkedin.com/in/moh-sirojudin-munir-3b01561b1) | [GitHub](https://github.com/sirojudinMunir) | [YouTube](https://www.youtube.com/@srj4555)
