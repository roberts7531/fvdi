#ifndef JOX030R3_H
#define JOX030R3_H

#include <stdint.h>
#include <stdbool.h>
#define VRAM_ADDR 0xC0800000

//register definitions
#define REG_SCROLLY             0x00
#define REG_SCROLLX             0x01
#define REG_PAL_IDX             0x02
#define REG_PAL_R               0x03
#define REG_PAL_G               0x04
#define REG_PAL_B               0x05
#define REG_SPR_XLOW            0x06
#define REG_SPR_XHIGH           0x07
#define REG_SPR_YLOW            0x08
#define REG_SPR_YHIGH           0x09
#define REG_SPR_IDX             0x0a
#define REG_SPR_DATA            0x0b
#define REG_BLT_START           0x0c
#define REG_BLT_CMD             0x0d
#define REG_BLT_DESTX_LOW       0x0e
#define REG_BLT_DESTX_HIGH      0x0f
#define REG_BLT_DESTY_LOW       0x10
#define REG_BLT_DESTY_HIGH      0x11
#define REG_BLT_PAT_COL         0x12
#define REG_BLT_PATT_DATA       0x13
#define REG_BLT_PATT_IDX        0x14
#define REG_BLT_WIDTH_LOW       0x15
#define REG_BLT_WIDTH_HIGH      0x16
#define REG_BLT_HEIGHT_LOW      0x17
#define REG_BLT_HEIGHT_HIGH     0x18
#define REG_BLT_PAT_BGCOL       0x19
#define REG_BLT_PAT_MODE        0x1A
#define REG_BLT_SRCX_LOW        0x1B
#define REG_BLT_SRCX_HIGH       0x1C
#define REG_BLT_SRCY_LOW        0x1D
#define REG_BLT_SRCY_HIGH       0x1E
#define REG_VRAM_STRIDE_LOW     0x1F
#define REG_VRAM_STRIDE_HIGH    0x20
#define REG_VIDEO_MODE          0x21
#define REG_TEXTMODE_INDEX_LOW  0x22
#define REG_TEXTMODE_INDEX_HIGH 0x23
#define REG_TEXTMODE_DATA       0x24
#define REG_TEXTMODE_MODE       0x25


#define TEXTMODE_MODE_ON        0
#define TEXTMODE_MODE_PARTIAL   1
#define TEXTMODE_MODE_OFF       2

#define REG_BLT_STATUS          0xc

#define BLT_CMD_FILL 0
#define BLT_CMD_BLIT 1

#define BLT_STATUS_RDY 1

#define BLT_START_MAGIC 0x3a

extern uint8_t colorLUT[16];
extern volatile uint8_t* vram;

void writeVideoReg(uint8_t reg, uint8_t value);
void writeVideoReg16Bit(uint8_t reg, uint16_t value);
uint8_t readVideoReg(uint8_t reg);
void waitForReady(void);

void setVideoMode(uint8_t mode, bool doubleSize);
void setVRAMStride(uint16_t stride);

void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t index);

void setScrollX(uint8_t scrollX);

void setScrollY(uint8_t scrollY);

void doBlit(uint16_t srcX, uint16_t srcY, uint16_t dstX, uint16_t dstY, uint16_t width, uint16_t height, bool waitForDone);

void fillPattern(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* pattern,uint8_t color,uint8_t bgcol, uint8_t isXor, bool waitForDone);

void loadSprite(uint16_t* spriteData);

void setSpritePosition(uint16_t x, uint16_t y);

void setTextAddr(uint16_t addr);

void outputChar(uint8_t ch);
void setTextModeMode(uint8_t mode);

#endif