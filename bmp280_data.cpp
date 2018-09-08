```
/*
 * bmp280_data.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: JSRagman
 *
 *  Description:
 *    Implements data structures for use with the BMP280.
 */


#include <ctime>             // time_t, time()
#include <stdexcept>         // runtime_error
#include <stdint.h>          // int32_t, uint32_t

#include "bmp280.hpp"

using namespace std;

namespace bosch_bmp280
{

// Calibration Parameters
// -----------------------------------------------------------------

/*
 * CalParams::CalParams()
 *
 * Description:
 *   Constructor. Initializes all parameters to zero,
 *   loaded = false.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
CalParams::CalParams()
{
    t1 = 0;
    t2 = 0;
    t3 = 0;

    p1 = 0;
    p2 = 0;
    p3 = 0;
    p4 = 0;
    p5 = 0;
    p6 = 0;
    p7 = 0;
    p8 = 0;
    p9 = 0;

    loaded = false;
}


// TP32Data
// -----------------------------------------------------------------

/*
 * TP32Data::TP32Data( int32_t temp, uint32_t press )
 *
 * Description:
 *   Constructor. Sets the timestamp, along with temperature and
 *   pressure values.
 *
 * Parameters:
 *   temp  - optional. Temperature value. The default value is zero.
 *   press - optional. Pressure value.    The default value is zero.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32Data::TP32Data( int32_t temp, uint32_t press )
{
    timestamp   = time(nullptr);
    temperature = temp;
    pressure    = press;
}


// TP32DataQueue
// -----------------------------------------------------------------

/*
 * TP32DataQueue::TP32DataQueue(int capacity)
 *
 * Description:
 *   Constructor. Sets the queue maximum capacity.
 *   Initializes summary data.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32DataQueue::TP32DataQueue(int capacity)
{
    qcap = capacity;

    t_high = INT32_MIN;
    t_low  = INT32_MAX;
    t_avg  = 0.0;
    p_high = 0;
    p_low  = UINT32_MAX;
    p_avg  = 0.0;

    stale = true;
}


/*
 * TP32Data TP32DataQueue::back()
 *
 * Description:
 *   Retrieves a reference to the most recent TP32Data object that
 *   was pushed to the queue, but does not remove it from the queue.
 *
 *   Throws a runtime_error exception if the queue is empty.
 *
 * Returns:
 *   Returns a reference to the most recent TP32Data object that was
 *   pushed to the queue.
 *
 * Exceptions:
 *   runtime_error
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32Data TP32DataQueue::back()
{
    if (dq.size() < 1)
    {
        runtime_error re {"TP32DataQueue::back(): The queue is empty."};
        throw re;
    }

    return dq.back();
}

/*
 * TP32Data TP32DataQueue::front()
 *
 * Description:
 *   Retrieves a reference to the TP32Data at the front of the queue
 *   (i.e., data that has been in the queue the longest), but does
 *   not remove it from the queue.
 *
 *   Throws a runtime_error exception if the queue is empty.
 *
 * Returns:
 *   Returns a reference to the TP32Data object at the front of
 *   the queue.
 *
 * Exceptions:
 *   runtime_error
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32Data TP32DataQueue::front()
{
    if (dq.size() < 1)
    {
        runtime_error re {"TP32DataQueue::front(): The queue is empty."};
        throw re;
    }

    return dq.front();
}

/*
 * TP32Data TP32DataQueue::pop()
 *
 * Description:
 *   Retrieves the TP32Data object from the front of the queue.
 *   This data is then removed from the queue.
 *
 *   Throws a runtime_error exception if the queue is empty.
 *
 * Returns:
 *   Returns the TP32Data object at the front of the queue.
 *
 * Exceptions:
 *   runtime_error
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32Data TP32DataQueue::pop()
{
    if (dq.size() < 1)
    {
        runtime_error re {"TP32DataQueue::pop(): The queue is empty."};
        throw re;
    }

    TP32Data tpd{ dq.front() };
    dq.pop_front();
    stale = true;

    return tpd;
}

/*
 * int TP32DataQueue::push(TP32Data tpd)
 *
 * Description:
 *   Adds a TP32Data object to the back of the queue.
 *
 *   If this causes the queue's size to exceed its configured capacity,
 *   data is removed from the front until size equals capacity.
 *
 * Parameters:
 *   tpd - A TP32Data object to be added to the back of the queue.
 *
 * Returns:
 *   Returns the resulting size of the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
int TP32DataQueue::push(TP32Data tpd)
{
    dq.push_back(tpd);
    while (dq.size() > qcap)
    {
        dq.pop_front();
    }

    stale = true;

    return dq.size();
}

/*
 * int TP32DataQueue::capacity()
 *
 * Description:
 *   Returns the configured capacity of the queue.
 *
 * Returns:
 *   Returns the configured capacity of the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
int TP32DataQueue::capacity()
{
    return qcap;
}

/*
 * void TP32DataQueue::clear()
 *
 * Description:
 *   Removes all elements from the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
void TP32DataQueue::clear()
{
    dq.clear();
    stale = true;
}

/*
 * bool TP32DataQueue::full()
 *
 * Description:
 *   Indicates whether queue size has reached its configured capacity.
 *
 * Returns:
 *   Returns true if queue size equals or exceeds its configured
 *   capacity.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
bool TP32DataQueue::full()
{
    return (dq.size() >= qcap);
}

/*
 * int TP32DataQueue::size()
 *
 * Description:
 *   Returns the current size of the queue.
 *
 * Returns:
 *   Returns the current size of the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
int TP32DataQueue::size()
{
    return dq.size();
}

/*
 * void TP32DataQueue::summarize()
 *
 * Description:
 *   Re-calculates temperature and pressure summaries.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
void TP32DataQueue::summarize()
{
    t_high = INT32_MIN;
    t_low  = INT32_MAX;
    p_high = 0;
    p_low  = UINT32_MAX;

    t_avg = 0.0;
    p_avg = 0.0;

    if (dq.size() > 0)
    {
         int32_t tsum = 0;
        uint32_t psum = 0;
        for (auto it = dq.cbegin(); it != dq.cend(); ++it)
        {
             int32_t temp  = ((TP32Data)*it).temperature;
            uint32_t press = ((TP32Data)*it).pressure;

            tsum += temp;
            if (temp > t_high)  t_high = temp;
            if (temp < t_low)   t_low  = temp;

            psum += press;
            if (press > p_high) p_high = press;
            if (press < p_low)  p_low  = press;
        }

        t_avg = (double)tsum/dq.size();
        p_avg = (double)psum/dq.size();

        stale = false;
    }
}

/*
 * time_t TP32DataQueue::timestart()
 *
 * Description:
 *   Retrieves the time stamp from the front of the queue.
 *
 * Exceptions:
 *   runtime_error
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
time_t TP32DataQueue::timestart()
{
    return this->front().timestamp;
}

/*
 * time_t TP32DataQueue::timestop()
 *
 * Description:
 *   Retrieves the time stamp from the back of the queue.
 *
 * Exceptions:
 *   runtime_error
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
time_t TP32DataQueue::timestop()
{
    return this->back().timestamp;
}

/*
 * int32_t TP32DataQueue::temperature_high()
 *
 * Description:
 *   Re-calculates summary data, if necessary, and returns
 *   the highest temperature reading from the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
int32_t TP32DataQueue::temperature_high()
{
    if (stale)
        this->summarize();

    return t_high;
}

/*
 * int32_t TP32DataQueue::temperature_low()
 *
 * Description:
 *   Re-calculates summary data, if necessary, and returns
 *   the lowest temperature reading from the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
int32_t TP32DataQueue::temperature_low()
{
    if (stale)
        this->summarize();

    return t_low;
}

/*
 * double TP32DataQueue::temperature_average()
 *
 * Description:
 *   Re-calculates summary data, if necessary, and returns
 *   the average of all temperature readings in the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
double TP32DataQueue::temperature_average()
{
    if (stale)
        this->summarize();

    return t_avg;
}

/*
 * uint32_t TP32DataQueue::pressure_high()
 *
 * Description:
 *   Re-calculates summary data, if necessary, and returns
 *   the highest pressure reading from the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
uint32_t TP32DataQueue::pressure_high()
{
    if (stale)
        this->summarize();

    return p_high;
}

/*
 * uint32_t TP32DataQueue::pressure_low()
 *
 * Description:
 *   Re-calculates summary data, if necessary, and returns
 *   the lowest pressure reading from the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
uint32_t TP32DataQueue::pressure_low()
{
    if (stale)
        this->summarize();

    return p_low;
}

/*
 * double TP32DataQueue::pressure_average()
 *
 * Description:
 *   Re-calculates summary data, if necessary, and returns
 *   the average of all pressure readings in the queue.
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
double TP32DataQueue::pressure_average()
{
    if (stale)
        this->summarize();

    return p_avg;
}

/*
 * TP32TemperatureSummary TP32DataQueue::TemperatureSummary()
 *
 * Description:
 *   Retrieves summary data for all temperature readings that
 *   are in the queue.
 *
 *   Throws a runtime_error exception if the queue is empty.
 *
 * Returns:
 *   Returns a TP32TemperatureSummary object.
 *
 * Exceptions:
 *   runtime_error
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32TemperatureSummary TP32DataQueue::TemperatureSummary()
{
    if (stale)
        this->summarize();

    TP32TemperatureSummary tsummary;
    tsummary.timestart   = this->front().timestamp;
    tsummary.timestop    = this->back().timestamp;
    tsummary.samplecount = dq.size();

    tsummary.high    = t_high;
    tsummary.low     = t_low;
    tsummary.average = t_avg;

    return tsummary;
}

/*
 * TP32PressureSummary TP32DataQueue::PressureSummary()
 *
 * Description:
 *   Retrieves summary data for all pressure readings that
 *   are in the queue.
 *
 *   Throws a runtime_error exception if the queue is empty.
 *
 * Returns:
 *   Returns a TP32PressureSummary object.
 *
 * Exceptions:
 *   runtime_error
 *
 * Namespace:
 *   bosch_bmp280
 *
 * Header File(s);
 *   bmp280.hpp
 */
TP32PressureSummary TP32DataQueue::PressureSummary()
{
    if (stale)
        this->summarize();

    TP32PressureSummary psummary;
    psummary.timestart   = this->front().timestamp;
    psummary.timestop    = this->back().timestamp;
    psummary.samplecount = dq.size();

    psummary.high    = p_high;
    psummary.low     = p_low;
    psummary.average = p_avg;

    return psummary;
}

} // namespace bosch_bmp280
```
