#ifndef ROBOT_PID_H
#define ROBOT_PID_H

#include <vector>
#include <cstdint>

class RobotPID {
private:
    float previousError;  // For derivative calculation
    float integral;        // For integral windup

public:
    // Constructor
    RobotPID();

    // Calculate left and right motor speeds based on error
    // Returns vector: [leftSpeed, rightSpeed]
    std::vector<int> calculateSpeeds(float error, int baseSpeed);
};

#endif
