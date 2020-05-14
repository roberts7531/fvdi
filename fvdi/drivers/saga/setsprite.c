# Pastebin yqKGPvMM
/******************************************************************************
**
**  Function:   SetSprite()
**
**  Purpose:    This function activates or deactivates the hardware sprite.
**
**  Parameters: A0: struct   BoardInfo
**              D0: BOOL     activate
**              D7: RGBFTYPE RGBFormat
**
**  Returns:    -
**
*******************************************************************************/

/*
	WORD	gbi_MouseX
	WORD	gbi_MouseY
	UBYTE	gbi_MouseWidth
	UBYTE	gbi_MouseHeight
	UBYTE	gbi_MouseXOffset
	UBYTE	gbi_MouseYOffset
	APTR	gbi_MouseImage
	STRUCT	gbi_MousePens,4*1
	LABEL	gbi_MouseRect
	WORD	gbi_MouseMinX
	WORD	gbi_MouseMinY
	WORD	gbi_MouseMaxX
	WORD	gbi_MouseMaxY
	APTR	gbi_MouseChunky
	APTR	gbi_MouseRendered
	APTR	gbi_MouseSaveBuffer
*/

BOOL ASM SAVEDS Card_SetSprite(
	__REGA0(struct BoardInfo *bi),
	__REGD0(BOOL              activate),
	__REGD7(RGBFTYPE          rgbFormat))
{
	if (LIBDEBUG)
		KPrintF(
		"Card_SetSprite($%08lx, %ld, %ld)\n",
		bi, activate, rgbFormat);
	
	if( activate )
	{
		// DMA ON: put position inside window
		Write16(SAGA_VIDEO_SPRITEX, bi->CardData[CARDDATA_MOUSEX]);
		Write16(SAGA_VIDEO_SPRITEY, bi->CardData[CARDDATA_MOUSEY]);
		bi->CardData[CARDDATA_FLAGS] &= ~CARDD_FLG_MOUSEOFF;
	}
	else
	{
		// DMA OFF: put position outside window
		
		Write16(SAGA_VIDEO_SPRITEX, SAGA_VIDEO_MAXHV - 1);
		Write16(SAGA_VIDEO_SPRITEY, SAGA_VIDEO_MAXHV - 1);
		bi->CardData[CARDDATA_FLAGS] |= CARDD_FLG_MOUSEOFF;
   	}
	
	return activate;
}

/******************************************************************************
**
**  Function:   SetSpriteColor()
**
**  Purpose:    This function changes one color of the hardware sprite.
**
**  Parameters: A0: struct   BoardInfo
**              D0: UBYTE    colorIndex (0 to 2)
**              D1: UBYTE    red        (0 to 255)
**              D2: UBYTE    green      (0 to 255)
**              D3: UBYTE    blue       (0 to 255)
**              D7: RGBFTYPE RGBFormat
**
**  Returns:    -
**
*******************************************************************************/

void SAVEDS ASM Card_SetSpriteColor(
	__REGA0(struct BoardInfo *bi),
	__REGD0(UBYTE             colorIndex),
	__REGD1(UBYTE             r),
	__REGD2(UBYTE             g),
	__REGD3(UBYTE             b),
	__REGD7(RGBFTYPE          rgbFormat))
{
	if (LIBDEBUG)
		KPrintF(
		"Card_SetSpriteColor($%08lx, %ld, %ld, %ld, %ld, %ld)\n",
		bi, colorIndex, r, g, b, rgbFormat);
	
	if ( BOARDID != VREG_BOARD_V4SA )
	{
		return; // Only for Vampire V4
	}
	
	if( colorIndex <= 2 )
	{
		// Input : 0x00RRGGBB
		// Output: 0x0RGB
		
		Write16(
			SAGA_VIDEO_SPRITECLUT + ( colorIndex << 1 ), 
			( ( r & 0xf0 ) << 4 ) + // RED
			( ( g & 0xf0 )      ) + // GREEN
			( ( b & 0xf0 ) >> 4 )   // BLUE
		);
	}
	
	return;
}

/******************************************************************************
**
**  Function:   SetSpriteImage()
**
**  Purpose:    This function gets new sprite image data from the MouseImage 
**              field of the BoardInfo structure and writes it to the board.
**
**  Parameters: A0: struct   BoardInfo
**              D7: RGBFTYPE RGBFormat
**
**  Returns:    -
**
*******************************************************************************/

void ASM SAVEDS Card_SetSpriteImage(
	__REGA0(struct BoardInfo *bi),
	__REGD7(RGBFTYPE          rgbFormat))
{
	UBYTE i, h = bi->MouseHeight;
	ULONG dptr = SAGA_VIDEO_SPRITEBPL;
	ULONG *sptr;

	if (LIBDEBUG)
		KPrintF(
		"Card_SetSpriteImage($%08lx, %ld)\n",
		bi, rgbFormat);
	
	sptr = (ULONG*)bi->MouseImage;
	
	sptr++; // skip header (4 Bytes)

	if( h > 16 )
	{
		// AmigaOS Sprite is 16x24
		// SAGA HW Sprite is 16x16
		
		h = 16;
	}
	
	for( i = 0 ; i < h ; i++ )
	{
		Write32(dptr, *sptr++);
		dptr += 4;
	}

	for( ; i < 16 ; i++ )
	{
		Write32(dptr, 0);
		dptr += 4;
	}

	return;
}

/******************************************************************************
**
**  Function:   SetSpritePosition()
**
**  Purpose:    This function sets the hardware mouse sprite position 
**              according to the values in the BoardInfo structure.
**
**  Parameters: A0: struct   BoardInfo
**              D0: WORD     mouseX
**              D1: WORD     mouseY
**              D7: RGBFTYPE RGBFormat
**
**  Returns:    -
**
*******************************************************************************/

void ASM SAVEDS Card_SetSpritePosition(
	__REGA0(struct BoardInfo *bi),
	__REGD0(WORD              x),
	__REGD1(WORD              y),
	__REGD7(RGBFTYPE          rgbFormat))
{
	UBYTE flags = bi->CardData[CARDDATA_FLAGS];

/*
	if (LIBDEBUG)
		KPrintF(
		"Card_SetSpritePosition($%08lx, %ld, %ld, %ld)\n",
		bi, x, y, rgbFormat);
*/
	
	if( flags & CARDD_FLG_MOUSEOFF )
	{
		return;
	}
	
	x = bi->MouseX - bi->XOffset;
	y = bi->MouseY - bi->YOffset;
	
	if( flags & CARDD_FLG_DBLX )
	{
		x <<= 1; // multiply by 2
	}
	
	if( flags & CARDD_FLG_DBLY )
	{
		y <<= 1; // multiply by 2
	}

	if ( BOARDID == VREG_BOARD_V4SA )
	{
		x += SAGA_MOUSE_DELTAX;
		y += SAGA_MOUSE_DELTAY;
	}
	
	Write16(SAGA_VIDEO_SPRITEX, x);
	Write16(SAGA_VIDEO_SPRITEY, y);
	bi->CardData[CARDDATA_MOUSEX] = (ULONG)x;
	bi->CardData[CARDDATA_MOUSEY] = (ULONG)y;
}
