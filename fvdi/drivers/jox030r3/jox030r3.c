#include "jox030r3.h"
#include <stdint.h>
#include <stdbool.h>

volatile uint8_t* vram = (uint8_t*)VRAM_ADDR;

inline void writeVideoReg(uint8_t reg, uint8_t value) {
    *(vram+0x200001+(reg<<1)) = value;
}

inline uint8_t readVideoReg(uint8_t reg) {
    return *((volatile uint8_t*)vram+0x200001+(reg<<1));
}


inline void writeVideoReg16Bit(uint8_t reg, uint16_t value) {
    writeVideoReg(reg,value&0xff);
	writeVideoReg(reg+1,value>>8);
}

void waitForReady(void) {
    while (!(readVideoReg(REG_BLT_STATUS)&BLT_STATUS_RDY));
}

void setVideoMode(uint8_t mode, bool doubleSize) {
    mode &= 0xf;
    if(doubleSize) mode |= 0x10;
    writeVideoReg(REG_VIDEO_MODE,mode); 
}

void setVRAMStride(uint16_t stride) {
    writeVideoReg16Bit(REG_VRAM_STRIDE_LOW,stride);
}

void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t index){
    writeVideoReg(REG_PAL_R,r);
    writeVideoReg(REG_PAL_G,g);
    writeVideoReg(REG_PAL_B,b);
    writeVideoReg(REG_PAL_IDX,index);
}

void setScrollX(uint8_t scrollX) {
    writeVideoReg(REG_SCROLLX,scrollX);
}

void setScrollY(uint8_t scrollY) {
    writeVideoReg(REG_SCROLLY,scrollY);
}

void doBlit(uint16_t srcX, uint16_t srcY, uint16_t dstX, uint16_t dstY, uint16_t width, uint16_t height, bool waitForDone) {
	waitForReady();
	writeVideoReg16Bit(REG_BLT_SRCX_LOW,srcX);
	writeVideoReg16Bit(REG_BLT_SRCY_LOW,srcY);
	writeVideoReg16Bit(REG_BLT_DESTX_LOW,dstX);
	writeVideoReg16Bit(REG_BLT_DESTY_LOW,dstY);
	writeVideoReg16Bit(REG_BLT_WIDTH_LOW,width);
	writeVideoReg16Bit(REG_BLT_HEIGHT_LOW,height);
	
    writeVideoReg(REG_BLT_CMD, BLT_CMD_BLIT);
    writeVideoReg(REG_BLT_START,BLT_START_MAGIC);
    if ( waitForDone ) waitForReady();
}

void fillPattern(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t* pattern,uint8_t color,uint8_t bgcol, uint8_t isXor, bool waitForDone){

	waitForReady();
    writeVideoReg16Bit(REG_BLT_DESTX_LOW,x);
    writeVideoReg16Bit(REG_BLT_DESTY_LOW,y);

    writeVideoReg16Bit(REG_BLT_PAT_MODE, isXor);

    writeVideoReg(REG_BLT_PAT_COL,color);
    writeVideoReg(REG_BLT_PAT_BGCOL,bgcol);
	
    writeVideoReg16Bit(REG_BLT_WIDTH_LOW,width);
    writeVideoReg16Bit(REG_BLT_HEIGHT_LOW,height);
    
    writeVideoReg(REG_BLT_PATT_IDX,0);
	int i;
	for(i = 0; i<16;i++){
		
		writeVideoReg(REG_BLT_PATT_DATA,pattern[i]>>8);
		writeVideoReg(REG_BLT_PATT_DATA,pattern[i]&0xff);
		
	}
    writeVideoReg(REG_BLT_CMD,BLT_CMD_FILL);
    writeVideoReg(REG_BLT_START,BLT_START_MAGIC);
	if ( waitForDone ) waitForReady();
}

void loadSprite(uint16_t* spriteData) {
    writeVideoReg(REG_SPR_IDX,0);
    uint8_t* data8 = (uint8_t*) spriteData;
	int i;
	for(i = 0;i<32;i++){
		writeVideoReg(REG_SPR_DATA,*(data8+i));
	}
}

void setSpritePosition(uint16_t x, uint16_t y) {
    writeVideoReg16Bit(REG_SPR_XLOW,x);
    writeVideoReg16Bit(REG_SPR_YLOW,y);
}

void setTextAddr(uint16_t addr) {
    writeVideoReg16Bit(REG_TEXTMODE_INDEX_LOW,addr);
}

void outputChar(uint8_t ch) {
    writeVideoReg(REG_TEXTMODE_DATA,ch);
}

void setTextModeMode(uint8_t mode) {
    writeVideoReg(REG_TEXTMODE_MODE,mode);
}