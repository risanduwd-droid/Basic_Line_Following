#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "headers/RaykhaS8.h"
#include "headers/RobotPID.h"
#include "headers/motor_driving.h"

// Global instances
RaykhaS8 trackSensor(false);
RobotPID myPID;
MotorDriver motor;
int baseSpeed = 100;

void driveMotors(int left, int right) {
    motor.drive(left, right);
}

// Simple non-blocking string reader
bool read_uart_string(char* buf, int max_len) {
    static int pos = 0;
    int c = getchar_timeout_us(0);
    while (c != PICO_ERROR_TIMEOUT) {
        if (c == '\n' || c == '\r') {
            buf[pos] = '\0';
            pos = 0;
            return true; // Line complete
        } else if (pos < max_len - 1 && c >= 32 && c <= 126) {
            buf[pos++] = (char)c;
        }
        c = getchar_timeout_us(0);
    }
    return false;
}

void parse_pid_command(const char* cmd) {
    // Expected format: P:15.0,I:0.0,D:5.0
    float p = myPID.getKp();
    float i = myPID.getKi();
    float d = myPID.getKd();
    
    if (sscanf(cmd, "P:%f,I:%f,D:%f", &p, &i, &d) == 3) {
        myPID.setTunings(p, i, d);
        // Acknowledge update to GUI
        printf("ACK:TUNINGS_SET\n");
    }
}

int main() {
    stdio_init_all();
    motor.init();
    trackSensor.init();
    
    sleep_ms(2000);
    printf("CALIBRATOR_READY\n");
    
    char cmd_buf[50];
    
    uint32_t last_telemetry_time = to_ms_since_boot(get_absolute_time());

    while (true) {
        // Handle incoming serial commands
        if (read_uart_string(cmd_buf, sizeof(cmd_buf))) {
            if (strlen(cmd_buf) > 0) {
                if (strncmp(cmd_buf, "P:", 2) == 0) {
                    parse_pid_command(cmd_buf);
                } else if (strcmp(cmd_buf, "STOP") == 0) {
                    driveMotors(0, 0);
                    baseSpeed = 0;
                    printf("ACK:STOPPED\n");
                } else if (strcmp(cmd_buf, "START") == 0) {
                    baseSpeed = 100;
                    printf("ACK:STARTED\n");
                }
            }
        }

        // Control loop processing
        float error = trackSensor.getError();
        
        if (error >= 999.0f) {
            driveMotors(0, 0);
            myPID.reset();
        } else if (baseSpeed > 0) {
            std::vector<int> speeds = myPID.calculateSpeeds(error, baseSpeed);
            driveMotors(speeds[0], speeds[1]);
        }
        
        // Send telemetry at ~10Hz
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        if (current_time - last_telemetry_time >= 100) {
            printf("T:%.2f,%d,%d,%.2f,%.2f,%.2f\n", 
                   error, baseSpeed, baseSpeed > 0 ? 1 : 0, 
                   myPID.getKp(), myPID.getKi(), myPID.getKd());
            last_telemetry_time = current_time;
        }

        // 50ms loop = 20Hz PID update rate
        sleep_ms(50);
    }
    
    return 0;
}
