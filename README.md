## Bosch BMP280
### Description
C++ Classes and data structures for supporting the Bosch Sensortec BMP280 Digital Pressure Sensor.
### Platform
BeagleBone Black, Rev C, running Debian 9.3 (iot-armhf)
### Details
Currently supports only the I2C interface.
### Notes
#### Headers
As this project has been growing like a weed, I have found it necessary
to split the bmp280.hpp header into separate files to enhance clarity.

To use these headers, it is only necessary to #include "bmp280.hpp", which
will pull in all of its children with its own #includes.