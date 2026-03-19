#ifndef RAYKHA_S8_H
#define RAYKHA_S8_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <cstdint>

class RaykhaS8 {
private:
    bool detectHigh;                          // true = white line, false = black line
    static constexpr uint32_t base_pin = 6;  // GP6 is the first pin
    static constexpr uint32_t pin_mask = 0xFF << 6;  // GP6-GP13 mask (8 pins)

public:
    // Constructor
    RaykhaS8(bool detectHighLine);

    // Initialize all 8 GPIO pins
    void init();

    // Read raw 8-bit sensor data
    uint8_t readRaw();

    // Get weighted error (-4.0 to +4.0, where 0.0 = centered on line)
    // Returns 999.0f if no line detected
    float getError();
};

#endif
