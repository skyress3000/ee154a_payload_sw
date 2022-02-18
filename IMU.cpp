#include "telemetry.h"
#include "IMU.h"
#include "MPU9250.h"
#include <Wire.h>
#include <SPI.h>

#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0
#define I2Cclock 400000
#define I2Cport Wire
MPU9250 imu(MPU9250_ADDRESS, I2Cport, I2Cclock);

bool imu_init(){
  // Initialize I2C
  Wire.begin();
  // Initialize IMU
  imu.initMPU9250();
  imu.initAK8963(imu.factoryMagCalibration);
  imu.getAres();
  imu.getGres();
  imu.getMres();
  imu.magCalMPU9250(imu.magBias, imu.magScale);

  return true;
}

telem_point_t sample_x_accel(){
  // Read acceleration in the x-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readAccelData(imu.accelCount); 
  imu.ax = (float)imu.accelCount[0] * imu.aRes;
  data.timestamp = millis();
  data.data.data_value = imu.ax;

  return data;
}

telem_point_t sample_y_accel(){
  // Read acceleration in the y-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readAccelData(imu.accelCount);
  imu.ay = (float)imu.accelCount[1] * imu.aRes;
  data.timestamp = millis();
  data.data.data_value = imu.ay;

  return data;
}

telem_point_t sample_z_accel(){
  // Read acceleration in the z-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readAccelData(imu.accelCount);
  imu.az = (float)imu.accelCount[2] * imu.aRes;
  data.timestamp = millis();
  data.data.data_value = imu.az;

  return data;
}

telem_point_t sample_x_gyro(){
  // Read gyro in the x-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readGyroData(imu.gyroCount);
  imu.gx = (float)imu.gyroCount[0] * imu.gRes;
  data.timestamp = millis();
  data.data.data_value = imu.gx;

  return data;
}

telem_point_t sample_y_gyro(){
  // Read gyro in the y-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readGyroData(imu.gyroCount);
  imu.gy = (float)imu.gyroCount[1] * imu.gRes;
  data.timestamp = millis();
  data.data.data_value = imu.gy;

  return data;
}

telem_point_t sample_z_gyro(){
  // Read gyro in the z-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readGyroData(imu.gyroCount);
  imu.gz = (float)imu.gyroCount[2] * imu.gRes;
  data.timestamp = millis();
  data.data.data_value = imu.gz;

  return data;
}

telem_point_t sample_x_mag(){
  // Read magnetometer in the x-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readMagData(imu.magCount);
  imu.mx = (float)imu.magCount[0] * imu.mRes * imu.factoryMagCalibration[0] - imu.magBias[0];
  data.timestamp = millis();
  data.data.data_value = imu.mx;

  return data;
}

telem_point_t sample_y_mag(){
  // Read magnetometer in the y-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readMagData(imu.magCount);
  imu.my = (float)imu.magCount[1] * imu.mRes * imu.factoryMagCalibration[1] - imu.magBias[1];
  data.timestamp = millis();
  data.data.data_value = imu.my;

  return data;
}

telem_point_t sample_z_mag(){
  // Read magnetometer in the z-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!(imu.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)){}
  // update
  imu.readMagData(imu.magCount);
  imu.mz = (float)imu.magCount[2] * imu.mRes * imu.factoryMagCalibration[2] - imu.magBias[2];
  data.timestamp = millis();
  data.data.data_value = imu.mz;

  return data;
}