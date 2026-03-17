#include "../headers/RobotPID.h"

RobotPID::RobotPID() {
    previousError = 0.0f;
    integral = 0.0f;
}

std::vector<int> RobotPID::calculateSpeeds(float error, int baseSpeed) {
    // Team Leader's request: K values are now safely inside the function
    float Kp = 15.0f;
    float Ki = 0.0f;   // Integral disabled for now
    float Kd = 5.0f;

    // Do the standard PID math
    float P = Kp * error;
    integral += error;
    float I = Ki * integral;
    float D = Kd * (error - previousError);
    previousError = error;

    float correction = P + I + D;

    // Calculate final wheel speeds with correction
    int leftSpeed = baseSpeed + (int)correction;
    int rightSpeed = baseSpeed - (int)correction;

    // Return both speeds packed in a vector
    return {leftSpeed, rightSpeed};
}
