#ifndef _INTERFACE_ST7789_H
#define _INTERFACE_ST7789_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define Interface_STM32_HAL
#define FreeRTOS

#ifdef Interface_STM32_HAL
#include "main.h"
#define 	ST7789_SPI_HAL 	hspi1
#ifdef ST7789_SPI_HAL
	extern SPI_HandleTypeDef ST7789_SPI_HAL;
#endif
#ifdef FreeRTOS
#include "cmsis_os.h"
#else
#define USE_HAL_Delay
#endif
#endif


void ST7789_Delay(uint32_t Delay);
void ST7789_DC_LOW(void);
void ST7789_DC_HIGH(void);
void ST7789_RESET(void);
void ST7789_SPI_Transmit(uint8_t Data,uint16_t Size);
void ST7789_TransmitMassData(uint8_t* buff, size_t buff_size);
bool ST7789_SPI_GetState(void);

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* _INTERFACE_ST7789_H */
