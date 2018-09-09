```
/*
 * bmp280.cpp
 *
 *  Created on: Sep 4, 2018
 *      Author: JSRagman
 *
 *  Description:
 *    Bosch Sensortec BMP280 Digital Pressure Sensor and associated
 *    data structures.
 *
 *  Note:
 *    Functions dealing with temperature and pressure compensation
 *    are located in a separate source file. Seriously. It's better
 *    this way.
 *
 *  And Another Thing:
 *    JSRagman is not associated in any way with the good people at
 *    Bosch, although sometimes he is a bit free with the drivers
 *    that are available on their GitHub site.
 */

#include <ctime>             // time_t, time()
#include <unistd.h>          // usleep()

#include "bmp280.hpp"        // BMP280


using namespace std;

namespace bosch_bmp280
{


// BMP280 Constructor, Destructor
// -----------------------------------------------------------------

/*
 * BMP280::BMP280(I2CBus* bus, uint8_t addr)
 *
 * Description:
 *   Constructor. Assigns the I2C bus and the target device address.
 *   Initializes temperature compensation variable tfine to zero.
 *
 * Parameters:
 *   bus  - pointer to an I2CBus object
 *   addr - I2C address of the target device
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s):
 *   bmp280.hpp
 */
BMP280::BMP280(bbbi2c::I2CBus* bus, uint8_t addr)
{
	i2cbus  = bus;
	i2caddr = addr;
	tfine   = 0;
}

/*
 * BMP280::~BMP280()
 *
 * Description:
 *   Destructor. Move along. Nothing to see here.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s):
 *   bmp280.hpp
 */
BMP280::~BMP280()
{ }


// BMP280 Public
// -----------------------------------------------------------------

/*
 * void BMP280::LoadCalParams()
 *
 * Description:
 *   Loads calibration parameters from the device ROM.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
void BMP280::LoadCalParams()
{
	uint8_t dat[BMP280_CAL_SIZE] {0};

	this->GetRegs(BMP280_CAL_START, dat, BMP280_CAL_SIZE);

	cparams.t1 = ((uint16_t)dat[BMP280_CAL_T1H_NDX] << 8) | (uint16_t)dat[BMP280_CAL_T1L_NDX];
	cparams.t2 = (( int16_t)dat[BMP280_CAL_T2H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_T2L_NDX];
	cparams.t3 = (( int16_t)dat[BMP280_CAL_T3H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_T3L_NDX];

	cparams.p1 = ((uint16_t)dat[BMP280_CAL_P1H_NDX] << 8) | (uint16_t)dat[BMP280_CAL_P1L_NDX];
	cparams.p2 = (( int16_t)dat[BMP280_CAL_P2H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_P2L_NDX];
	cparams.p3 = (( int16_t)dat[BMP280_CAL_P3H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_P3L_NDX];
	cparams.p4 = (( int16_t)dat[BMP280_CAL_P4H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_P4L_NDX];
	cparams.p5 = (( int16_t)dat[BMP280_CAL_P5H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_P5L_NDX];
	cparams.p6 = (( int16_t)dat[BMP280_CAL_P6H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_P6L_NDX];
	cparams.p7 = (( int16_t)dat[BMP280_CAL_P7H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_P7L_NDX];
	cparams.p8 = (( int16_t)dat[BMP280_CAL_P8H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_P8L_NDX];
	cparams.p9 = (( int16_t)dat[BMP280_CAL_P9H_NDX] << 8) | ( int16_t)dat[BMP280_CAL_P9L_NDX];

	cparams.loaded = true;
}

/*
 * TP32Data BMP280::GetUncompData()
 *
 * Description:
 *   Retrieves raw temperature and pressure data from the sensor.
 *
 * Returns:
 *   Returns a structure containing uncompensated temperature
 *   and pressure data along with a time stamp.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32Data BMP280::GetUncompData()
{
    TP32Data unc;
    uint8_t dat[6]{0};

    this->GetRegs(BMP280_R_PMSB, dat, 6);

    unc.pressure =
        (uint32_t) (
            (((uint32_t)dat[0]) << 12) |
            (((uint32_t)dat[1]) <<  4) |
            (((uint32_t)dat[2]) >>  4)
            );

    unc.temperature =
        (int32_t) (
            (((int32_t)dat[3]) << 12) |
            (((int32_t)dat[4]) <<  4) |
            (((int32_t)dat[5]) >>  4)
            );

    return unc;
}

/*
 * TP32Data BMP280::GetComp32FixedData()
 *
 * Description:
 *   Retrieves a temperature and pressure reading and applies
 *   32-bit fixed-point compensation.
 *
 *   Temperature compensation is performed first, in order to generate
 *   the 32-bit integer (tfine) that is required for the pressure
 *   compensation function.
 *
 * Returns:
 *   Returns a TP32Data structure containing a time stamp, a temperature
 *   reading (in 1/100 degrees centigrade), and a pressure reading (in
 *   pascals).
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32Data BMP280::GetComp32FixedData()
{
    TP32Data reading;
    TP32Data unc = this->GetUncompData();

    reading.timestamp   = unc.timestamp;
    reading.temperature = this->Comp32FixedTemp(unc.temperature);
    reading.pressure    = this->Comp32FixedPress(unc.pressure);

    return reading;
}

/*
 * void BMP280::GetRegs(uint8_t startaddr, uint8_t* data, int len)
 *
 * Description:
 *   Reads the contents of one or more consecutive device registers.
 *
 * Parameters:
 *   startaddr - address of the first register to be read.
 *   data      - pointer to a buffer that will receive data.
 *   len       - the number of bytes to read.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
void BMP280::GetRegs(uint8_t startaddr, uint8_t* data, int len)
{
    i2cbus->Xfer(startaddr, data, len, i2caddr);
}

/*
 * void BMP280::SetRegs(uint8_t* data, int len)
 *
 * Description:
 *   Writes to one or more device registers. Outgoing bytes must
 *   be organized in pairs - a register address followed by a data
 *   byte for that register.
 *
 *   This {address, data} sequence is repeated for each register
 *   to be written.
 *
 * Parameters:
 *   data - pointer to a buffer that contains data which will be
 *          written to the device.
 *   len  - the total number of bytes to be written.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
void BMP280::SetRegs(uint8_t* data, int len)
{
    i2cbus->Write(data, len, i2caddr);
}

/*
 * void BMP280::GetConfig(uint8_t& ctrl, uint8_t& conf)
 *
 * Description:
 *   Reads the device ctrl_meas and config registers.
 *
 * Parameters:
 *   ctrl - receives the contents of the ctrl_meas register.
 *   conf - receives the contents of the config register.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
void BMP280::GetConfig(uint8_t& ctrl, uint8_t& conf)
{
    uint8_t dat[2];
    this->GetRegs(BMP280_R_CTRL, dat, 2);

    ctrl = dat[0];
    conf = dat[1];
}

/*
 * void BMP280::SetConfig(uint8_t ctrl, uint8_t conf)
 *
 * Description:
 *   Resets the BMP280 to get it into Sleep mode, then writes the
 *   ctrl and conf parameters to the ctrl_meas and config registers.
 *
 *   Note that the first write to ctrl_meas is done with the mode bits
 *   masked out (ctrx). The config register is written next, followed
 *   by the ctrl_meas register, this time with the mode bits included.
 *
 *   The CONFIG_DELAY constant is currently a number that I pulled out
 *   of a hat as a reasonable amount of time before the first data
 *   read should be attempted. The BMP280 datasheet gives the precise
 *   method of determining what this time should be.
 *
 * Parameters:
 *   ctrl - 8-bits to be written to the ctrl_meas register.
 *   conf - 8-bits to be written to the config register.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
void BMP280::SetConfig(uint8_t ctrl, uint8_t conf)
{
    uint8_t ctrx = (ctrl & BMP280_MODE_MSK_OUT);
    uint8_t dat[] { BMP280_R_CTRL, ctrx, BMP280_R_CONF, conf, BMP280_R_CTRL, ctrl };

    this->Reset();
    this->SetRegs(dat, 6);
    usleep(BMP280_CONFIG_DELAY);
}

/*
 * void BMP280::SetConfig(int preset)
 *
 * Description:
 *   Sets the device to one of the six available preset configurations.
 *
 * Parameters:
 *   preset - An integer value between one and six, inclusive.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
void BMP280::SetConfig(int preset)
{
uint8_t ctrl;
uint8_t conf;

    switch (preset)
    {
      case 1:
        ctrl = BMP280_CTRL_PRE1;
        conf = BMP280_CONF_PRE1;
        break;
      case 2:
        ctrl = BMP280_CTRL_PRE2;
        conf = BMP280_CONF_PRE2;
        break;
      case 3:
        ctrl = BMP280_CTRL_PRE3;
        conf = BMP280_CONF_PRE3;
        break;
      case 4:
        ctrl = BMP280_CTRL_PRE4;
        conf = BMP280_CONF_PRE4;
        break;
      case 5:
        ctrl = BMP280_CTRL_PRE5;
        conf = BMP280_CONF_PRE5;
        break;
      case 6:
        ctrl = BMP280_CTRL_PRE6;
        conf = BMP280_CONF_PRE6;
        break;
      default:
        ctrl = BMP280_CTRL_PRE1;
        conf = BMP280_CONF_PRE1;
        break;
    }

    this->SetConfig(ctrl, conf);
}

/*
 * void BMP280::Reset()
 *
 * Description:
 *   Resets the device. Allows time for the reset process to
 *   complete before returning.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s):
 *   bmp280.hpp
 */
void BMP280::Reset()
{
    uint8_t dat[] { BMP280_R_RESET, BMP280_CMD_RESET };
    i2cbus->Write(dat, 2, i2caddr);

    usleep(BMP280_RESET_DELAY);
}

} // namespace bosch_bmp280
```
