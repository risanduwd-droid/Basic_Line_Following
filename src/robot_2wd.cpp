#include <stdio.h>
#include <stdlib.h> // For abs()
#include "headers/motor_driving.h"

void MotorDriver::init() {
    uint pins[] = {LEFT_MOTOR_L_PWM, LEFT_MOTOR_R_PWM, RIGHT_MOTOR_L_PWM, RIGHT_MOTOR_R_PWM};
    
    // Setup all 4 pins for PWM
    for(int i=0; i<4; i++) {
        gpio_set_function(pins[i], GPIO_FUNC_PWM);
    }

    slice_L_F = pwm_gpio_to_slice_num(LEFT_MOTOR_L_PWM);
    slice_L_R = pwm_gpio_to_slice_num(LEFT_MOTOR_R_PWM);
    slice_R_F = pwm_gpio_to_slice_num(RIGHT_MOTOR_L_PWM);
    slice_R_R = pwm_gpio_to_slice_num(RIGHT_MOTOR_R_PWM);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 100.0f); 

    pwm_init(slice_L_F, &config, true);
    pwm_init(slice_L_R, &config, true);
    pwm_init(slice_R_F, &config, true);
    pwm_init(slice_R_R, &config, true);

    // Set wrap to 62500 for ~20kHz frequency
    pwm_set_wrap(slice_L_F, 62500);
    pwm_set_wrap(slice_L_R, 62500);
    pwm_set_wrap(slice_R_F, 62500);
    pwm_set_wrap(slice_R_R, 62500);
    
    stop();
}

void MotorDriver::drive(int leftSpeed, int rightSpeed) {
    if (leftSpeed > 255) leftSpeed = 255;
    if (leftSpeed < -255) leftSpeed = -255;
    if (rightSpeed > 255) rightSpeed = 255;
    if (rightSpeed < -255) rightSpeed = -255;

    uint32_t leftPWM = (abs(leftSpeed) * 62500) / 255;
    uint32_t rightPWM = (abs(rightSpeed) * 62500) / 255;

    // Left Motor
    if (leftSpeed >= 0) {
        pwm_set_gpio_level(LEFT_MOTOR_L_PWM, leftPWM);
        pwm_set_gpio_level(LEFT_MOTOR_R_PWM, 0);
    } else {
        pwm_set_gpio_level(LEFT_MOTOR_L_PWM, 0);
        pwm_set_gpio_level(LEFT_MOTOR_R_PWM, leftPWM);
    }

    // Right Motor
    if (rightSpeed >= 0) {
        pwm_set_gpio_level(RIGHT_MOTOR_L_PWM, rightPWM);
        pwm_set_gpio_level(RIGHT_MOTOR_R_PWM, 0);
    } else {
        pwm_set_gpio_level(RIGHT_MOTOR_L_PWM, 0);
        pwm_set_gpio_level(RIGHT_MOTOR_R_PWM, rightPWM);
    }
}

void MotorDriver::stop() {
    drive(0, 0);
}

// --- HERE ARE THE MISSING HELPER FUNCTIONS! ---
void MotorDriver::move_forward(int speed) { drive(speed, speed); }
void MotorDriver::move_backward(int speed) { drive(-speed, -speed); }
void MotorDriver::turn_left(int speed) { drive(-speed, speed); }
void MotorDriver::turn_right(int speed) { drive(speed, -speed); }

void MotorDriver::demo_movements(int speed) {
    move_forward(speed);
    sleep_ms(2000);
    stop();
    sleep_ms(1000);
    turn_left(speed);
    sleep_ms(2000);
    stop();
}
