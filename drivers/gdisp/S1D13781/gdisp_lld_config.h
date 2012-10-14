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
 * @file    gdispS6d1121/gdisp_lld_config.h
 * @brief   GDISP Graphic Driver subsystem low level driver header for the S6d1121 display.
 *
 * @addtogroup GDISP
 * @{
 */

#ifndef _GDISP_LLD_CONFIG_H
#define _GDISP_LLD_CONFIG_H

#if HAL_USE_GDISP

/*===========================================================================*/
/* Driver hardware support.                                                  */
/*===========================================================================*/

#define GDISP_DRIVER_NAME				"S1D13781"
#define GDISP_LLD(x)					gdisp_lld_##x##_S1D13781

#define GDISP_HARDWARE_LINES			FALSE
#define GDISP_HARDWARE_CLEARS			TRUE
#define GDISP_HARDWARE_FILLS			TRUE
#define GDISP_HARDWARE_BITFILLS			TRUE
#define GDISP_HARDWARE_CIRCLES			FALSE
#define GDISP_HARDWARE_CIRCLEFILLS		FALSE
#define GDISP_HARDWARE_ELLIPSES			FALSE
#define GDISP_HARDWARE_ELLIPSEFILLS		FALSE
#define GDISP_HARDWARE_TEXT				FALSE
#define GDISP_HARDWARE_TEXTFILLS		TRUE
#define GDISP_HARDWARE_SCROLL			FALSE
#define GDISP_HARDWARE_PIXELREAD		FALSE
#define GDISP_HARDWARE_CONTROL			TRUE
#define GDISP_HARDWARE_QUERY			FALSE

#define GDISP_SOFTWARE_TEXTFILLDRAW		FALSE
#define GDISP_SOFTWARE_TEXTBLITCOLUMN	FALSE

#define GDISP_PIXELFORMAT				GDISP_PIXELFORMAT_CUSTOM
#define GDISP_PACKED_PIXELS				FALSE
#define GDISP_PACKED_LINES				FALSE

typedef uint16_t color_t;
#define COLOR(c)			((color_t)(c))
#define MASKCOLOR			FALSE
#define RGB2COLOR(r,g,b)	((color_t)((((b) & 0xF8)<<8) | (((g) & 0xFC)<<3) | (((r) & 0xF8)>>3)))
#define HTML2COLOR(h)		((color_t)((((h) & 0xF80000)>>8) | (((h) & 0x00FC00)>>5) | (((h) & 0x0000F8)>>3)))
#define RED_OF(c)			(((c) & 0xF800)>>8)
#define GREEN_OF(c)			(((c)&0x007E)>>3)
#define BLUE_OF(c)			(((c)&0x001F)<<3)
#define RGB565CONVERT(red, green, blue) (uint16_t)( (( blue   >> 3 ) << 11 ) | (( green >> 2 ) << 5  ) | ( red  >> 3 ))

#endif	/* HAL_USE_GDISP */

#endif	/* _GDISP_LLD_CONFIG_H */
/** @} */
