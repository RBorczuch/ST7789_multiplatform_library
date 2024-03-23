/*
 * ST7789.h
 *
 *  Created on: Mar 2, 2024
 *      Author: RB_PC
 */

#ifndef INC_ST7789_H_
#define INC_ST7789_H_

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include <Interface_ST7789.h>
#include "main.h"
//#include "fonts.h"

#include "stdlib.h"
#include "string.h"

#define	ST7789_IS_240X240

extern uint16_t ST7789_X_Start;
extern uint16_t ST7789_Y_Start;

#define RGB565(r, g, b)         (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))


//--- color------------------------------
	#define   	ST7789_BLACK   	0x0000
	#define   	ST7789_BLUE    	0x001F
	#define   	ST7789_RED     	0xF800
	#define   	ST7789_GREEN   	0x07E0
	#define 	ST7789_CYAN    	0x07FF
	#define 	ST7789_MAGENTA 	0xF81F
	#define 	ST7789_YELLOW  	0xFFE0
	#define 	ST7789_WHITE   	0xFFFF
//------------------------------------------------

// ST7789 ----
#define ST7789_ColorMode_65K    0x50
#define ST7789_ColorMode_262K   0x60
#define ST7789_ColorMode_12bit  0x03
#define ST7789_ColorMode_16bit  0x05
#define ST7789_ColorMode_18bit  0x06
#define ST7789_ColorMode_16M    0x07
//------------------------------------------------

#define ST7789_MADCTL_MY  		0x80
#define ST7789_MADCTL_MX  		0x40
#define ST7789_MADCTL_MV  		0x20
#define ST7789_MADCTL_ML  		0x10
#define ST7789_MADCTL_RGB 		0x00
#define ST7789_MADCTL_BGR 		0x08
#define ST7789_MADCTL_MH  		0x04
//-------------------------------------------------


#define ST7789_SWRESET 0x01
#define ST7789_SLPIN   0x10
#define ST7789_SLPOUT  0x11
#define ST7789_NORON   0x13
#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21
#define ST7789_DISPOFF 0x28
#define ST7789_DISPON  0x29
#define ST7789_CASET   0x2A
#define ST7789_RASET   0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_COLMOD  0x3A
#define ST7789_MADCTL  0x36
//-----------------------------------------------

#define DELAY 0x80
#define MAX_DELAY_MS 500

#ifdef ST7789_IS_240X240

	#define ST7789_WIDTH  			240
	#define ST7789_HEIGHT 			240
	#define ST7789_XSTART 			0
	#define ST7789_YSTART 			0
	#define ST7789_ROTATION 		(ST7789_MADCTL_MV|ST7789_MADCTL_ML|ST7789_MADCTL_MH)


#endif

// Deklaracje funkcji
void ST7789_Init(void);
void ST7789_SendCommand(uint8_t Cmd);
void ST7789_SendData(uint8_t Data);
void ST7789_SendDataStream(uint8_t* data_buffer, uint16_t buffer_size);
void ST7789_SendCommandStream(const uint8_t cmdArray[][10], uint8_t numCommandSets);
void ST7789_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ST7789_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void ST7789_FillScreen(uint16_t color);
void ST7789_RamWrite(uint16_t *pBuff, uint32_t Len);
void ST7789_Clear(void);

// Zmienne globalne i stałe
extern const uint8_t init_commands_count;
extern const uint8_t init_Commands[][10];


/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* INC_ST7789_H_ */
