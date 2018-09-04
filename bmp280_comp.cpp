/*
 * bmp280_comp.cpp
 *
 *  Created on: Sep 4, 2018
 *      Author: JSRagman
 *
 *  Description:
 *    Compensation functions for Bosch Sensortec BMP280 Digital Pressure Sensor
 *
 *  Notes:
 *    1. 32-bit fixed-point compensation.
 *    2. In the compensation functions, I picked the action apart into a ton of
 *       local variables just to get a feel for what is going on. Feel free to
 *       reverse this, if you like.
 *
 *  And Another Thing:
 *    JSRagman is not associated in any way with the good people at
 *    Bosch, although sometimes he is a bit free with the drivers
 *    that are available on their GitHub site.
 */

#include "bmp280.hpp"
#include "bmp280_defs.hpp"


namespace bosch_bmp280
{

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
 *   bmp280_defs.hpp
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
 * int32_t BMP280::Comp32FixedTemp(int32_t unctemp)
 *
 * Description:
 *   Applies 32-bit fixed-point compensation to a temperature reading.
 *   Also generates a temperature value (tfine) which is used to
 *   compensate an associated pressure reading.
 *
 *   Compensation formula derived from Bosch datasheet
 *   BST-BMP280-DS001-19, Revision 1.19, January 2018, section 8.2.
 *
 * Parameters:
 *   unctemp - an uncompensated temperature reading.
 *
 * Returns:
 *   Returns a 32-bit integer that has units of 1/100 degrees centigrade.
 *   Divide this output by 100 to get temperature in degrees centigrade.
 *
 *   tfine - When this function exits, tfine will contain a value that
 *   can be used to compensate an associated pressure reading.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
int32_t BMP280::Comp32FixedTemp(int32_t unctemp)
{
	if (!cparams.loaded) this->LoadCalParams();

	int32_t t1 = (int32_t)cparams.t1;
	int32_t t2 = (int32_t)cparams.t2;
	int32_t t3 = (int32_t)cparams.t3;

	int32_t tvar1, tvar1a;
	int32_t tvar2;
	int32_t tvar3;

    tvar1 = (((unctemp >> 3)-(t1 << 1))*t2) >> 11;
    tvar1a = ((unctemp >> 4)-t1);

    tvar2 = ( ((tvar1a*tvar1a) >> 12) * t3) >> 14;

    tfine =  tvar1 + tvar2;
    tvar3 = (tfine * 5 + 128) >> 8;

    return tvar3;
}

/*
 * uint32_t BMP280::Comp32FixedPress(uint32_t uncpress)
 *
 * Description:
 *   Applies 32-bit fixed-point compensation to a pressure reading.
 *
 *   Compensation formula derived from Bosch datasheet
 *   BST-BMP280-DS001-19, Revision 1.19, January 2018, section 8.2.
 *
 * Parameters:
 *   unpress - an uncompensated pressure reading.
 *
 * Returns:
 *   Returns barometric pressure as an unsigned 32-bit integer.
 *   Pressure units are Pa.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
uint32_t BMP280::Comp32FixedPress(uint32_t uncpress)
{
	if (!cparams.loaded) this->LoadCalParams();

	 int32_t v1, v1a;
	uint32_t v1b;
	 int32_t v2;
	uint32_t v3, v3a;
	uint32_t pressure;

	int32_t p1 = (int32_t)cparams.p1;
	int32_t p2 = (int32_t)cparams.p2;
	int32_t p3 = (int32_t)cparams.p3;
	int32_t p4 = (int32_t)cparams.p4;
	int32_t p5 = (int32_t)cparams.p5;
	int32_t p6 = (int32_t)cparams.p6;
	int32_t p7 = (int32_t)cparams.p7;
	int32_t p8 = (int32_t)cparams.p8;
	int32_t p9 = (int32_t)cparams.p9;

	v1  = (tfine >> 1) - 64000;
	v1a = (v1 >> 2)*(v1 >> 2);

	v2  = (v1a >> 11)*p6 + ((v1*p5) << 1);
	v2  = (v2 >> 2) + (p4 << 16);

	v1  = (((p3*(v1a >> 13)) >> 3) + ((p2*v1) >> 1)) >> 18;
	v1  = ((((32768 + v1))*p1) >> 15);
	v3  = ( (uint32_t)(1048576 - uncpress)-(v2 >> 12) ) * 3125;

    if (v1 != 0)
    {
    	v1b = (uint32_t)v1;
		if (v3 < 0x80000000)
			v3 = (v3 << 1)/v1b;
		else
			v3 = (v3/v1b)*2;

		v3a = (v3 >> 3)*(v3 >> 3);
		v1  = (p9 * (int32_t)(v3a >> 13)) >> 12;
		v2  = (((int32_t)(v3 >> 2)) * p8) >> 13;

		pressure = (uint32_t)( (int32_t)v3 + ((v1+v2+p7) >> 4) );
    }
    else
    {
    	pressure = 0;
    }

    return pressure;
}



}
