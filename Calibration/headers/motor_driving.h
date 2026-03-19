#ifndef MOTOR_DRIVING_H
#define MOTOR_DRIVING_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Motor PWM Pin Assignments (from architecture)
#define LEFT_MOTOR_PWM_PIN 20   // GP20 - Left motor speed
#define RIGHT_MOTOR_PWM_PIN 21  // GP21 - Right motor speed (was GP22, using 21 for consistency)
#define RIGHT_MOTOR_PWM_PIN_ALT 28  // GP28 is ADC2 but used as PWM for right motor

// Motor enable pins (hardwired to 3.3V, so no need to control)
// L_EN and R_EN are tied to 3.3V on the IBT-2 drivers

class MotorDriver {
private:
    // PWM slice numbers (calculated at init)
    uint leftSlice;
    uint rightSlice;

public:
    MotorDriver() : leftSlice(0), rightSlice(0) {}

    // Initialize PWM for both motors
    void init();

    // Drive motors with PWM values (-255 to +255, where 0 = stopped)
    // Positive = forward, negative = backward
    void drive(int leftSpeed, int rightSpeed);

    // Stop both motors
    void stop();

    // Helper movement functions for convenient control
    void move_forward(int speed);
    void move_backward(int speed);
    void turn_left(int speed);
    void turn_right(int speed);

    // Demo function to test robot movements
    void demo_movements(int speed);
};

#endif
