/*
 * bmp280.hpp
 *
 *  Created on: Sep 4, 2018
 *      Author: JSRagman
 *
 *  Description:
 *    Bosch BMP280 Digital Pressure Sensor and associated
 *    data structures.
 *
 *  Note:
 *    This header was written by JSRagman, who is NOT a professional
 *    programmer. Use it, if you like, but don't stake your life on it.
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

#include "bbb-i2c.hpp"       // I2CBus

#include "bmp280_defs.hpp"
#include "bmp280_comp.hpp"

using bbbi2c::I2CBus;

namespace bosch_bmp280
{

class BMP280
{
	protected:
	
		I2CBus*    i2cbus;
		uint8_t    i2caddr;
		int32_t    tfine;
		CalParams  cparams;

	public:
	
		std::mutex mtx;

		BMP280 ( I2CBus* bus, uint8_t addr );
		~BMP280 ();

		TP32Data  GetUncompData ();
		TP32Data  GetComp32FixedData ();

		void      LoadCalParams ();
		int32_t   Comp32FixedTemp  (  int32_t unctemp  );
		uint32_t  Comp32FixedPress ( uint32_t uncpress );

		void  GetRegs   ( uint8_t startaddr, uint8_t* data, int len );
		void  SetRegs   ( uint8_t* data, int len );
	
		void  SetConfig ( int preset );
		void  SetConfig ( uint8_t ctrl, uint8_t conf );

		void  Reset ();

}; // class BMP280

} // namespace bosch_bmp280b

#endif /* BMP280_HPP_ */
