#ifndef ROBOTPID_H
#define ROBOTPID_H

#include <vector>

class RobotPID {
public:
    RobotPID();
    
    // Dynamic setters for calibration
    void setTunings(float p, float i, float d);
    void reset();
    
    std::vector<int> calculateSpeeds(float error, int baseSpeed);

    float getKp() const { return Kp; }
    float getKi() const { return Ki; }
    float getKd() const { return Kd; }

private:
    float Kp;
    float Ki;
    float Kd;
    float previousError;
    float integral;
};

#endif // ROBOTPID_H
