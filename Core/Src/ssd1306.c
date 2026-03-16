#include "ssd1306.h"
#include <string.h>

static I2C_HandleTypeDef *ssd1306_i2c;

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

static uint16_t CurrentX;
static uint16_t CurrentY;

static void SSD1306_WriteCommand(uint8_t cmd)
{
    uint8_t data[2];

    data[0] = 0x00;
    data[1] = cmd;

    HAL_I2C_Master_Transmit(ssd1306_i2c, SSD1306_I2C_ADDR, data, 2, HAL_MAX_DELAY);
}

static void SSD1306_WriteData(uint8_t* buffer, size_t buff_size)
{
    uint8_t data[buff_size + 1];

    data[0] = 0x40;
    memcpy(&data[1], buffer, buff_size);

    HAL_I2C_Master_Transmit(ssd1306_i2c, SSD1306_I2C_ADDR, data, buff_size + 1, HAL_MAX_DELAY);
}

void SSD1306_Init(I2C_HandleTypeDef *hi2c)
{
    ssd1306_i2c = hi2c;

    HAL_Delay(100);

    SSD1306_WriteCommand(0xAE);
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0xB0);
    SSD1306_WriteCommand(0xC8);
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0x10);
    SSD1306_WriteCommand(0x40);
    SSD1306_WriteCommand(0x81);
    SSD1306_WriteCommand(0xFF);
    SSD1306_WriteCommand(0xA1);
    SSD1306_WriteCommand(0xA6);
    SSD1306_WriteCommand(0xA8);
    SSD1306_WriteCommand(0x3F);
    SSD1306_WriteCommand(0xA4);
    SSD1306_WriteCommand(0xD3);
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0xD5);
    SSD1306_WriteCommand(0xF0);
    SSD1306_WriteCommand(0xD9);
    SSD1306_WriteCommand(0x22);
    SSD1306_WriteCommand(0xDA);
    SSD1306_WriteCommand(0x12);
    SSD1306_WriteCommand(0xDB);
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(0x8D);
    SSD1306_WriteCommand(0x14);
    SSD1306_WriteCommand(0xAF);

    SSD1306_Clear();
}

void SSD1306_UpdateScreen(void)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        SSD1306_WriteCommand(0xB0 + i);
        SSD1306_WriteCommand(0x00);
        SSD1306_WriteCommand(0x10);

        SSD1306_WriteData(&SSD1306_Buffer[SSD1306_WIDTH*i], SSD1306_WIDTH);
    }
}

void SSD1306_Clear(void)
{
    memset(SSD1306_Buffer, 0x00, sizeof(SSD1306_Buffer));
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y)
{
    if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
        return;

    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
}

void SSD1306_SetCursor(uint8_t x, uint8_t y)
{
    CurrentX = x;
    CurrentY = y;
}

char SSD1306_WriteChar(char ch, FontDef Font)
{
    uint8_t i,j;
    uint8_t line;

    if(ch < 32 || ch > 126)
        return 0;

    uint16_t index = (ch - 32) * Font.width;

    for(i=0;i<Font.width;i++)
    {
        line = Font.data[index + i];

        for(j=0;j<Font.height;j++)
        {
            if(line & (1 << j))
                SSD1306_DrawPixel(CurrentX+i,CurrentY+j);
        }
    }

    CurrentX += Font.width + 1;

    return ch;
}
char SSD1306_WriteString(char* str, FontDef Font)
{
    while(*str)
    {
        SSD1306_WriteChar(*str, Font);
        str++;
    }

    return *str;
}
