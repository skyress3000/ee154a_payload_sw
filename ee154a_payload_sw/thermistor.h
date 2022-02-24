#include "telemetry.h"
#include <math.h>

#define TEMP_PIN       A2
#define N_AVG          32

const float Rref = 10000;
const float a1 = 3.35 * pow(10,-3);
const float b1 = 2.57 * pow(10,-4);
const float c1 = 2.62 * pow(10,-6);
const float d1 = 6.38 * pow(10,-8);


telem_point_t sample_temp_internal();        // record internal temperature
