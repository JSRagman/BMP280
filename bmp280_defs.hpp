/*
 * bmp280_defs.hpp
 *
 *  Created on: Aug 26, 2018
 *      Author: JSRagman
 *
 *  Description:
 *    Definitions for the Bosch Sensortec BMP280 Digital Pressure Sensor.
 *
 *  NOTE:
 *    This header was written by JSRagman, who is NOT a professional
 *    programmer.  Use it, if you like, but don't stake your life on it.
 *
 *  And Another Thing:
 *    JSRagman is not associated in any way with the good people at
 *    Bosch, although sometimes he is a bit free with the drivers
 *    that are available on their GitHub site.
 *
 *    The sensor preset configurations at the bottom of this
 *    header are lifted directly from the BMP280 data sheet
 *    BST-BMP280-DS001-19 Revision 1.19 | January 2018
 */

#ifndef BMP280_DEFS_H_
#define BMP280_DEFS_H_


// Chip Properties
#define BMP280_ID     0x58
#define BMP280_I2C0   0x76
#define BMP280_I2C1   0x77


// BMP280 Register Addresses
// --------------------------------------
#define BMP280_R_ID          0xD0  // id register
#define BMP280_R_RESET       0xE0  // reset
#define BMP280_R_STAT        0xF3  // status
#define BMP280_R_CTRL        0xF4  // ctrl_meas
#define BMP280_R_CONF        0xF5  // config

#define BMP280_R_PMSB        0xF7  // pressure data
#define BMP280_R_PLSB        0xF8
#define BMP280_R_PXLSB       0xF9

#define BMP280_R_TMSB        0xFA  // temperature data
#define BMP280_R_TLSB        0xFB
#define BMP280_R_TXLSB       0xFC

// Calibration Register Addresses
#define BMP280_R_CAL_T1L     0x88
#define BMP280_R_CAL_T1H     0x89
#define BMP280_R_CAL_T2L     0x8A
#define BMP280_R_CAL_T2H     0x8B
#define BMP280_R_CAL_T3L     0x8C
#define BMP280_R_CAL_T3H     0x8D
#define BMP280_R_CAL_P1L     0x8E
#define BMP280_R_CAL_P1H     0x8F
#define BMP280_R_CAL_P2L     0x90
#define BMP280_R_CAL_P2H     0x91
#define BMP280_R_CAL_P3L     0x92
#define BMP280_R_CAL_P3H     0x93
#define BMP280_R_CAL_P4L     0x94
#define BMP280_R_CAL_P4H     0x95
#define BMP280_R_CAL_P5L     0x96
#define BMP280_R_CAL_P5H     0x97
#define BMP280_R_CAL_P6L     0x98
#define BMP280_R_CAL_P6H     0x99
#define BMP280_R_CAL_P7L     0x9A
#define BMP280_R_CAL_P7H     0x9B
#define BMP280_R_CAL_P8L     0x9C
#define BMP280_R_CAL_P8H     0x9D
#define BMP280_R_CAL_P9L     0x9E
#define BMP280_R_CAL_P9H     0x9F

// Calibration Registers, Indexed
#define BMP280_CAL_START     0x88
#define BMP280_CAL_SIZE        24

#define	BMP280_CAL_T1L_NDX    0
#define	BMP280_CAL_T1H_NDX    1
#define	BMP280_CAL_T2L_NDX    2
#define	BMP280_CAL_T2H_NDX    3
#define	BMP280_CAL_T3L_NDX    4
#define	BMP280_CAL_T3H_NDX    5
#define	BMP280_CAL_P1L_NDX    6
#define	BMP280_CAL_P1H_NDX    7
#define	BMP280_CAL_P2L_NDX    8
#define	BMP280_CAL_P2H_NDX    9
#define	BMP280_CAL_P3L_NDX   10
#define	BMP280_CAL_P3H_NDX   11
#define	BMP280_CAL_P4L_NDX   12
#define	BMP280_CAL_P4H_NDX   13
#define	BMP280_CAL_P5L_NDX   14
#define	BMP280_CAL_P5H_NDX   15
#define	BMP280_CAL_P6L_NDX   16
#define	BMP280_CAL_P6H_NDX   17
#define	BMP280_CAL_P7L_NDX   18
#define	BMP280_CAL_P7H_NDX   19
#define	BMP280_CAL_P8L_NDX   20
#define	BMP280_CAL_P8H_NDX   21
#define	BMP280_CAL_P9L_NDX   22
#define	BMP280_CAL_P9H_NDX   23


// More Useful Definitions
// --------------------------------------

// Status Register (0xF3) Mask
#define BMP280_STATUS_MSK    0x09  // 0000_1001

// Reset
#define BMP280_CMD_RESET     0xB6  // Reset command.
#define BMP280_RESET_DELAY   3000  // Reset delay, in microseconds.
#define BMP280_CONFIG_DELAY  8000  // Configuration delay, in microseconds.



// ctrl_meas Register (0xF4)
// -------------------------------
// osrs_t
#define BMP280_OS_T_MSK     0xE0    // 111_000_00
#define BMP280_OS_T_SKIP    0x00
#define BMP280_OS_T_1X      0x20
#define BMP280_OS_T_2X      0x40
#define BMP280_OS_T_4X      0x60
#define BMP280_OS_T_8X      0x80
#define BMP280_OS_T_16X     0xA0

// osrs_p
#define BMP280_OS_P_MSK     0x1C    // 000_111_00
#define BMP280_OS_P_SKIP    0x00
#define BMP280_OS_P_1X      0x04
#define BMP280_OS_P_2X      0x08
#define BMP280_OS_P_4X      0x0C
#define BMP280_OS_P_8X      0x10
#define BMP280_OS_P_16X     0x14

// mode
#define BMP280_MODE_MSK     0x03    // 000_000_11
#define BMP280_MODE_MSK_OUT 0xFC    // 111_111_00
#define BMP280_MODE_SLEEP   0x00
#define BMP280_MODE_FORCED  0x01
#define BMP280_MODE_NORMAL  0x03


// config Register (0xF5)
// -------------------------------
// t_sb
#define BMP280_T_SB_MASK    0xE0    // 111_000_00
#define BMP280_T_SB_0_5     0x00
#define BMP280_T_SB_62_5    0x20
#define BMP280_T_SB_125     0x40
#define BMP280_T_SB_250     0x60
#define BMP280_T_SB_500     0x80
#define BMP280_T_SB_1K      0xA0
#define BMP280_T_SB_2K      0xC0
#define BMP280_T_SB_4K      0xE0

// filter
#define BMP280_FILTER_MASK  0x1C    // 000_111_00
#define BMP280_FILTER_OFF   0x00
#define BMP280_FILTER_2     0x04
#define BMP280_FILTER_4     0x08
#define BMP280_FILTER_8     0x0C
#define BMP280_FILTER_16    0x10

// spi3w_en
#define BMP280_SPI3W_MASK   0x01    // 000_000_01
#define BMP280_SPI3W_NDX       0



// pre-defined configurations*
// * from Bosch Sensortec BST-BMP280-DS001-19
//   Revision 1.19 | January 2018
/* --------------------------
 *   1. hand held device (low power)
 *   2. hand held device (dynamic)
 *   3. weather monitoring
 *   4. elevator floor-change detection
 *   5. drop detection
 *   6. indoor navigation
 */

// Hand held device, low power.
//   ctrl_meas = 0x57
//   config    = 0x28
#define BMP280_CTRL_PRE1    ( BMP280_OS_T_2X   | BMP280_OS_P_16X   | BMP280_MODE_NORMAL )
#define BMP280_CONF_PRE1    ( BMP280_T_SB_62_5 | BMP280_FILTER_4                        )

// Hand held device, dynamic.
//   ctrl_meas = 0x2F
//   config    = 0x10
#define BMP280_CTRL_PRE2    ( BMP280_OS_T_1X   | BMP280_OS_P_4X    | BMP280_MODE_NORMAL )
#define BMP280_CONF_PRE2    ( BMP280_T_SB_0_5  | BMP280_FILTER_16                       )

// Weather monitoring.
//   ctrl_meas = 0x25
//   config    = 0xE0
#define BMP280_CTRL_PRE3    ( BMP280_OS_T_1X   | BMP280_OS_P_1X    | BMP280_MODE_FORCED )
#define BMP280_CONF_PRE3    ( BMP280_T_SB_4K   | BMP280_FILTER_OFF                      )

// Elevator floor-change detection.
//   ctrl_meas = 0x2F
//   config    = 0x48
#define BMP280_CTRL_PRE4    ( BMP280_OS_T_1X   | BMP280_OS_P_4X    | BMP280_MODE_NORMAL )
#define BMP280_CONF_PRE4    ( BMP280_T_SB_125  | BMP280_FILTER_4                        )

// Drop detection.
//   ctrl_meas = 0x2B
//   config    = 0x00
#define BMP280_CTRL_PRE5    ( BMP280_OS_T_1X   | BMP280_OS_P_2X    | BMP280_MODE_NORMAL )
#define BMP280_CONF_PRE5    ( BMP280_T_SB_0_5  | BMP280_FILTER_OFF                      )

// Indoor navigation.
//   ctrl_meas = 0x57
//   config    = 0x10
#define BMP280_CTRL_PRE6    ( BMP280_OS_T_2X   | BMP280_OS_P_16X   | BMP280_MODE_NORMAL )
#define BMP280_CONF_PRE6    ( BMP280_T_SB_0_5  | BMP280_FILTER_16                       )



#endif /* BMP280_DEFS_H_ */
