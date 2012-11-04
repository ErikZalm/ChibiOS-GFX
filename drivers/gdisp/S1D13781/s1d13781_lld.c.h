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

#ifndef S1D13781_H
#define S1D13781_H

#if defined(GDISP_USE_GPIO)
	#error "gdispS1D13781: LCD_USE_GPIO not implemented yet"

#elif defined(GDISP_USE_FSMC)
	#error "gdispS1D13781: LCD_USE_FSMC not implemented yet"

#elif defined(GDISP_USE_SPI)
	/*
	 * MapToRegSpace() - Add 0x60800 to the index in order to access register space.
	 */
	#define MapToRegSpace(Index)	 ( Index < 0xFC ? Index | 0x60800 : Index )

	#define  LUT2_Address           0x60400

	static uint16_t lcdSpiBuffer[4];
	static uint16_t lcdSpiReadBuffer[4];

	static void lld_lcdWaitBlit(void) {
		lcdSpiBuffer[0] = 0xC806;
		lcdSpiBuffer[1] = 0x0884;
		int i;

		SPID2.spi->CR1 |= SPI_CR1_BR_0; /* lower bitrate */
		for(i=0; i < 1000; i++) {
//			chThdSleepMicroseconds(10);
			spiSelect(&SPID2);
			spiExchange(&SPID2, 4, lcdSpiBuffer, lcdSpiReadBuffer);
			spiUnselect(&SPID2);
			if ((lcdSpiReadBuffer[3] & 0x01) != 1) break;
		}
		SPID2.spi->CR1 &= ~SPI_CR1_BR_0; /* restore bitrate */
	}

	static void lld_lcdWriteReg(uint32_t Index, uint32_t Value) {
		/* Map Index to register space */
		uint32_t Addr = MapToRegSpace( Index );

		/* Add Write 16 bit to address */
		Addr &= 0x00FFFFFF;
		Addr |= 0x88000000;

		lcdSpiBuffer[0] = Addr >> 16;
		lcdSpiBuffer[1] = Addr;
		lcdSpiBuffer[2] = Value;

		spiSelect(&SPID2);
		spiSend(&SPID2, 3, lcdSpiBuffer);
		spiUnselect(&SPID2);
	}

	static void lld_lcdWriteMemory(uint32_t Offset, uint32_t Value) {
		/* Add Write 16 bit to address */
		Offset &= 0x00FFFFFF;
		Offset |= 0x88000000;

		lcdSpiBuffer[0] = Offset >> 16;
		lcdSpiBuffer[1] = Offset;
		lcdSpiBuffer[2] = Value;

		spiSelect(&SPID2);
		spiSend(&SPID2, 3, lcdSpiBuffer);
		spiUnselect(&SPID2);
	}

	void lld_lcdBurstWriteMemory( uint32_t Offset, const uint16_t* Data, uint32_t Count )
	{
		/* Add Write 16 bit to address */
		Offset &= 0x00FFFFFF;
		Offset |= 0x88000000;

		lcdSpiBuffer[0] = Offset >> 16;
		lcdSpiBuffer[1] = Offset;

		spiSelect(&SPID2);
		spiSend(&SPID2, 2, lcdSpiBuffer);
		spiSend(&SPID2, Count, Data);
		spiUnselect(&SPID2);
	}
#else
#endif


#endif /* S1D13781_H */
