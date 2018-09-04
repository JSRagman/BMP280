/*
 * bmp280.hpp
 *
 *  Created on: Sep 4, 2018
 *      Author: JSRagman
 *
 *  Description:
 *    Bosch BMP280 Digital Pressure Sensor and supporting data structures.
 *
 *  I2CBus:
 *    This header was tested on a BeagleBone Black. I'm sure other
 *    platform I2C busses will work just as well.
 *
 *  NOTE:
 *    This header was written by JSRagman, who is NOT a professional
 *    programmer.  Use it, if you like, but don't stake your life on it.
 *
 *  And Another Thing:
 *    JSRagman is not associated in any way with the good people at
 *    Bosch, although sometimes he is a bit free with the drivers
 *    that are available on their GitHub site.
 */

#ifndef BMP280_HPP_
#define BMP280_HPP_


#include  <ctime>            // time_t
#include  <mutex>            // std::mutex
#include  <stdint.h>         // int16_t, uint16_t

#include "bbb-i2c.hpp"


namespace bosch_bmp280
{

/*
 * struct CalParams
 *
 * Description:
 *   A structure for BMP280 calibration parameters.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s):
 *   bmp280.hpp
 */
struct CalParams
{
	uint16_t  t1;
	 int16_t  t2;
	 int16_t  t3;

	uint16_t  p1;
	 int16_t  p2;
	 int16_t  p3;
	 int16_t  p4;
	 int16_t  p5;
	 int16_t  p6;
	 int16_t  p7;
	 int16_t  p8;
	 int16_t  p9;

	bool loaded;

	CalParams();
};

/*
 * struct TPData
 *
 * Description:
 *   A structure for recording temperature and pressure
 *   data as 32-bit integers. Time stamp thrown in for
 *   good measure.
 *
 *   Useful for:
 *     1.  raw temperature and pressure data, or
 *     2.  results from 32-bit fixed-point compensation.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s):
 *   bmp280.hpp
 */
struct TP32Data
{
	  time_t  timestamp;
	 int32_t  temperature;
	uint32_t  pressure;
};



class BMP280
{

protected:

	 bbbi2c::I2CBus*  i2cbus;
	uint8_t           i2caddr;

	 int32_t    tfine;
	 CalParams  cparams;

public:

	std::mutex mtx;

	BMP280 ( bbbi2c::I2CBus* bus, uint8_t addr );
	~BMP280 ();

	TP32Data  GetUncompData ();

	void      LoadCalParams ();
	int32_t   Comp32FixedTemp  (  int32_t unctemp  );
	uint32_t  Comp32FixedPress ( uint32_t uncpress );

	TP32Data  GetComp32FixedData ();

	void  GetRegs   ( uint8_t startaddr, uint8_t* data, int len );
	void  SetRegs   ( uint8_t* data, int len );
	void  SetConfig ( int preset );
	void  SetConfig ( uint8_t ctrl, uint8_t conf );

	void  Reset ();

}; // class BMP280


} // namespace bosch_bmp280b



#endif /* BMP280_HPP_ */
