/*
    ChibiOS/RT - Copyright (C) 2012
                 Joel Bodenmann aka Tectu <joel@unormal.org>

    This file is part of ChibiOS-LCD-Driver.

    ChibiOS-LCD-Driver is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS-LCD-Driver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    gdispS6d1121/gdisp_lld.c
 * @brief   GDISP Graphics Driver subsystem low level driver source for the S6d1121 display.
 *
 * @addtogroup GDISP
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "gdisp.h"

#if HAL_USE_GDISP || defined(__DOXYGEN__)

/* Include the emulation code for things we don't support */
#include "gdisp_emulation.c"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*
 * Maximum speed SPI configuration (21MHz, CPHA=0, CPOL=0, MSb first).
 */
static const SPIConfig spi2cfg = {
  NULL,
  GPIOB,
  GPIOB_DISP_NCS,
  SPI_CR1_DFF
};

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#include "s1d13781_lld.c.h"

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/* ---- Required Routines ---- */
/*
	The following 2 routines are required.
	All other routines are optional.
*/

/**
 * @brief   Low level GDISP driver initialization.
 *
 * @notapi
 */
bool_t GDISP_LLD(init)(void) {

	#ifdef GDISP_USE_SPI
	  spiStart(&SPID2, &spi2cfg);
	#endif

	lld_lcdWriteReg( 0x06, 0x0100 ); /* Software Reset Register                            */
	chThdSleepMicroseconds(10000);   /* LCD Panel Power On Delay (in ms)                   */
	lld_lcdWriteReg( 0x04, 0x0002 ); /* Power Save Register                                */
	lld_lcdWriteReg( 0x10, 0x0000 ); /* PLL Setting Register 0                             */
	lld_lcdWriteReg( 0x12, 0x000F ); /* PLL Setting Register 1                             */
	lld_lcdWriteReg( 0x14, 0x0029 ); /* PLL Setting Register 2                             */
	lld_lcdWriteReg( 0x10, 0x0001 ); /* PLL Setting Register 0                             */
	chThdSleepMicroseconds(2500);    /* PLL Register Programming Delay (in us)             */
	lld_lcdWriteReg( 0x16, 0x0006 ); /* Internal Clock Configuration Register              */
	lld_lcdWriteReg( 0x04, 0x0002 ); /* Power Save Register                                */
	lld_lcdWriteReg( 0x20, 0x004F ); /* Panel Setting Register                             */
	lld_lcdWriteReg( 0x22, 0x0001 ); /* Display Setting Register                           */
	lld_lcdWriteReg( 0x24, 0x003C ); /* Horizontal Display Width Register                  */
	lld_lcdWriteReg( 0x26, 0x002D ); /* Horizontal Non-Display Period Register             */
	lld_lcdWriteReg( 0x28, 0x0110 ); /* Vertical Display Height Register                   */
	lld_lcdWriteReg( 0x2A, 0x0012 ); /* Vertical Non-Display Period Register               */
	lld_lcdWriteReg( 0x2C, 0x0090 ); /* HS Pulse Width Register                            */
	lld_lcdWriteReg( 0x2E, 0x0010 ); /* HS Pulse Start Position Register                   */
	lld_lcdWriteReg( 0x30, 0x0002 ); /* VS Pulse Width Register                            */
	lld_lcdWriteReg( 0x32, 0x0011 ); /* VS Pulse Start Position Register                   */
	lld_lcdWriteReg( 0x40, 0x0001 ); /* Main Layer Setting Register                        */
	lld_lcdWriteReg( 0x42, 0x0000 ); /* Main Layer Start Address Register 0                */
	lld_lcdWriteReg( 0x44, 0x0000 ); /* Main Layer Start Address Register 1                */
	lld_lcdWriteReg( 0x50, 0x0006 ); /* PIP Layer Setting Register                         */
	lld_lcdWriteReg( 0x52, 0xFC00 ); /* PIP Layer Start Address Register 0                 */
	lld_lcdWriteReg( 0x54, 0x0003 ); /* PIP Layer Start Address Register 1                 */
	lld_lcdWriteReg( 0x56, 0x01e0 ); /* PIP Layer Width Register                           */
	lld_lcdWriteReg( 0x58, 0x0110 ); /* PIP Layer Height Register                          */
	lld_lcdWriteReg( 0x5A, 0x0000 ); /* PIP Layer X Start Position Register                */
	lld_lcdWriteReg( 0x5C, 0x0000 ); /* PIP Layer Y Start Position Register                */
	lld_lcdWriteReg( 0x60, 0x0000 ); /* PIP Enable Register                                */
	lld_lcdWriteReg( 0x62, 0x0040 ); /* Alpha Blending Register                            */
	lld_lcdWriteReg( 0x64, 0x0000 ); /* Transparency Register                              */
	lld_lcdWriteReg( 0x66, 0x0000 ); /* Key Color Register 0                               */
	lld_lcdWriteReg( 0x68, 0x0000 ); /* Key Color Register 1                               */
	lld_lcdWriteReg( 0xD0, 0x0000 ); /* GPIO Configuration Register                        */
	lld_lcdWriteReg( 0xD2, 0x0000 ); /* GPIO Status / Control Register                     */
	lld_lcdWriteReg( 0xD4, 0x000F ); /* GPIO Pull-Down Control Register                    */
	chThdSleepMicroseconds(10000);

    /*
     * Enable backlight
     */
	lld_lcdWriteReg(0xD0, 0x0003);
	lld_lcdWriteReg(0xD2, 0x0003);

//	lld_lcdResetViewPort();

	/* Now initialise the GDISP structure */
	GDISP.clipx0 = 0;
	GDISP.clipy0 = 0;
	GDISP.clipx1 = GDISP_SCREEN_WIDTH;
	GDISP.clipy1 = GDISP_SCREEN_HEIGHT;
	GDISP.Width = GDISP_SCREEN_WIDTH;
	GDISP.Height = GDISP_SCREEN_HEIGHT;
	GDISP.Orientation = GDISP_ROTATE_0;
	GDISP.Powermode = powerOn;
	GDISP.Backlight = 100;
	GDISP.Contrast = 50;
	return TRUE;
}

/**
 * @brief   Draws a pixel on the display.
 *
 * @param[in] x        X location of the pixel
 * @param[in] y        Y location of the pixel
 * @param[in] color    The color of the pixel
 *
 * @notapi
 */
void GDISP_LLD(drawpixel)(coord_t x, coord_t y, color_t color) {
	#if GDISP_NEED_VALIDATION
		if (x >= GDISP.Width || y >= GDISP.Height) return;
	#endif
	lld_lcdWriteMemory((x * 2 + y * GDISP.Width * 2), color);
}

/* ---- Optional Routines ---- */
/*
	All the below routines are optional.
	Defining them will increase speed but everything
	will work if they are not defined.
	If you are not using a routine - turn it off using
	the appropriate GDISP_HARDWARE_XXXX macro.
	Don't bother coding for obvious similar routines if
	there is no performance penalty as the emulation software
	makes a good job of using similar routines.
		eg. If fillarea() is defined there is little
			point in defining clear() unless the
			performance bonus is significant.
	For good performance it is suggested to implement
		fillarea() and blitarea().
*/

#if GDISP_HARDWARE_CLEARS || defined(__DOXYGEN__)
	/**
	 * @brief   Clear the display.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] color    The color of the pixel
	 *
	 * @notapi
	 */
	void GDISP_LLD(clear)(color_t color) {
		uint16_t lcdBitBLTBuffer[14];
	    lld_lcdWriteReg( 0x80, 0x80);
		lcdBitBLTBuffer[0] = 0x04;          /* 0x82 */
		lcdBitBLTBuffer[2] = 0x02;          /* 0x86 */
		lcdBitBLTBuffer[5] = 0x00;          /* 0x8C */
		lcdBitBLTBuffer[6] = 0x00;          /* 0x8E */
		lcdBitBLTBuffer[7] = GDISP_SCREEN_WIDTH;  /* 0x90 */
		lcdBitBLTBuffer[8] = GDISP_SCREEN_WIDTH;  /* 0x92 */
		lcdBitBLTBuffer[9] = GDISP_SCREEN_HEIGHT; /* 0x94 */
		lcdBitBLTBuffer[12] = color;        /* 0x9A */
		lld_lcdBurstWriteMemory(0x60882, lcdBitBLTBuffer, 14);

	    lld_lcdWriteReg( 0x80, 0x01);
	    lld_lcdWaitBlit();
	}
#endif

#if GDISP_HARDWARE_LINES || defined(__DOXYGEN__)
	/**
	 * @brief   Draw a line.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] x0, y0   The start of the line
	 * @param[in] x1, y1   The end of the line
	 * @param[in] color    The color of the line
	 *
	 * @notapi
	 */
	void GDISP_LLD(drawline)(coord_t x0, coord_t y0, coord_t x1, coord_t y1, color_t color) {
		#if GDISP_NEED_VALIDATION
			/* Need to clip to screen */
		#endif
		/* Code here */
	}
#endif

#if GDISP_HARDWARE_FILLS || defined(__DOXYGEN__)
	/**
	 * @brief   Fill an area with a color.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] x, y     The start filled area
	 * @param[in] cx, cy   The width and height to be filled
	 * @param[in] color    The color of the fill
	 *
	 * @notapi
	 */
	void GDISP_LLD(fillarea)(coord_t x, coord_t y, coord_t cx, coord_t cy, color_t color) {
		#if GDISP_NEED_VALIDATION
			if (cx < 1 || cy < 1 || x >= GDISP.Width || y >= GDISP.Height) return;
			if (x+cx > GDISP.Width)	cx = GDISP.Width - x;
			if (y+cy > GDISP.Height)	cy = GDISP.Height - y;
		#endif

		uint16_t lcdBitBLTBuffer[14];
		lcdBitBLTBuffer[0] = 0x04;          /* 0x82 */
		lcdBitBLTBuffer[2] = 0x02;          /* 0x86 */
		lcdBitBLTBuffer[5] = (y * GDISP_SCREEN_WIDTH * 2) + (x * 2);         /* 0x8C */
		lcdBitBLTBuffer[6] = ((y * GDISP_SCREEN_WIDTH * 2) + (x * 2)) >> 16;  /* 0x8E */
		lcdBitBLTBuffer[7] = GDISP_SCREEN_WIDTH;  /* 0x90 */
		lcdBitBLTBuffer[8] = cx;            /* 0x92 */
		lcdBitBLTBuffer[9] = cy;            /* 0x94 */
		lcdBitBLTBuffer[12] = color;        /* 0x9A */
		lld_lcdBurstWriteMemory(0x60882, lcdBitBLTBuffer, 14);

		lld_lcdWriteReg( 0x80, 0x01);
		lld_lcdWaitBlit();
	}
#endif

#if GDISP_HARDWARE_BITFILLS || defined(__DOXYGEN__)
	/**
	 * @brief   Fill an area with a bitmap.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] x, y     The start filled area
	 * @param[in] cx, cy   The width and height to be filled
	 * @param[in] srcx, srcy   The bitmap position to start the fill from
	 * @param[in] srccx    The width of a line in the bitmap.
	 * @param[in] buffer   The pixels to use to fill the area.
	 *
	 * @notapi
	 */
	void GDISP_LLD(blitareaex)(coord_t x, coord_t y, coord_t cx, coord_t cy, coord_t srcx, coord_t srcy, coord_t srccx, const pixel_t *buffer) {
		coord_t endy;
		unsigned lg;

		#if GDISP_NEED_VALIDATION || GDISP_NEED_CLIP
			if (x < GDISP.clipx0) { cx -= GDISP.clipx0 - x; srcx += GDISP.clipx0 - x; x = GDISP.clipx0; }
			if (y < GDISP.clipy0) { cy -= GDISP.clipy0 - y; srcy += GDISP.clipy0 - y; y = GDISP.clipy0; }
			if (srcx+cx > srccx)		cx = srccx - srcx;
			if (cx <= 0 || cy <= 0 || x >= GDISP.clipx1 || y >= GDISP.clipy1) return;
			if (x+cx > GDISP.clipx1)	cx = GDISP.clipx1 - x;
			if (y+cy > GDISP.clipy1)	cy = GDISP.clipy1 - y;
		#endif

//		lld_lcdSetViewPort(x, y, cx, cy);
//		lld_lcdWriteStreamStart();

//		endx = srcx + cx;
		endy = y + cy;
		lg = srccx;
		buffer += srcx + srcy * srccx;
		for(; y < endy; y++, buffer += lg)
			lld_lcdBurstWriteMemory((x * 2) + ((y) * GDISP_SCREEN_WIDTH * 2), buffer, cx);

//			for(x=srcx; x < endx; x++)
//				lld_lcdWriteData(*buffer++);
//		lld_lcdWriteStreamStop();
//		lld_lcdResetViewPort();
	}
#endif

/* Circular Drawing Functions */
#if (GDISP_NEED_CIRCLE && GDISP_HARDWARE_CIRCLES) || defined(__DOXYGEN__)
	/**
	 * @brief   Draw a circle.
	 * @note    Optional - The high level driver can emulate using software.
	 * @note    If GDISP_NEED_CLIPPING is defined this routine MUST behave
	 *          correctly if the circle is over the edges of the screen.
	 *
	 * @param[in] x, y     The centre of the circle
	 * @param[in] radius   The radius of the circle
	 * @param[in] color    The color of the circle
	 *
	 * @notapi
	 */
	void GDISP_LLD(drawcircle)(coord_t x, coord_t y, coord_t radius, color_t color) {
		#if GDISP_NEED_VALIDATION
			/* Code here */
		#endif
		/* Code here */
	}
#endif

#if (GDISP_NEED_CIRCLE && GDISP_HARDWARE_CIRCLEFILLS) || defined(__DOXYGEN__)
	/**
	 * @brief   Create a filled circle.
	 * @note    Optional - The high level driver can emulate using software.
	 * @note    If GDISP_NEED_CLIPPING is defined this routine MUST behave
	 *          correctly if the circle is over the edges of the screen.
	 *
	 * @param[in] x, y     The centre of the circle
	 * @param[in] radius   The radius of the circle
	 * @param[in] color    The color of the circle
	 *
	 * @notapi
	 */
	void GDISP_LLD(fillcircle)(coord_t x, coord_t y, coord_t radius, color_t color) {
		#if GDISP_NEED_VALIDATION
			/* Code here */
		#endif
		/* Code here */
	}
#endif

#if (GDISP_NEED_ELLIPSE && GDISP_HARDWARE_ELLIPSES) || defined(__DOXYGEN__)
	/**
	 * @brief   Draw an ellipse.
	 * @note    Optional - The high level driver can emulate using software.
	 * @note    If GDISP_NEED_CLIPPING is defined this routine MUST behave
	 *          correctly if the ellipse is over the edges of the screen.
	 *
	 * @param[in] x, y     The centre of the ellipse
	 * @param[in] a, b     The dimensions of the ellipse
	 * @param[in] color    The color of the ellipse
	 *
	 * @notapi
	 */
	void GDISP_LLD(drawellipse)(coord_t x, coord_t y, coord_t a, coord_t b, color_t color) {
		#if GDISP_NEED_VALIDATION
			/* Code here */
		#endif
		/* Code here */
	}
#endif

#if (GDISP_NEED_ELLIPSE && GDISP_HARDWARE_ELLIPSEFILLS) || defined(__DOXYGEN__)
	/**
	 * @brief   Create a filled ellipse.
	 * @note    Optional - The high level driver can emulate using software.
	 * @note    If GDISP_NEED_CLIPPING is defined this routine MUST behave
	 *          correctly if the ellipse is over the edges of the screen.
	 *
	 * @param[in] x, y     The centre of the ellipse
	 * @param[in] a, b     The dimensions of the ellipse
	 * @param[in] color    The color of the ellipse
	 *
	 * @notapi
	 */
	void GDISP_LLD(fillellipse)(coord_t x, coord_t y, coord_t a, coord_t b, color_t color) {
		#if GDISP_NEED_VALIDATION
			/* Code here */
		#endif
		/* Code here */
	}
#endif

#if (GDISP_NEED_TEXT && GDISP_HARDWARE_TEXT) || defined(__DOXYGEN__)
	#include "gdisp_fonts.h"
#endif

#if (GDISP_NEED_TEXT && GDISP_HARDWARE_TEXT) || defined(__DOXYGEN__)
	/**
	 * @brief   Draw a character using a transparent background.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] x, y     The top-left corner of the text
	 * @param[in] c        The character to print
	 * @param[in] color    The color of the character
	 *
	 * @notapi
	 */
	void GDISP_LLD(drawchar)(coord_t x, coord_t y, char c, font_t font, color_t color) {
		#if GDISP_NEED_VALIDATION
			/* Code here */
		#endif
		/* Code here */
	}
#endif

#if (GDISP_NEED_TEXT && GDISP_HARDWARE_TEXTFILLS) || defined(__DOXYGEN__)
	/**
	 * @brief   Draw a character using a filled background.
	 * @note    Optional - The high level driver can emulate using software.
	 *
	 * @param[in] x, y     The top-left corner of the text
	 * @param[in] c        The character to print
	 * @param[in] color    The color of the character
	 * @param[in] bgcolor  The background color
	 *
	 * @notapi
	 */
	void GDISP_LLD(fillchar)(coord_t x, coord_t y, char c, font_t font, color_t color, color_t bgcolor) {
		volatile coord_t	width, height;  // TODO check why volatile is needed. Otherwise width * height = 0 if height is optimized out
		coord_t	xscale, yscale;

		/* Check we actually have something to print */
		width = _getCharWidth(font, c);
		if (!width) return;

		xscale = font->xscale;
		yscale = font->yscale;
		height = font->height * yscale;
		width *= xscale;

		const fontcolumn_t	*ptr;
		fontcolumn_t		column;
		coord_t				i, j, xs, ys;

		/* Working buffer for fast non-transparent text rendering [patch by Badger]
			This needs to be larger than the largest character we can print.
			Assume the max is double sized.
		*/
		static pixel_t		buf[20*(sizeof(fontcolumn_t)*8)*2];

		#if GDISP_NEED_VALIDATION
			/* Check our buffer is big enough */
			if ((unsigned)(width * height) > sizeof(buf)/sizeof(buf[0]))	return;
		#endif

		ptr = _getCharData(font, c);

		/* Loop through the data and display. The font data is LSBit first, down the column */
		for(i = 0; i < width; i+=xscale) {
			/* Get the font bitmap data for the column */
			column = *ptr++;

			/* Draw each pixel */
			for(j = 0; j < height; j+=yscale, column >>= 1) {
				if (column & 0x01) {
					for(xs=0; xs < xscale; xs++)
						for(ys=0; ys < yscale; ys++)
							gdispPackPixels(buf, width, i+xs, j+ys, color);
				} else {
					for(xs=0; xs < xscale; xs++)
						for(ys=0; ys < yscale; ys++)
							gdispPackPixels(buf, width, i+xs, j+ys, bgcolor);
				}
			}
		}

		/*
		 * write char in display memory (after end of normal memory)
		 */
		lld_lcdBurstWriteMemory(480 * 272 * 2, buf, width * height);

		uint16_t lcdBitBLTBuffer[10];
		lcdBitBLTBuffer[0] = 0x05;                                           /* 0x82 16bpp, source linear */
		lcdBitBLTBuffer[2] = 0x00;                                           /* 0x86 Move positive */
		lcdBitBLTBuffer[3] = ((480*272)*2) & 0xffff;                         /* 0x88 Source start address */
		lcdBitBLTBuffer[4] = (480*272*2)>>16;                                /* 0x8A Source start address */
		lcdBitBLTBuffer[5] = (y * GDISP_SCREEN_WIDTH * 2) + (x * 2);         /* 0x8C Destination start address */
		lcdBitBLTBuffer[6] = ((y * GDISP_SCREEN_WIDTH * 2) + (x * 2)) >> 16; /* 0x8E Destination start address */
		lcdBitBLTBuffer[7] = GDISP_SCREEN_WIDTH;                             /* 0x90 Rectangle offset (screen width) */
		lcdBitBLTBuffer[8] = width;                                          /* 0x92 Width */
		lcdBitBLTBuffer[9] = height;                                         /* 0x94 Height */
		lld_lcdBurstWriteMemory(0x60882, lcdBitBLTBuffer, 10);

		lld_lcdWriteReg( 0x80, 0x01);                                        /* Start blit */
	}
#endif

#if (GDISP_NEED_PIXELREAD && GDISP_HARDWARE_PIXELREAD) || defined(__DOXYGEN__)
	/**
	 * @brief   Get the color of a particular pixel.
	 * @note    Optional.
	 * @note    If x,y is off the screen, the result is undefined.
	 *
	 * @param[in] x, y     The start of the text
	 *
	 * @notapi
	 */
	color_t GDISP_LLD(getpixelcolor)(coord_t x, coord_t y) {
		/* This routine is marked "DO NOT USE" in the original
		 *  GLCD driver. We just keep our GDISP_HARDWARE_READPIXEL
		 *  turned off for now.
		 */
		color_t color;

		#if GDISP_NEED_VALIDATION
			if (x >= GDISP.Width || y >= GDISP.Height) return 0;
		#endif

		lld_lcdSetCursor(x, y);
		lld_lcdWriteStreamStart();

		color = lld_lcdReadData();
		color = lld_lcdReadData();

		lld_lcdWriteStreamStop();

		return color;
	}
#endif

#if (GDISP_NEED_SCROLL && GDISP_HARDWARE_SCROLL) || defined(__DOXYGEN__)
	/**
	 * @brief   Scroll vertically a section of the screen.
	 * @note    Optional.
	 * @note    If x,y + cx,cy is off the screen, the result is undefined.
	 * @note    If lines is >= cy, it is equivelent to a area fill with bgcolor.
	 *
	 * @param[in] x, y     The start of the area to be scrolled
	 * @param[in] cx, cy   The size of the area to be scrolled
	 * @param[in] lines    The number of lines to scroll (Can be positive or negative)
	 * @param[in] bgcolor  The color to fill the newly exposed area.
	 *
	 * @notapi
	 */
	void GDISP_LLD(verticalscroll)(coord_t x, coord_t y, coord_t cx, coord_t cy, int lines, color_t bgcolor) {
		/* This is marked as "TODO: Test this" in the original GLCD driver.
		 * For now we just leave the GDISP_HARDWARE_SCROLL off.
		 */
		static color_t buf[((SCREEN_HEIGHT > SCREEN_WIDTH ) ? SCREEN_HEIGHT : SCREEN_WIDTH)];
		coord_t row0, row1;
		unsigned i, gap, abslines;

		abslines = lines < 0 ? -lines : lines;

		#if GDISP_NEED_VALIDATION
			if (cx < 1 || cy < 1 || x >= GDISP.Width || y >= GDISP.Height) return;
			if (x+cx > GDISP.Width)	cx = GDISP.Width - x;
			if (y+cy > GDISP.Height)	cy = GDISP.Height - y;
		#endif

		if (!abslines) return;
		if (abslines >= cy) {
			abslines = cy;
			gap = 0;
		} else {
			gap = cy - abslines;
			for(i = 0; i < gap; i++) {
				if(lines > 0) {
					row0 = y + i + lines;
					row1 = y + i;
				} else {
					row0 = (y - i - 1) + lines;
					row1 = (y - i - 1);
				}

				/* read row0 into the buffer and then write at row1*/
				lld_lcdSetViewPort(x, row0, cx, 1);
				lld_lcdReadStreamStart();
				lld_lcdReadStream(buf, cx);
				lld_lcdReadStreamStop();

				lld_lcdSetViewPort(x, row1, cx, 1);
				lld_lcdWriteStreamStart();
				lld_lcdWriteStream(buf, cx);
				lld_lcdWriteStreamStop();
			}
		}

		/* fill the remaining gap */
		lld_lcdSetViewPort(x, lines > 0 ? (y+gap) : y, cx, abslines);
		lld_lcdWriteStreamStart();
		gap = cx*abslines;
		for(i = 0; i < gap; i++) lld_lcdWriteData(bgcolor);
		lld_lcdWriteStreamStop();
		lld_lcdResetViewPort();
	}
#endif

#if GDISP_HARDWARE_CONTROL || defined(__DOXYGEN__)
	/**
	 * @brief   Driver Control
	 * @detail	Unsupported control codes are ignored.
	 * @note	The value parameter should always be typecast to (void *).
	 * @note	There are some predefined and some specific to the low level driver.
	 * @note	GDISP_CONTROL_POWER			- Takes a gdisp_powermode_t
	 * 			GDISP_CONTROL_ORIENTATION	- Takes a gdisp_orientation_t
	 * 			GDISP_CONTROL_BACKLIGHT -	 Takes an int from 0 to 100. For a driver
	 * 											that only supports off/on anything other
	 * 											than zero is on.
	 * 			GDISP_CONTROL_CONTRAST		- Takes an int from 0 to 100.
	 * 			GDISP_CONTROL_LLD			- Low level driver control constants start at
	 * 											this value.
	 *
	 * @param[in] what		What to do.
	 * @param[in] value		The value to use (always cast to a void *).
	 *
	 * @notapi
	 */
	void GDISP_LLD(control)(unsigned what, void *value) {
		switch(what) {
		case GDISP_CONTROL_POWER:
			if (GDISP.Powermode == (gdisp_powermode_t)value)
				return;
			switch((gdisp_powermode_t)value) {
				case powerOff:
					/* 	Code here */
					/* break; */
				case powerOn:
					/* 	Code here */
					/* You may need this ---
						if (GDISP.Powermode != powerSleep)
							GDISP_LLD(init();
					*/
					/* break; */
				case powerSleep:
					/* 	Code here */
					/* break; */
				default:
					return;
			}
			GDISP.Powermode = (gdisp_powermode_t)value;
			return;
		case GDISP_CONTROL_ORIENTATION:
			if (GDISP.Orientation == (gdisp_orientation_t)value)
				return;
			switch((gdisp_orientation_t)value) {
			case GDISP_ROTATE_0:
				lld_lcdWriteReg(0x0001,0x0127);
				lld_lcdWriteReg(0x03, 0b0011);
				GDISP.Height = GDISP_SCREEN_HEIGHT;
				GDISP.Width = GDISP_SCREEN_WIDTH;
				break;
			case GDISP_ROTATE_90:
				lld_lcdWriteReg(0x0001,0x0027);
				lld_lcdWriteReg(0x0003, 0b1011);
				GDISP.Height = GDISP_SCREEN_WIDTH;
				GDISP.Width = GDISP_SCREEN_HEIGHT;
				break;
			case GDISP_ROTATE_180:
				lld_lcdWriteReg(0x0001,0x0127);
				lld_lcdWriteReg(0x0003, 0b0000);
				GDISP.Height = GDISP_SCREEN_HEIGHT;
				GDISP.Width = GDISP_SCREEN_WIDTH;
				break;
			case GDISP_ROTATE_270:
				lld_lcdWriteReg(0x0001,0x0027);
				lld_lcdWriteReg(0x0003, 0b1000);
				GDISP.Height = GDISP_SCREEN_WIDTH;
				GDISP.Width = GDISP_SCREEN_HEIGHT;
				break;
			default:
				return;
			}
			GDISP.Orientation = (gdisp_orientation_t)value;
			return;
/*
		case GDISP_CONTROL_BACKLIGHT:
		case GDISP_CONTROL_CONTRAST:
*/
		}
	}
#endif

#if (GDISP_NEED_QUERY && GDISP_HARDWARE_QUERY) || defined(__DOXYGEN__)
/**
 * @brief   Query a driver value.
 * @detail	Typecase the result to the type you want.
 * @note	GDISP_QUERY_WIDTH			- (coord_t)	Gets the width of the screen
 * 			GDISP_QUERY_HEIGHT			- (coord_t)	Gets the height of the screen
 * 			GDISP_QUERY_POWER			- (gdisp_powermode_t) Get the current powermode
 * 			GDISP_QUERY_ORIENTATION		- (gdisp_orientation_t) Get the current screen orientation
 * 			GDISP_QUERY_BACKLIGHT		- (coord_t) Get the backlight state (0 to 100)
 * 			GDISP_QUERY_CONTRAST		- (coord_t) Get the contrast (0 to 100).
 * 			GDISP_QUERY_LLD				- Low level driver control constants start at
 * 											this value.
 *
 * @param[in] what     What to Query
 *
 * @notapi
 */
void *GDISP_LLD(query)(unsigned what) {
	switch(what) {
	case GDISP_QUERY_WIDTH:			return (void *)(unsigned)GDISP.Width;
	case GDISP_QUERY_HEIGHT:		return (void *)(unsigned)GDISP.Height;
	case GDISP_QUERY_POWER:			return (void *)(unsigned)GDISP.Powermode;
	case GDISP_QUERY_ORIENTATION:	return (void *)(unsigned)GDISP.Orientation;
	case GDISP_QUERY_BACKLIGHT:		return (void *)(unsigned)GDISP.Backlight;
	case GDISP_QUERY_CONTRAST:		return (void *)(unsigned)GDISP.Contrast;
	case GDISP_QUERY_LLD+0:
		/* Code here */
	default:						return (void *)-1;
	}
}
#endif

#endif /* HAL_USE_GDISP */
/** @} */
