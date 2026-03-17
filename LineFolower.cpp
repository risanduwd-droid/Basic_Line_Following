#include <stdio.h>
#include <vector>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "headers/RaykhaS8.h"
#include "headers/RobotPID.h"
#include "headers/motor_driving.h"

// Global instances
RaykhaS8 trackSensor(false);  // false = black line detected (inverted logic)
RobotPID myPID;
MotorDriver motor;  // Motor driver instance
int baseSpeed = 100;

void driveMotors(int left, int right) {
    // Drive motors with PWM control on GP20/GP21
    printf("Left: %d | Right: %d\n", left, right);
    motor.drive(left, right);
}

int main() {
    // Initialize USB/UART stdio
    stdio_init_all();

    // Initialize motor driver
    motor.init();

    
    // Initialize sensors
    trackSensor.init();
    
    // Boot message
    sleep_ms(2000);
    printf("\n=== LINE FOLLOWER INITIALIZED ===\n");
    printf("Ready to follow!\n\n");
    
    // Main control loop
    while (true) {  
        float error = trackSensor.getError();
        
        // Check if line was lost
        if (error >= 999.0f) {
            printf("LOST LINE!\n");
            driveMotors(0, 0);
            sleep_ms(100);
            continue;
        }
        
        // Calculate motor speeds from PID
        std::vector<int> speeds = myPID.calculateSpeeds(error, baseSpeed);
        int leftMotor = speeds[0];
        int rightMotor = speeds[1];
        
        // Drive the motors
        driveMotors(leftMotor, rightMotor);
        
        // Control loop frequency: 50ms = 20Hz
        sleep_ms(50);
    }
    
    return 0;
}
