/*
    Marlin - Copyright (C) 2012 Erik van der Zalm.

    This file is part of Marlin.

    Marlin is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STMPE811.h
 * @brief   touchscreen controller header.
 *
 * @addtogroup DISPLAY
 * @{
 */

#ifndef _STMPE811_H_
#define _STMPE811_H_

/* STMPE811 Registers ************************************************************************/
/* Register Addresses */

#define STMPE811_CHIP_ID             0x00  /* Device identification (16-bit) */
#define STMPE811_ID_VER              0x02  /* Revision number: 0x01=sample 0x03=final silicon */
#define STMPE811_SYS_CTRL1           0x03  /* Reset control */
#define STMPE811_SYS_CTRL2           0x04  /* Clock control */
#define STMPE811_SPI_CFG             0x08  /* SPI interface configuration */
#define STMPE811_INT_CTRL            0x09  /* Interrupt control register */
#define STMPE811_INT_EN              0x0a  /* Interrupt enable register */
#define STMPE811_INT_STA             0x0b  /* Interrupt status register */
#define STMPE811_GPIO_EN             0x0c  /* GPIO interrupt enable register */
#define STMPE811_GPIO_INTSTA         0x0d  /* GPIO interrupt status register */
#define STMPE811_ADC_INTEN           0x0e  /* ADC interrupt enable register */
#define STMPE811_ADC_INTSTA          0x0f  /* ADC interrupt status register */
#define STMPE811_GPIO_SETPIN         0x10  /* GPIO set pin register */
#define STMPE811_GPIO_CLRPIN         0x11  /* GPIO clear pin register */
#define STMPE811_GPIO_MPSTA          0x12  /* GPIO monitor pin state register */
#define STMPE811_GPIO_DIR            0x13  /* GPIO direction register */
#define STMPE811_GPIO_ED             0x14  /* GPIO edge detect register */
#define STMPE811_GPIO_RE             0x15  /* GPIO rising edge register */
#define STMPE811_GPIO_FE             0x16  /* GPIO falling edge register */
#define STMPE811_GPIO_AF             0x17  /* Alternate function register */
#define STMPE811_ADC_CTRL1           0x20  /* ADC control */
#define STMPE811_ADC_CTRL2           0x21  /* ADC control */
#define STMPE811_ADC_CAPT            0x22  /* To initiate ADC data acquisition */
#define STMPE811_ADC_DATACH(n)       (0x30 + ((n) << 1))  /* ADC channel n (16-bit) */
#define STMPE811_ADC_DATACH0         0x30  /* ADC channel 0 (16-bit) */
#define STMPE811_ADC_DATACH1         0x32  /* ADC channel 1 (16_bit) */
#define STMPE811_ADC_DATACH2         0x34  /* ADC channel 2 (16-bit) */
#define STMPE811_ADC_DATACH3         0x36  /* ADC channel 3 (16-bit) */
#define STMPE811_ADC_DATACH4         0x38  /* ADC channel 4 (16-bit) */
#define STMPE811_ADC_DATACH5         0x3a  /* ADC channel 5 (16-bit) */
#define STMPE811_ADC_DATACH6         0x3c  /* ADC channel 6 (16-bit) */
#define STMPE811_ADC_DATACH7         0x3e  /* ADC channel 7 (16-bit) */
#define STMPE811_TSC_CTRL            0x40  /* 4-wire touchscreen controller setup */
#define STMPE811_TSC_CFG             0x41  /* Touchscreen controller configuration */
#define STMPE811_WDW_TRX             0x42  /* Window setup for top right X (16-bit) */
#define STMPE811_WDW_TRY             0x44  /* Window setup for top right Y (16-bit) */
#define STMPE811_WDW_BLX             0x46  /* Window setup for bottom left X (16-bit) */
#define STMPE811_WDW_BLY             0x48  /* Window setup for bottom left Y (16-bit) */
#define STMPE811_FIFO_TH             0x4a  /* FIFO level to generate interrupt */
#define STMPE811_FIFO_STA            0x4b  /* Current status of FIFO */
#define STMPE811_FIFO_SIZE           0x4c  /* Current filled level of FIFO */
#define STMPE811_TSC_DATAX           0x4d  /* Data port for touchscreen (16-bit) */
#define STMPE811_TSC_DATAY           0x4f  /* Data port for touchscreen (16-bit) */
#define STMPE811_TSC_DATAZ           0x51  /* Data port for touchscreen */
#define STMPE811_TSC_DATAXYZ         0x52  /* Data port for touchscreen (32-bit) */
#define STMPE811_TSC_FRACTIONZ       0x56  /* Touchscreen controller FRACTION_Z */
#define STMPE811_TSC_DATA            0x57  /* Data port for touchscreen */
#define STMPE811_TSC_IDRIVE          0x58  /* Touchscreen controller drive I */
#define STMPE811_TSC_SHIELD          0x59  /* Touchscreen controller shield */
#define STMPE811_TEMP_CTRL           0x60  /* Temperature sensor setup */
#define STMPE811_TEMP_DATA           0x61  /* Temperature data access port */
#define STMPE811_TEMP_TH             0x62  /* Threshold for temperature controlled interrupt */

/* Register bit definitions */

/* Device identification (16-bit) */

#define CHIP_ID                      0x0811

/* Revision number: 0x01=sample 0x03=final silicon */

#define ID_VER_SAMPLE                0x01
#define ID_VER_FINAL                 0x03

/* Reset control */

#define SYS_CTRL1_HIBERNATE          (1 << 0)  /* Bit 0: Force the device into hibernation mode */
#define SYS_CTRL1_SOFTRESET          (1 << 1)  /* Bit 1: Reset the STMPE811 */

/* Clock control */

#define SYS_CTRL2_ADC_OFF            (1 << 0)  /* Bit 0: Switch off clock to ADC */
#define SYS_CTRL2_TSC_OFF            (1 << 1)  /* Bit 1: Switch off clock to touchscreen controller */
#define SYS_CTRL2_GPIO_OFF           (1 << 2)  /* Bit 2: Switch off clock to GPIO */
#define SYS_CTRL2_TS_OFF             (1 << 3)  /* Bit 3: Switch off clock to temperature sensor */

/* SPI interface configuration */

#define SPI_CFG_SPI_CLK_MOD0         (1 << 0)  /* Bit 0: Value of SCAD/A0 pin at power-up reset */
#define SPI_CFG_SPI_CLK_MOD1         (1 << 1)  /* Bit 1: Value of SCAD/A0 pin at power-up reset */
#define SPI_CFG_AUTO_INCR            (1 << 2)  /* Bit 2: Enable internally autoincremented addressing */

/* Interrupt control register */

#define INT_CTRL_GLOBAL_INT          (1 << 0)  /* Bit 0: Master interrupt enable */
#define INT_CTRL_INT_TYPE            (1 << 1)  /* Bit 1: Type of interrupt signal.  1=edge */
#define INT_CTRL_INT_POLARITY        (1 << 2)  /* Bit 2: Interrupt pin polarity. 1=active high */

/* Interrupt enable/status register */

#define INT_TOUCH_DET                (1 << 0)  /* Bit 0: Touch is detected */
#define INT_FIFO_TH                  (1 << 1)  /* Bit 1: FIFO is equal or above threshold value */
#define INT_FIFO_OFLOW               (1 << 2)  /* Bit 2: FIFO is overflowed */
#define INT_FIFO_FULL                (1 << 3)  /* Bit 3: FIFO is full */
#define INT_FIFO_EMPTY               (1 << 4)  /* Bit 4: FIFO is empty */
#define INT_TEMP_SENS                (1 << 5)  /* Bit 5: Temperature threshold triggering */
#define INT_ADC                      (1 << 6)  /* Bit 6: Any enabled ADC interrupts */
#define INT_GPIO                     (1 << 7)  /* Bit 7: Any enabled GPIO interrupts */
#define INT_ALL                      0xff

/* GPIO interrupt enable/status register */

#define GPIO_INT(n)                  (1 << (n))

/* ADC interrupt enable/status register */

#define ADC_INT(n)                   (1 << (n))

/* GPIO set/clear/sta/dir/edge/rising/falling/af registers */

#define GPIO_PIN(n)                  (1 << (n))
#define TSC_PIN_SET                  (0xf0)   /* Pins 4-7:  Used by touchscreen */

/* ADC control */

#define ADC_CTRL1_REF_SEL            (1 << 1)  /* Bit 1: Selects internal/external, 1=external */
#define ADC_CTRL1_MOD_12B            (1 << 3)  /* Bit 3: Selects 10/12-bit ADC operation, 1=12-bit */
#define ADC_CTRL1_SAMPLE_TIME_SHIFT  (4)       /* Bits 4-6: ADC conversion time in number of clock */
#define ADC_CTRL1_SAMPLE_TIME_MASK   (7 << ADC_CTRL1_SAMPLE_TIME_SHIFT)
#  define ADC_CTRL1_SAMPLE_TIME_36   (0 << ADC_CTRL1_SAMPLE_TIME_SHIFT)
#  define ADC_CTRL1_SAMPLE_TIME_44   (1 << ADC_CTRL1_SAMPLE_TIME_SHIFT)
#  define ADC_CTRL1_SAMPLE_TIME_56   (2 << ADC_CTRL1_SAMPLE_TIME_SHIFT)
#  define ADC_CTRL1_SAMPLE_TIME_64   (3 << ADC_CTRL1_SAMPLE_TIME_SHIFT)
#  define ADC_CTRL1_SAMPLE_TIME_80   (4 << ADC_CTRL1_SAMPLE_TIME_SHIFT)
#  define ADC_CTRL1_SAMPLE_TIME_96   (5 << ADC_CTRL1_SAMPLE_TIME_SHIFT)
#  define ADC_CTRL1_SAMPLE_TIME_124  (6 << ADC_CTRL1_SAMPLE_TIME_SHIFT)

/* ADC control */

#define ADC_CTRL2_ADC_FREQ_SHIFT     (0)       /* Bits 0-1: Selects the clock speed of ADC */
#define ADC_CTRL2_ADC_FREQ_MASK      (3 << ADC_CTRL2_ADC_FREQ_SHIFT)
#  define ADC_CTRL2_ADC_FREQ_1p625   (0 << ADC_CTRL2_ADC_FREQ_SHIFT) /* 00: 1.625 MHz typ. */
#  define ADC_CTRL2_ADC_FREQ_3p25    (1 << ADC_CTRL2_ADC_FREQ_SHIFT) /* 01: 3.25 MHz typ. */
#  define ADC_CTRL2_ADC_FREQ_6p5     (2 << ADC_CTRL2_ADC_FREQ_SHIFT) /* 1x: 6.5 MHz typ. */

/* To initiate ADC data acquisition */

#define ADC_CAPT_CH(n)               (1 << (n))

/* 4-wire touchscreen controller setup */

#define TSC_CTRL_EN                  (1 << 0)  /* Bit 0: Enable TSC */
#define TSC_CTRL_OP_MOD_SHIFT        (1)       /* Bits 1-3: TSC operating mode */
#define TSC_CTRL_OP_MOD_MASK         (7 << TSC_CTRL_OP_MOD_SHIFT)
#  define TSC_CTRL_OP_MOD_XYZ        (0 << TSC_CTRL_OP_MOD_SHIFT) /* 000: X, Y, Z acquisition */
#  define TSC_CTRL_OP_MOD_XY         (1 << TSC_CTRL_OP_MOD_SHIFT) /* 001: X, Y only */
#  define TSC_CTRL_OP_MOD_X          (2 << TSC_CTRL_OP_MOD_SHIFT) /* 010: X only */
#  define TSC_CTRL_OP_MOD_Y          (3 << TSC_CTRL_OP_MOD_SHIFT) /* 011: Y only */
#  define TSC_CTRL_OP_MOD_Z          (4 << TSC_CTRL_OP_MOD_SHIFT) /* 100: Z only */
#define TSC_CTRL_TRACK_SHIFT         (4)       /* Bits 4-6: Tracking index */
#define TSC_CTRL_TRACK_MASK          (7 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TRACK_NONE          (0 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TRACK_4             (1 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TRACK_8             (2 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TRACK_16            (3 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TRACK_32            (4 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TRACK_64            (5 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TRACK_92            (6 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TRACK_127           (7 << TSC_CTRL_TRACK_SHIFT)
#define TSC_CTRL_TSC_STA             (1 << 7)  /* Bit 7: TSC status. 1=touch detected */

/* Touchscreen controller configuration */

#define TSC_CFG_SETTLING_SHIFT       (0)       /* Bits 0-2: Panel driver settling time */
#define TSC_CFG_SETTLING_MASK        (7 << TSC_CFG_SETTLING_SHIFT)
#  define TSC_CFG_SETTLING_10US      (0 << TSC_CFG_SETTLING_SHIFT)
#  define TSC_CFG_SETTLING_100US     (1 << TSC_CFG_SETTLING_SHIFT)
#  define TSC_CFG_SETTLING_500US     (2 << TSC_CFG_SETTLING_SHIFT)
#  define TSC_CFG_SETTLING_1MS       (3 << TSC_CFG_SETTLING_SHIFT)
#  define TSC_CFG_SETTLING_5MS       (4 << TSC_CFG_SETTLING_SHIFT)
#  define TSC_CFG_SETTLING_10MS      (5 << TSC_CFG_SETTLING_SHIFT)
#  define TSC_CFG_SETTLING_50MS      (6 << TSC_CFG_SETTLING_SHIFT)
#  define TSC_CFG_SETTLING_100MS     (7 << TSC_CFG_SETTLING_SHIFT)
#define TSC_CFG_TOUCH_DELAY_SHIFT    (3)       /* Bits 3-5: Touch detect delay */
#define TSC_CFG_TOUCH_DELAY_MASK     (7 << TSC_CFG_TOUCH_DELAY_SHIFT)
#  define TSC_CFG_TOUCH_DELAY_10US   (0 << TSC_CFG_TOUCH_DELAY_SHIFT)
#  define TSC_CFG_TOUCH_DELAY_50US   (1 << TSC_CFG_TOUCH_DELAY_SHIFT)
#  define TSC_CFG_TOUCH_DELAY_100US  (2 << TSC_CFG_TOUCH_DELAY_SHIFT)
#  define TSC_CFG_TOUCH_DELAY_500US  (3 << TSC_CFG_TOUCH_DELAY_SHIFT)
#  define TSC_CFG_TOUCH_DELAY_1MS    (4 << TSC_CFG_TOUCH_DELAY_SHIFT)
#  define TSC_CFG_TOUCH_DELAY_5MS    (5 << TSC_CFG_TOUCH_DELAY_SHIFT)
#  define TSC_CFG_TOUCH_DELAY_10MS   (6 << TSC_CFG_TOUCH_DELAY_SHIFT)
#  define TSC_CFG_TOUCH_DELAY_50MS   (7 << TSC_CFG_TOUCH_DELAY_SHIFT)
#define TSC_CFG_AVE_CTRL_SHIFT       (6)       /* Bits 6-7: Average control */
#define TSC_CFG_AVE_CTRL_MASK        (3 << TSC_CFG_AVE_CTRL_SHIFT)
#  define TSC_CFG_AVE_CTRL_1SAMPLE   (0 << TSC_CFG_AVE_CTRL_SHIFT)
#  define TSC_CFG_AVE_CTRL_2SAMPLES  (1 << TSC_CFG_AVE_CTRL_SHIFT)
#  define TSC_CFG_AVE_CTRL_4SAMPLES  (2 << TSC_CFG_AVE_CTRL_SHIFT)
#  define TSC_CFG_AVE_CTRL_8SAMPLES  (3 << TSC_CFG_AVE_CTRL_SHIFT)

/* Current status of FIFO */

#define FIFO_STA_FIFO_RESET          (1 << 0)  /* Bit 0:  Resets FIFO. All data in FIFO are cleared */
#define FIFO_STA_FIFO_TH_TRIG        (1 << 4)  /* Bit 0: 1 = FIFO size is at or beyond threshold */
#define FIFO_STA_FIFO_EMPTY          (1 << 5)  /* Bit 0: FIFO is empty */
#define FIFO_STA_FIFO_FULL           (1 << 6)  /* Bit 0: FIFO is full */
#define FIFO_STA_FIFO_OFLOW          (1 << 7)  /* Bit 0: FIFO is overflow */

/* Touchscreen controller FRACTION_Z */

#define TSC_FRACTIONZ_MASK           0x07

/* Touchscreen controller drive I */

#define TSC_IDRIVE                   (1 << 0)  /* Bit 0: MAX current on TSC driving channel */
#  define TSC_IDRIVE_20MA            (0)       /*        20mA typical, 35mA max */
#  define TSC_IDRIVE_50MA            (1 << 0)  /*        50mA typical, 80mA max */

/* Touchscreen controller shield */

#define TSC_SHIELD_YM                (1 << 0)  /* Bit 0: Ground Y- */
#define TSC_SHIELD_YP                (1 << 1)  /* Bit 1: Ground Y+ */
#define TSC_SHIELD_XM                (1 << 2)  /* Bit 2: Ground X- */
#define TSC_SHIELD_XP                (1 << 3)  /* Bit 3: Ground X+ */

/* Temperature sensor setup */

#define TEMP_CTRL_ENABLE             (1 << 0)  /* Bit 0: Enable */
#define TEMP_CTRL_ACQ                (1 << 1)  /* Bit 1: Acquire */
#define TEMP_CTRL_ACQ_MOD            (1 << 2)  /* Bit 2: 0=once, 1=every 10MS */
#define TEMP_CTRL_THRES_EN           (1 << 3)  /* Bit 3: Threshold enable */
#define TEMP_CTRL_THRES_RANGE        (1 << 4)  /* Bit 4: temperature threshold enable, 0='>=' 1='<' */

#endif /* _STMPE811_H_ */

/** @} */
