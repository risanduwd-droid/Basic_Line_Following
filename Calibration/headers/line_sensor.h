#ifndef LINE_SENSOR_H
#define LINE_SENSOR_H

#include "pico/stdlib.h"
#include <cmath>

// Convert 5V analog reading to distance in cm (Sharp IR sensor)
// voltage: 0.0 to 3.3V (after voltage divider)
inline float sharpIRToCm(float voltage) {
    if (voltage < 0.1f) return 80.0f;  // Out of range (too far)
    return 27.86f * powf(voltage, -1.15f);
}

#endif
