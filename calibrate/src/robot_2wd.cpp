/*
    *   robot_2wd.cpp
    *   Created on: Jun 6, 2024
    *   Implementation of the MotorDriver class for 2 wheel drive (2WD) robot motor controls
    *   Pasan_RK95
*/

#include <stdio.h>
#include "motor_driving.h"
#include "pico/stdlib.h"

// Initialize the MotorDriver class implementation
void MotorDriver::init() {
    // Initialize PWM for left motor on GP20
    gpio_set_function(LEFT_MOTOR_PWM_PIN, GPIO_FUNC_PWM);
    leftSlice = pwm_gpio_to_slice_num(LEFT_MOTOR_PWM_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 100.0f);  // Divide clock for proper frequency
    pwm_init(leftSlice, &config, true);

    // Initialize PWM for right motor on GP21
    gpio_set_function(RIGHT_MOTOR_PWM_PIN, GPIO_FUNC_PWM);
    rightSlice = pwm_gpio_to_slice_num(RIGHT_MOTOR_PWM_PIN);
    pwm_init(rightSlice, &config, true);

    // Set PWM frequency to ~1kHz
    pwm_set_wrap(leftSlice, 62500);   // 125MHz / 100 / 20kHz = 62.5
    pwm_set_wrap(rightSlice, 62500);

    printf("MotorDriver initialized on GP%d (left) and GP%d (right)\n", 
           LEFT_MOTOR_PWM_PIN, RIGHT_MOTOR_PWM_PIN);
}

// Drive motors with PWM values (-255 to +255, where 0 = stopped)
void MotorDriver::drive(int leftSpeed, int rightSpeed) {
    // Clang values to input range (-255 to +255)
    if (leftSpeed > 255) leftSpeed = 255;
    if (leftSpeed < -255) leftSpeed = -255;
    if (rightSpeed > 255) rightSpeed = 255;
    if (rightSpeed < -255) rightSpeed = -255;

    // Convert to PWM range (0-65535 for Pico)
    // leftSpeed: -255 to +255 -> 0 to 65535
    int leftPWM = (leftSpeed + 255) * 256 / 510;
    int rightPWM = (rightSpeed + 255) * 256 / 510;

    pwm_set_gpio_level(LEFT_MOTOR_PWM_PIN, leftPWM);
    pwm_set_gpio_level(RIGHT_MOTOR_PWM_PIN, rightPWM);
}

// Stop both motors
void MotorDriver::stop() {
    drive(0, 0);
    printf("Motors stopped\n");
}

// Helper functions for convenient movement control
void MotorDriver::move_forward(int speed) {
    printf("Moving forward at speed: %d\n", speed);
    drive(speed, speed);
}

void MotorDriver::move_backward(int speed) {
    printf("Moving backward at speed: %d\n", speed);
    drive(-speed, -speed);
}

void MotorDriver::turn_left(int speed) {
    printf("Turning left at speed: %d\n", speed);
    drive(-speed, speed);
}

void MotorDriver::turn_right(int speed) {
    printf("Turning right at speed: %d\n", speed);
    drive(speed, -speed);
}

// Demo function to test robot movements
void MotorDriver::demo_movements(int speed) {
    move_forward(speed);
    sleep_ms(2000);
    stop();
    sleep_ms(1000);
    turn_left(speed);
    sleep_ms(2000);
    stop();
    sleep_ms(1000);
    move_backward(speed);
    sleep_ms(2000);
    stop();
    sleep_ms(1000);
    turn_right(speed);
    sleep_ms(2000);
    stop();
}