/*
 * bmp280_data.hpp
 *
 *  Created on: Sep 6, 2018
 *      Author: JSRagman
 *
 *  Description:
 *    Data structures for use with the BMP280.
 *
 *  Disclaimer:
 *    This header was written by JSRagman, who is NOT a professional
 *    programmer.  Use it, if you like, but don't stake your life on it.
 *
 *  And Another Thing:
 *    JSRagman is not associated in any way with the good people at
 *    Bosch, although sometimes he is a bit free with the drivers
 *    that are available on their GitHub site.
 */

#ifndef BMP280_DATA_HPP_
#define BMP280_DATA_HPP_


#include <ctime>             // time_t
#include <deque>             // deque
#include <stdint.h>          // int32_t, uint32_t

namespace bosch_bmp280
{

/*
 * struct TP32Data
 *
 * Description:
 *   A structure for recording temperature and pressure
 *   data as 32-bit integers.
 *
 *   Useful for:
 *     1.  raw temperature and pressure data, or
 *     2.  results from 32-bit fixed-point compensation.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s):
 *   bmp280_data.hpp
 */
struct TP32Data
{
	 time_t   timestamp;
	 int32_t  temperature;
	uint32_t  pressure;

	TP32Data ( int32_t temp=0, uint32_t press=0 );
};

/*
 * struct TP32DataSummary
 *
 * Description:
 *   Intended to summarize a set of temperature or pressure
 *   readings over a given period of time.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s):
 *   bmp280_data.hpp
 */
struct TP32DataSummary
{
	time_t   timestart;
	time_t   timestop;
	uint32_t samplecount;

	uint32_t high;
	uint32_t low;
	uint32_t average;
};

/*
 * class TP32DataQueue
 *
 * Description:
 *   Intended to accumulate temperature and pressure readings as
 *   they are taken from the sensor.
 *
 *   New readings are pushed to the back of the queue, which
 *   increases in size until the stated capacity is reached.
 *   At this point, one reading is removed from the front of
 *   the queue for each new reading that is pushed.
 *
 *   In this way, the queue represents a moving time interval
 *   of fixed size. The actual interval is determined by the
 *   number of readings and the frequency at which they are
 *   pushed.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s):
 *   bmp280_data.hpp
 */
class TP32DataQueue
{

  protected:
    std::deque<TP32Data> dq;
    unsigned int qcap;

  public:
    TP32DataQueue ( int capacity=60 );

    TP32Data back  ();
  	TP32Data front ();
    TP32Data pop   ();
    int      push  ( TP32Data tpdata );

    int      capacity ();
    void     clear ();
    bool     full  ();
    int      size  ();
    time_t   timestart ();
    time_t   timestop  ();

    TP32DataSummary TemperatureSummary();
    TP32DataSummary PressureSummary();
  
}; // class TP32DataQueue

} // namespace bosch_bmp280

#endif /* BMP280_DATA_HPP_ */
