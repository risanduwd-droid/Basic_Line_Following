# Line Follower Robot - Build Instructions

## **Project Structure**
```
LineFolower/
├── LineFolower.cpp              # Main entry point
├── CMakeLists.txt               # Build configuration (updated)
├── headers/                     # Class definitions
│   ├── RaykhaS8.h              # Line sensor class
│   ├── RobotPID.h              # PID controller class
│   ├── motor_driving.h         # Motor PWM control (template)
│   └── line_sensor.h           # Sharp IR utility functions
├── src/                         # Implementations
│   ├── RaykhaS8.cpp            # Line sensor logic
│   └── RobotPID.cpp            # PID calculations
├── pico_sdk_import.cmake        # Pico SDK CMake
└── build/                       # Compiled output (auto-generated)
```

## **How to Build & Upload**

### 1. **Clean Build** (First Time or After Major Changes)
```bash
cd build
rm -r CMakeFiles/ CMakeCache.txt
cmake ..
ninja
```

### 2. **Quick Rebuild** (After Code Changes)
```bash
cd build
ninja
```

### 3. **Upload to Pico**
- Hold BOOTSEL button on Pico 2
- Connect via USB
- Run the "Run Project" VS Code task to load `build/LineFolower.uf2`
- Or: `picotool load build/LineFolower.uf2 -fx`

## **What's Implemented**
✅ **RaykhaS8 Class**: Reads 8 line sensors (GP6-GP13), returns weighted error  
✅ **RobotPID Class**: PD loop (Kp=15, Kd=5) for steering correction  
✅ **CMakeLists.txt**: Configured for C++17, hardware libraries linked  
✅ **Main Loop**: 20Hz control loop, reads error → calculates motor speeds  

## **What Still Needs Completion**
1. **Motor PWM Drivers** (motor_driving.h template provided)
   - Implement `driveMotors()` in LineFolower.cpp to use MotorDriver class
   - Set proper PWM frequency (target ~1kHz for smooth control)
   
2. **Encoder PIO** (Go-to: architecture doc for 407 pulses/rev)
   - Create `quadrature_encoder.pio` file
   - Uncomment PIO header generation in CMakeLists.txt
   - Implement `get_left_ticks()` and `get_right_ticks()` functions
   
3. **UART Communication** (currently disabled)
   - Enable UART0 IRQ on GP0/GP1 to receive commands from Pi 4B
   - Update state machine to handle 'W' (Wall Follow), 'S' (Stop), etc.
   
4. **I2C Sensors** (IMU & Compass)
   - Add IMU.h header with MPU6500 init
   - Add Compass.h header with GY271M init
   
5. **Ultrasonic & Sharp IR** (ADC reading)
   - Implement ultrasonic timing on GP2/GP3
   - Read Sharp IR on GP26/GP27 (already ADC configured)

## **Debug Tips**
- USB output enabled → `printf()` statements visible on serial port (115200 baud)
- If "LOST LINE" appears repeatedly: Check pull-up resistors on GP6-13
- If motors don't respond: Check IBT-2 VCC = 3.3V, L_EN/R_EN tied to 3.3V
- If encoder counts = 0: Check PIO and GP16-19 wiring

## **Key Includes**
```cpp
#include "pico/stdlib.h"           // Basic Pico functions
#include "hardware/gpio.h"         // GPIO control
#include "hardware/pwm.h"          // Motor PWM
#include "hardware/adc.h"          // ADC for IR sensors
#include "hardware/i2c.h"          // I2C for IMU/Compass
#include "hardware/pio.h"          // PIO for encoders
#include "pico/binary_info.h"      // Binary info
```

---
**Team Endurance** | RP2350 @ 125MHz | C++17 | Pico SDK v2.2.0
