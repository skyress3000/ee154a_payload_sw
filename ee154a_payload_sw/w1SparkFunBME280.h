/******************************************************************************
SparkFunBME280.h
BME280 Arduino and Teensy Driver
Marshall Taylor @ SparkFun Electronics
May 20, 2015
https://github.com/sparkfun/BME280_Breakout

Resources:
Uses Wire.h for i2c operation
Uses SPI.h for SPI operation

Development environment specifics:
Arduino IDE 1.6.4
Teensy loader 1.23

This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@sparkfun.com.
Distributed as-is; no warranty is given.

TODO:
	roll library ver to 2.0
	remove hard wire.
	write escalating examples


******************************************************************************/

// Test derived class for base class SparkFunIMU
#ifndef __W1BME280_H__
#define __W1BME280_H__

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include <SPI.h>

//Uncomment the following line to enable software I2C
//You will need to have the SoftwareWire library installed
//#include <SoftwareWire.h> //SoftwareWire by Testato. Installed from library manager.

#define I2C_MODE 0
#define SPI_MODE 1

#ifndef w1BME280_SPI_CLOCK
#ifdef ARDUINO_ARCH_ESP32
#define w1BME280_SPI_CLOCK 1000000
#else
#define w1BME280_SPI_CLOCK 500000
#endif
#endif

#ifndef w1BME280_SPI_MODE
#define w1BME280_SPI_MODE SPI_MODE0
#endif

#define NO_WIRE 0
#define HARD_WIRE 1
#define SOFT_WIRE 2

#define MODE_SLEEP 0b00
#define MODE_FORCED 0b01
#define MODE_NORMAL 0b11

//Register names:
#define w1BME280_DIG_T1_LSB_REG			0x88
#define w1BME280_DIG_T1_MSB_REG			0x89
#define w1BME280_DIG_T2_LSB_REG			0x8A
#define w1BME280_DIG_T2_MSB_REG			0x8B
#define w1BME280_DIG_T3_LSB_REG			0x8C
#define w1BME280_DIG_T3_MSB_REG			0x8D
#define w1BME280_DIG_P1_LSB_REG			0x8E
#define w1BME280_DIG_P1_MSB_REG			0x8F
#define w1BME280_DIG_P2_LSB_REG			0x90
#define w1BME280_DIG_P2_MSB_REG			0x91
#define w1BME280_DIG_P3_LSB_REG			0x92
#define w1BME280_DIG_P3_MSB_REG			0x93
#define w1BME280_DIG_P4_LSB_REG			0x94
#define w1BME280_DIG_P4_MSB_REG			0x95
#define w1BME280_DIG_P5_LSB_REG			0x96
#define w1BME280_DIG_P5_MSB_REG			0x97
#define w1BME280_DIG_P6_LSB_REG			0x98
#define w1BME280_DIG_P6_MSB_REG			0x99
#define w1BME280_DIG_P7_LSB_REG			0x9A
#define w1BME280_DIG_P7_MSB_REG			0x9B
#define w1BME280_DIG_P8_LSB_REG			0x9C
#define w1BME280_DIG_P8_MSB_REG			0x9D
#define w1BME280_DIG_P9_LSB_REG			0x9E
#define w1BME280_DIG_P9_MSB_REG			0x9F
#define w1BME280_DIG_H1_REG				0xA1
#define w1BME280_CHIP_ID_REG				0xD0 //Chip ID
#define w1BME280_RST_REG					0xE0 //Softreset Reg
#define w1BME280_DIG_H2_LSB_REG			0xE1
#define w1BME280_DIG_H2_MSB_REG			0xE2
#define w1BME280_DIG_H3_REG				0xE3
#define w1BME280_DIG_H4_MSB_REG			0xE4
#define w1BME280_DIG_H4_LSB_REG			0xE5
#define w1BME280_DIG_H5_MSB_REG			0xE6
#define w1BME280_DIG_H6_REG				0xE7
#define w1BME280_CTRL_HUMIDITY_REG		0xF2 //Ctrl Humidity Reg
#define w1BME280_STAT_REG					0xF3 //Status Reg
#define w1BME280_CTRL_MEAS_REG			0xF4 //Ctrl Measure Reg
#define w1BME280_CONFIG_REG				0xF5 //Configuration Reg
#define w1BME280_MEASUREMENTS_REG			0xF7 //Measurements register start
#define w1BME280_PRESSURE_MSB_REG			0xF7 //Pressure MSB
#define w1BME280_PRESSURE_LSB_REG			0xF8 //Pressure LSB
#define w1BME280_PRESSURE_XLSB_REG		0xF9 //Pressure XLSB
#define w1BME280_TEMPERATURE_MSB_REG		0xFA //Temperature MSB
#define w1BME280_TEMPERATURE_LSB_REG		0xFB //Temperature LSB
#define w1BME280_TEMPERATURE_XLSB_REG		0xFC //Temperature XLSB
#define w1BME280_HUMIDITY_MSB_REG			0xFD //Humidity MSB
#define w1BME280_HUMIDITY_LSB_REG			0xFE //Humidity LSB

//Class w1BME280_SensorSettings.  This object is used to hold settings data.  The application
//uses this classes' data directly.  The settings are adopted and sent to the sensor
//at special times, such as .begin.  Some are used for doing math.
//
//This is a kind of bloated way to do this.  The trade-off is that the user doesn't
//need to deal with #defines or enums with bizarre names.
//
//A power user would strip out w1BME280_SensorSettings entirely, and send specific read and
//write command directly to the IC. (ST #defines below)
//
struct w1BME280_SensorSettings
{
  public:
	
	//Main Interface and mode settings
    uint8_t commInterface;
    uint8_t I2CAddress;
    uint8_t chipSelectPin;
	SPISettings spiSettings{w1BME280_SPI_CLOCK, MSBFIRST, w1BME280_SPI_MODE};

	//Deprecated settings
	uint8_t runMode;
	uint8_t tStandby;
	uint8_t filter;
	uint8_t tempOverSample;
	uint8_t pressOverSample;
	uint8_t humidOverSample;
    float tempCorrection; // correction of temperature - added to the result
};

//Used to hold the calibration constants.  These are used
//by the driver as measurements are being taking
struct SensorCalibration
{
  public:
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
	
	uint8_t dig_H1;
	int16_t dig_H2;
	uint8_t dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;
	int8_t dig_H6;
	
};

struct w1BME280_SensorMeasurements
{
  public:
	float temperature;
	float pressure;
	float humidity;
};

//This is the main operational class of the driver.

class w1BME280
{
  public:
    //settings
    w1BME280_SensorSettings settings;
	SensorCalibration calibration;
	int32_t t_fine;
	
	//Constructor generates default BME280_SensorSettings.
	//(over-ride after construction if desired)
    w1BME280( void );
    //~w1BME280() = default;
	
	//Call to apply BME280_SensorSettings.
	//This also gets the SensorCalibration constants
    uint8_t begin( void );
    bool beginSPI(uint8_t csPin); //Communicate using SPI
    bool beginI2C(TwoWire &wirePort = Wire1); //Called when user provides Wire port
    
	#ifdef SoftwareWire_h
	bool beginI2C(SoftwareWire &wirePort); //Called when user provides a softwareWire port
	#endif

	uint8_t getMode(void); //Get the current mode: sleep, forced, or normal
	void setMode(uint8_t mode); //Set the current mode

	void setTempOverSample(uint8_t overSampleAmount); //Set the temperature sample mode
	void setPressureOverSample(uint8_t overSampleAmount); //Set the pressure sample mode
	void setHumidityOverSample(uint8_t overSampleAmount); //Set the humidity sample mode
	void setStandbyTime(uint8_t timeSetting); //Set the standby time between measurements
	void setFilter(uint8_t filterSetting); //Set the filter
	
	void setI2CAddress(uint8_t i2caddress); //Set the address the library should use to communicate. Use if address jumper is closed (0x76).

	void setReferencePressure(float refPressure); //Allows user to set local sea level reference pressure
	float getReferencePressure();
	
	bool isMeasuring(void); //Returns true while the device is taking measurement
	
	//Software reset routine
	void reset( void );
		void readAllMeasurements(w1BME280_SensorMeasurements *measurements, byte tempScale = 0);
	
    //Returns the values as floats.
    float readFloatPressure( void );
	float readFloatAltitudeMeters( void );
	float readFloatAltitudeFeet( void );
		void readFloatPressureFromBurst(uint8_t buffer[], w1BME280_SensorMeasurements *measurements);
	
	float readFloatHumidity( void );
		void readFloatHumidityFromBurst(uint8_t buffer[], w1BME280_SensorMeasurements *measurements);

    //Temperature related methods
    void setTemperatureCorrection(float corr);
    float readTempC( void );
    float readTempF( void );
		float readTempFromBurst(uint8_t buffer[]);

	//Dewpoint related methods
	//From Pavel-Sayekat: https://github.com/sparkfun/SparkFun_BME280_Breakout_Board/pull/6/files
    double dewPointC(void);
    double dewPointF(void);
	
    //The following utilities read and write

	//ReadRegisterRegion takes a uint8 array address as input and reads
	//a chunk of memory into that array.
    void readRegisterRegion(uint8_t*, uint8_t, uint8_t );
	//readRegister reads one register
    uint8_t readRegister(uint8_t);
    //Reads two regs, LSByte then MSByte order, and concatenates them
	//Used for two-byte reads
	int16_t readRegisterInt16( uint8_t offset );
	//Writes a byte;
    void writeRegister(uint8_t, uint8_t);

private:
	uint8_t checkSampleValue(uint8_t userValue); //Checks for valid over sample values
	void readTempCFromBurst(uint8_t buffer[], w1BME280_SensorMeasurements *measurements);
	void readTempFFromBurst(uint8_t buffer[], w1BME280_SensorMeasurements *measurements);

    uint8_t _wireType = HARD_WIRE; //Default to Wire.h
    TwoWire *_hardPort = NO_WIRE; //The generic connection to user's chosen I2C hardware
    
	#ifdef SoftwareWire_h
	SoftwareWire *_softPort = NO_WIRE; //Or, the generic connection to software wire port
	#endif
	
	float _referencePressure = 101325.0; //Default but is changeable
};

#endif  // End of __BME280_H__ definition check
