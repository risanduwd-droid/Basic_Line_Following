#include "../headers/RaykhaS8.h"

RaykhaS8::RaykhaS8(bool detectHighLine) {
    detectHigh = detectHighLine;
}

void RaykhaS8::init() {
    // Initialize all 8 pins at once using the mask
    gpio_init_mask(pin_mask);

    // Set all 8 pins to INPUT at once
    gpio_set_dir_in_masked(pin_mask);

    // Prevent floating inputs: use pull-up (common for QRE sensors when line is black)
    // Since there is no masked version for pulls, we loop through the 8 pins:
    for (int i = 0; i < 8; i++) {
        gpio_pull_up(base_pin + i); 
    }
}

uint8_t RaykhaS8::readRaw() {
    // 1. Read every single pin on the Pico (GP0 to GP29) in one CPU cycle
    uint32_t all_pins = gpio_get_all();

    // 2. Mask out everything except GP6-GP13, then shift it down so it fits neatly into 8 bits
    uint8_t sensorData = static_cast<uint8_t>((all_pins & pin_mask) >> base_pin);

    // 3. If your line is black (LOW), invert the bits so the line always shows as 1s in our math
    if (!detectHigh) {
        sensorData = static_cast<uint8_t>(~sensorData);
    }

    return sensorData;
}

float RaykhaS8::getError() {
    uint8_t raw = readRaw();

    // If no line is detected at all (00000000), return our fail-safe code
    if (raw == 0) {
        return 999.0f;
    }

    float totalPosition = 0.0f;
    int activeSensors = 0;

    // Array of weights from Left (Bit 0) to Right (Bit 7)
    // Adjust these if your sensor array is mounted backwards!
    float weights[8] = {-4.0f, -3.0f, -2.0f, -1.0f, 1.0f, 2.0f, 3.0f, 4.0f};

    // Loop through the 8 bits to calculate the weighted average
    for (int i = 0; i < 8; i++) {
        // Check if the specific bit (sensor) is a 1
        if (raw & (1u << i)) {
            totalPosition += weights[i];
            activeSensors++;
        }
    }

    // Return the averaged error (0.0 means perfectly centered)
    return totalPosition / activeSensors;
}
