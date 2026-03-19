#ifndef MOTOR_DRIVING_H
#define MOTOR_DRIVING_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

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
    void move_forward(int speed);
    void move_backward(int speed);
    void turn_left(int speed);
    void turn_right(int speed);
    void demo_movements(int speed);
};

#endif