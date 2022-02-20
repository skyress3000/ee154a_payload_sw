#include "telemetry.h"

// CONSTANTS
#define ALL_SENSORS INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS
#define GYRO_FSR 2000 // Gyro full scale range: +/- 250, 500, 1000, or 2000 dps
#define ACC_FSR  4    // Accel full scale range: +/- 2, 4, 8, or 16 g
#define LPF_HZ   98   // Can be any of the following: 188, 98, 42, 20, 10, 5
#define ACC_GYRO_RATE 100 // 4 Hz to 1 kHz
#define MAG_RATE      100 // 1 Hz to 100 Hz

// FUNCTIONS
bool imu_init();
telem_point_t sample_x_accel();        // record MPU9250 x acceleration
telem_point_t sample_y_accel();        // record MPU9250 y acceleration
telem_point_t sample_z_accel();        // record MPU9250 z acceleration
telem_point_t sample_x_gyro();         // record MPU9250 x gyro value
telem_point_t sample_y_gyro();         // record MPU9250 y gyro value
telem_point_t sample_z_gyro();         // record MPU9250 z gyro value
telem_point_t sample_x_mag();          // record MPU9250 x magnetometer value
telem_point_t sample_y_mag();          // record MPU9250 y magnetometer value
telem_point_t sample_z_mag();          // record MPU9250 z magnetometer value