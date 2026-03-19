#ifndef MOTOR_DRIVING_H
#define MOTOR_DRIVING_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

<<<<<<< HEAD
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
=======
// Team Endurance Architecture: 4 Pins for 2 IBT-2 Drivers
#define LEFT_MOTOR_L_PWM 20   // Left Motor Forward
#define LEFT_MOTOR_R_PWM 21   // Left Motor Reverse
#define RIGHT_MOTOR_L_PWM 22  // Right Motor Forward
#define RIGHT_MOTOR_R_PWM 28  // Right Motor Reverse (ADC2 used as digital PWM)

class MotorDriver {
private:
    uint slice_L_F, slice_L_R;
    uint slice_R_F, slice_R_R;

public:
    MotorDriver() {}

    void init();
    void drive(int leftSpeed, int rightSpeed);
    void stop();
    
    // Helpers
>>>>>>> local-changes
    void move_forward(int speed);
    void move_backward(int speed);
    void turn_left(int speed);
    void turn_right(int speed);
<<<<<<< HEAD

    // Demo function to test robot movements
    void demo_movements(int speed);
};

#endif
=======
    void demo_movements(int speed);
};

#endif
>>>>>>> local-changes
