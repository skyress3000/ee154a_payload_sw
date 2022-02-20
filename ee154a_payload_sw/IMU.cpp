#include "telemetry.h"
#include "IMU.h"
#include <SparkFunMPU9250-DMP.h>

MPU9250_DMP imu;

bool imu_init(){
  // Initialize the IMU
  bool success = (imu.begin() == INV_SUCCESS);

  // Return false if we didn't successfully initialize
  if (!success){
    return false;
  }
  // Otherwise, keep initializing the IMU
  imu.setSensors(ALL_SENSORS);
  imu.setGyroFSR(GYRO_FSR);
  imu.setAccelFSR(ACC_FSR);
  imu.setLPF(LPF_HZ);
  imu.setSampleRate(ACC_GYRO_RATE);
  imu.setCompassSampleRate(MAG_RATE);

  return true;
}

telem_point_t sample_x_accel(){
  // Read acceleration in the x-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_ACCEL);
  data.timestamp = millis();
  data.data.data_value = imu.calcAccel(imu.ax);

  return data;
}

telem_point_t sample_y_accel(){
  // Read acceleration in the y-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_ACCEL);
  data.timestamp = millis();
  data.data.data_value = imu.calcAccel(imu.ay);

  return data;
}

telem_point_t sample_z_accel(){
  // Read acceleration in the z-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_ACCEL);
  data.timestamp = millis();
  data.data.data_value = imu.calcAccel(imu.az);

  return data;
}

telem_point_t sample_x_gyro(){
  // Read gyro in the x-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_GYRO);
  data.timestamp = millis();
  data.data.data_value = imu.calcGyro(imu.gx);

  return data;
}

telem_point_t sample_y_gyro(){
  // Read gyro in the y-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_GYRO);
  data.timestamp = millis();
  data.data.data_value = imu.calcGyro(imu.gy);

  return data;
}

telem_point_t sample_z_gyro(){
  // Read gyro in the z-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_GYRO);
  data.timestamp = millis();
  data.data.data_value = imu.calcGyro(imu.gz);

  return data;
}

telem_point_t sample_x_mag(){
  // Read magnetometer in the x-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_COMPASS);
  data.timestamp = millis();
  data.data.data_value = imu.calcMag(imu.mx);

  return data;
}

telem_point_t sample_y_mag(){
  // Read magnetometer in the y-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_COMPASS);
  data.timestamp = millis();
  data.data.data_value = imu.calcMag(imu.my);

  return data;
}

telem_point_t sample_z_mag(){
  // Read magnetometer in the z-direction
  telem_point_t data;

  // Wait for data to be ready, then update
  while(!imu.dataReady()){}

  imu.update(UPDATE_COMPASS);
  data.timestamp = millis();
  data.data.data_value = imu.calcMag(imu.mz);

  return data;
}