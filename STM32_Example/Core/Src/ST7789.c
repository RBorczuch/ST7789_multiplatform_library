/*
 * ST7789.c
 *
 *  Created on: Mar 2, 2024
 *      Author: RB_PC
 */
#include <ST7789.h>

const uint8_t init_commands_count;
const uint8_t init_Commands[][10] = { 	// Przyjmujemy maksymalną długość 10 dla każdego zestawu komend
    {3, ST7789_SWRESET, DELAY, 150}, 			// Software reset z opóźnieniem 150ms
    {3, ST7789_SLPOUT,  DELAY, 255},			 // Wybudzenie z opóźnieniem 500ms (255 oznacza 500ms w logice funkcji)
    {4, ST7789_COLMOD,  ST7789_ColorMode_65K | ST7789_ColorMode_16bit, DELAY, 10}, // Ustawienie koloru z opóźnieniem
    {2, ST7789_MADCTL,  0x00}, 					// Ustawienie kierunku pamięci
    {5, ST7789_CASET,   0x00, 0x00, 0x00, 240}, // Ustawienie zakresu kolumn
    {5, ST7789_RASET,   0x00, 0x00, (320>>8), (320&0xFF)}, // Ustawienie zakresu wierszy
    {3, ST7789_INVON,   DELAY, 10}, 			// Inwersja ON z opóźnieniem
    {3, ST7789_NORON,   DELAY, 10}, 			// Normalny ekran ON z opóźnieniem
    {3, ST7789_DISPON,  DELAY, 10}  			// Włączenie ekranu z opóźnieniem
};

uint16_t ST7789_X_Start = ST7789_XSTART;
uint16_t ST7789_Y_Start = ST7789_YSTART;

uint16_t ST7789_Width, ST7789_Height;

void ST7789_SendCommand(uint8_t Cmd){

		ST7789_DC_LOW();
		ST7789_SPI_Transmit(Cmd,1);
		while(ST7789_SPI_GetState()){};
		ST7789_DC_HIGH();
}
void ST7789_SendData(uint8_t Data){

		ST7789_DC_HIGH();
		ST7789_SPI_Transmit(Data,1);
		while(ST7789_SPI_GetState()){};
}
static void ST7789_ProcessDelay(uint8_t delayValue) {
    uint16_t delayMs = delayValue;
    if (delayMs == 255) {
        delayMs = MAX_DELAY_MS; // Dłuższe opóźnienie
    }
    ST7789_Delay(delayMs);
}

void ST7789_SendDataStream(uint8_t *data_buffer, uint16_t buffer_size) {

	ST7789_TransmitMassData(data_buffer, buffer_size);
	while(ST7789_SPI_GetState()){};
}

void ST7789_SendCommandStream(const uint8_t cmdArray[][10], uint8_t numCommandSets) {
    for (uint8_t i = 0; i < numCommandSets; i++) {
        const uint8_t* cmdSet = cmdArray[i];
        uint8_t numElements = cmdSet[0]; 			// Pierwszy element określa liczbę elementów w zestawie
        ST7789_SendCommand(cmdSet[1]); 				// Drugi element to komenda

        uint8_t j = 2; 								// Początek argumentów komendy
        while (j < numElements) {
            if (cmdSet[j] == DELAY) {
                									// Sprawdzenie, czy następuje opóźnienie
                j++; 								// Przesunięcie do wartości opóźnienia
                ST7789_ProcessDelay(cmdSet[j]);
            } else {
            	ST7789_SendData(*(cmdSet + j));	// Wysyłanie argumentu do komendy
            }
            j++;
        }
    }
}

void ST7789_Init(void){
	ST7789_Width = ST7789_WIDTH;
	ST7789_Height = ST7789_HEIGHT;

	ST7789_RESET();
	const uint8_t init_commands_count = sizeof(init_Commands) / sizeof(init_Commands[0]);
	ST7789_SendCommandStream(init_Commands, init_commands_count);
}

/*void ST7789_RamWrite(uint16_t *pBuff, uint32_t Len){

  uint8_t buff[2];
  buff[0] = *pBuff >> 8;
  buff[1] = *pBuff & 0xFF;

  while (Len--){
	  ST7789_SendData(buff[0]);
	  ST7789_SendData(buff[1]);
	  //ST7789_TransmitMassData(buff, 2);
  }

}

void ST7789_RamWrite( uint16_t *pBuff, uint32_t Len){
    uint8_t buff[2];
    while (Len--) {
        buff[0] = *pBuff >> 8;
        buff[1] = *pBuff & 0xFF;
        ST7789_SendData(buff[0]);
        ST7789_SendData(buff[1]);
        pBuff++; // Move to the next color in the buffer
    }
}*/
void ST7789_RamWrite(uint16_t *pBuff, uint32_t Len){
  uint8_t buff[2];
  buff[0] = *pBuff >> 8;
  buff[1] = *pBuff & 0xFF;

  while (Len--){
    //ST7789_SendData( buff[0] );
    //ST7789_SendData( buff[1] );
	  ST7789_SendDataStream( buff, 2);
  }

}

void ST7789_ColumnSet(uint16_t ColumnStart, uint16_t ColumnEnd){

  if (ColumnStart > ColumnEnd){
    return;
  }

  if (ColumnEnd > ST7789_Width){
    return;
  }

  ColumnStart += ST7789_X_Start;
  ColumnEnd += ST7789_X_Start;

  ST7789_SendCommand(ST7789_CASET);
  ST7789_SendData(ColumnStart >> 8);
  ST7789_SendData(ColumnStart & 0xFF);
  ST7789_SendData(ColumnEnd >> 8);
  ST7789_SendData(ColumnEnd & 0xFF);
}

void ST7789_RowSet(uint16_t RowStart, uint16_t RowEnd){

  if (RowStart > RowEnd){
    return;
  }

  if (RowEnd > ST7789_Height){
    return;
  }

  RowStart += ST7789_Y_Start;
  RowEnd += ST7789_Y_Start;

  ST7789_SendCommand(ST7789_RASET);
  ST7789_SendData(RowStart >> 8);
  ST7789_SendData(RowStart & 0xFF);
  ST7789_SendData(RowEnd >> 8);
  ST7789_SendData(RowEnd & 0xFF);

}

void ST7789_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){

	ST7789_ColumnSet(x0, x1);
	ST7789_RowSet(y0, y1);
	ST7789_SendCommand(ST7789_RAMWR);
}

void ST7789_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){

  if ((x >= ST7789_Width) || (y >= ST7789_Height)){
	  return;
  }

  if ((x + w) > ST7789_Width){
	  w = ST7789_Width - x;
  }

  if ((y + h) > ST7789_Height){
	  h = ST7789_Height - y;
  }

  ST7789_SetWindow(x, y, x + w - 1, y + h - 1);

  ST7789_RamWrite(&color, (h * w));
}

void ST7789_FillScreen(uint16_t color){

  ST7789_FillRect(0, 0,  ST7789_Width, ST7789_Height, color);
}

void ST7789_Clear(void){

  ST7789_FillRect(0, 0,  ST7789_Width, ST7789_Height, 0);
}
