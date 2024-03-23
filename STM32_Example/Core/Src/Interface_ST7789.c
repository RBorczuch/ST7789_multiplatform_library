#include <Interface_ST7789.h>


inline void ST7789_Delay(uint32_t Delay){
#ifdef Interface_STM32_HAL
    #ifdef FreeRTOS
        osDelay(Delay);
    #else
        HAL_Delay(Delay);
    #endif
#endif
}

inline void ST7789_DC_LOW(void){
#ifdef Interface_STM32_HAL
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET);
#endif
}

inline void ST7789_DC_HIGH(void){
#ifdef Interface_STM32_HAL
    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);
#endif
}

inline void ST7789_RESET(void){
#ifdef Interface_STM32_HAL
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);
    ST7789_Delay(20);
    HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
#endif
}

void ST7789_SPI_Transmit(uint8_t Data, uint16_t Size){
#ifdef Interface_STM32_HAL
    HAL_SPI_Transmit(&ST7789_SPI_HAL, &Data, Size, HAL_MAX_DELAY);
#endif
}

bool ST7789_SPI_GetState(){
#ifdef Interface_STM32_HAL
	if(HAL_SPI_GetState(&ST7789_SPI_HAL) != HAL_SPI_STATE_READY){
		return true;
	}
	else return false;
#endif
}


void ST7789_TransmitMassData(uint8_t* buff, size_t buff_size){
#ifdef Interface_STM32_HAL
    // The maximum chunk size for HAL_SPI_Transmit is limited by the size parameter's type (uint16_t).
    const size_t MaxChunkSize = 0xFFFF;
    while (buff_size > 0) {
        // Calculate the size of the current chunk to transmit.
        // If buff_size is less than MaxChunkSize, it sends the remaining buff_size.
        // Otherwise, it sends a full chunk of MaxChunkSize.
        size_t chunkSize = (buff_size <= MaxChunkSize) ? buff_size : MaxChunkSize;

        // Transmit the current chunk.
        HAL_SPI_Transmit(&ST7789_SPI_HAL, buff, chunkSize, HAL_MAX_DELAY);

        // Move the buffer pointer forward by the size of the transmitted chunk.
        buff += chunkSize;

        // Decrease the remaining buffer size by the size of the transmitted chunk.
        buff_size -= chunkSize;
    }
#endif
}
