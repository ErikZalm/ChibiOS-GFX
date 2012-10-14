/*
    ChibiOS-LCD-Driver - Copyright (C) 2012
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
 * @file    touchpadXPT2046/touchpad_lld.c
 * @brief   Touchpad Driver subsystem low level driver source.
 *
 * @addtogroup TOUCHPAD
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "touchpad.h"
#include "STMPE811.h"

#if HAL_USE_TOUCHPAD || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/
#if !defined(__DOXYGEN__)
    /* Local copy of the current touchpad driver */
    static const TOUCHPADDriver *tpDriver;

    static uint16_t sampleBuf[7];

#define TOUCH_WA_SIZE   THD_WA_SIZE(512)

static uint8_t rxbuf[16];
static uint8_t txbuf[16];
static i2cflags_t errors = 0;

static uint16_t TouchX;
static uint16_t TouchY;
static uint16_t TouchZ;
static uint16_t PenDown;

static int TouchCnt = 0;

#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

uint8_t stmpe811GetReg8(uint8_t RegAddr) {
	msg_t status = RDY_OK;
	systime_t tmo = MS2ST(100);

	txbuf[0] = RegAddr;
	i2cAcquireBus(&I2CD2);
	status = i2cMasterTransmitTimeout(&I2CD2, 0x82 >> 1, txbuf, 1, rxbuf, 1, tmo);
	i2cReleaseBus(&I2CD2);
	if (status != RDY_OK) {
		errors = i2cGetErrors(&I2CD2);
		return -1;
	}
	return rxbuf[0];
}

uint16_t stmpe811GetReg16(uint8_t RegAddr) {
	msg_t status = RDY_OK;
	systime_t tmo = MS2ST(100);

	txbuf[0] = RegAddr;
	i2cAcquireBus(&I2CD2);
	status = i2cMasterTransmitTimeout(&I2CD2, 0x82 >> 1, txbuf, 1, rxbuf, 2, tmo);
	i2cReleaseBus(&I2CD2);
	if (status != RDY_OK) {
		errors = i2cGetErrors(&I2CD2);
		return -1;
	}
	return (rxbuf[0] << 8 | rxbuf[1]);
}

uint8_t stmpe811PutReg8(uint8_t RegAddr, uint8_t RegVal) {
	msg_t status = RDY_OK;
	systime_t tmo = MS2ST(100);

	txbuf[0] = RegAddr;
	txbuf[1] = RegVal;
	i2cAcquireBus(&I2CD2);
	status = i2cMasterTransmitTimeout(&I2CD2, 0x82 >> 1, txbuf, 2, rxbuf, 0, tmo);
	i2cReleaseBus(&I2CD2);
	if (status != RDY_OK) {
		errors = i2cGetErrors(&I2CD2);
		return -1;
	}
	return 0;
}

/**
 * @brief   Touch screen thread function.
 *
 * @param[in] p         pointer to a @p BaseSequentialStream object
 * @return              Termination reason.
 * @retval RDY_OK       terminated by command.
 * @retval RDY_RESET    terminated by reset condition on the I/O channel.
 */
static msg_t touch_thread(void *p) {
	msg_t msg = RDY_OK;
	(void) p;

	chRegSetThreadName("touch");

	while (TRUE) {
		if((stmpe811GetReg8(STMPE811_TSC_CTRL) & TSC_CTRL_TSC_STA) != 0) { /* Touch detected */
			TouchCnt++;
			TouchX = stmpe811GetReg16(STMPE811_TSC_DATAX);
			TouchY = stmpe811GetReg16(STMPE811_TSC_DATAY);
			TouchZ = stmpe811GetReg8(STMPE811_TSC_DATAZ);
			stmpe811PutReg8(STMPE811_FIFO_STA, FIFO_STA_FIFO_RESET);
			stmpe811PutReg8(STMPE811_FIFO_STA, 0);
			if(TouchCnt > 2) {
			   PenDown = 1;
			   TouchCnt = 2;
			}
		}
		else {
			TouchCnt--;
			if(TouchCnt < 0) {
			   PenDown = 0;
			   TouchCnt = 0;
			}
		}
		chThdSleepMicroseconds(5000);
	}
	return msg;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/* ---- Required Routines ---- */

/**
 * @brief   Low level Touchpad driver initialization.
 *
 * @notapi
 */
void tp_lld_init(const TOUCHPADDriver *tp) {
	uint8_t regval;

	/**
	 * Check touch
	 */
	if (stmpe811GetReg16(0x00) != 0x811) {
		return;
	}

	/* Enable TSC and ADC functions */

	regval = stmpe811GetReg8(STMPE811_SYS_CTRL2);
	regval &= ~(SYS_CTRL2_TSC_OFF | SYS_CTRL2_ADC_OFF);
	stmpe811PutReg8(STMPE811_SYS_CTRL2, regval);

	/* Enable the TSC global interrupts */

	regval = stmpe811GetReg8(STMPE811_INT_EN);
	regval |= (uint32_t)(INT_TOUCH_DET | INT_FIFO_TH | INT_FIFO_OFLOW);
	stmpe811PutReg8(STMPE811_INT_EN, regval);

	/* Select Sample Time, bit number and ADC Reference */
	stmpe811PutReg8(STMPE811_ADC_CTRL1, ADC_CTRL1_MOD_12B | ADC_CTRL1_SAMPLE_TIME_80);

	/* Wait for 2 ms */
	chThdSleepMilliseconds(2);

	/* Select the ADC clock speed */
	stmpe811PutReg8(STMPE811_ADC_CTRL2, ADC_CTRL2_ADC_FREQ_3p25);

	/* Select TSC pins in non-GPIO mode (AF=0) */

	regval = stmpe811GetReg8(STMPE811_GPIO_AF);
	regval &= ~(uint8_t)TSC_PIN_SET;
	stmpe811PutReg8(STMPE811_GPIO_AF, regval);

	/* Select 2 nF filter capacitor */

	stmpe811PutReg8(STMPE811_TSC_CFG, (TSC_CFG_AVE_CTRL_4SAMPLES | TSC_CFG_TOUCH_DELAY_500US | TSC_CFG_SETTLING_500US));

	/* Select single point reading */

	stmpe811PutReg8(STMPE811_FIFO_TH, 1);

	/* Reset and clear the FIFO. */

	stmpe811PutReg8(STMPE811_FIFO_STA, FIFO_STA_FIFO_RESET);
	stmpe811PutReg8(STMPE811_FIFO_STA, 0);

	/* set the data format for Z value: 7 fractional part and 1 whole part */

	stmpe811PutReg8(STMPE811_TSC_FRACTIONZ, 0x07);

	/* Set the driving capability of the device for TSC pins: 50mA */

	stmpe811PutReg8(STMPE811_TSC_IDRIVE, TSC_IDRIVE_50MA);

	/* Enable the TSC.  Use no tracking index, touch-screen controller
	 * operation mode (XYZ).
	 */

	stmpe811PutReg8(STMPE811_TSC_CTRL, TSC_CTRL_EN);

	/* Clear all the status pending bits */

	stmpe811PutReg8(STMPE811_INT_STA, INT_ALL);

	chThdCreateFromHeap(NULL, TOUCH_WA_SIZE, NORMALPRIO, touch_thread,
			(void *) NULL);
}

/**
 * @brief   Reads out the X direction.
 *
 * @note    The samples are median filtered for greater noise reduction
 *
 * @notapi
 */
uint16_t tp_lld_read_x(void) {
	return TouchX;
}

/*
 * @brief	Reads out the Y direction.
 *
 * @notapi
 */
uint16_t tp_lld_read_y(void) {
	return TouchY;
}

/* ---- Optional Routines ---- */
#if TOUCHPAD_HAS_IRQ || defined(__DOXYGEN__)
	/*
	 * @brief	for checking if touchpad is pressed or not.
	 *
	 * @return	1 if pressed / 0 if not pressed
	 *
	 * @notapi
	 */
	uint8_t tp_lld_irq(void) {
		return (PenDown);
	}
#endif

#if TOUCHPAD_HAS_PRESSURE || defined(__DOXYGEN__)
	/*
	 * @brief	Reads out the Z direction / pressure.
	 *
	 * @notapi
	 */
	uint16_t tp_lld_read_z(void) {
		return TouchZ;
	}
#endif

#endif /* HAL_USE_TOUCHPAD */
/** @} */

