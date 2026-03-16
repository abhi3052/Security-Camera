#ifndef __SSD1306_H
#define __SSD1306_H

#include "stm32f4xx_hal.h"
#include "fonts.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#define SSD1306_I2C_ADDR (0x3C << 1)

void SSD1306_Init(I2C_HandleTypeDef *hi2c);
void SSD1306_UpdateScreen(void);
void SSD1306_Clear(void);

void SSD1306_DrawPixel(uint8_t x, uint8_t y);

void SSD1306_SetCursor(uint8_t x, uint8_t y);

char SSD1306_WriteChar(char ch, FontDef Font);
char SSD1306_WriteString(char* str, FontDef Font);

#endif
