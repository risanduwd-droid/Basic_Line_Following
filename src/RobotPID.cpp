#include "../headers/RobotPID.h"

RobotPID::RobotPID() {
    // Default values matched with the previous hardcoded values
    Kp = 15.0f;
    Ki = 0.0f;
    Kd = 5.0f;
    previousError = 0.0f;
    integral = 0.0f;
}

void RobotPID::setTunings(float p, float i, float d) {
    Kp = p;
    Ki = i;
    Kd = d;
}

void RobotPID::reset() {
    integral = 0.0f;
    previousError = 0.0f;
}

std::vector<int> RobotPID::calculateSpeeds(float error, int baseSpeed) {
    // Standard PID math
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
