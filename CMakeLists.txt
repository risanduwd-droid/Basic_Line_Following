# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

if(WIN32)
    set(CMAKE_MAKE_PROGRAM "${USERHOME}/.pico-sdk/ninja/v1.12.1/ninja.exe" CACHE FILEPATH "Ninja executable")
endif()

set(PICO_BOARD pico2 CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(PIDCalibrator C CXX ASM)

pico_sdk_init()

# --- FIXED: Target name is now consistently PIDCalibrator ---
add_executable(PIDCalibrator
    calibrate_main.cpp
    src/RaykhaS8.cpp
    src/RobotPID.cpp
    src/robot_2wd.cpp
)

pico_set_program_name(PIDCalibrator "PIDCalibrator")
pico_set_program_version(PIDCalibrator "0.1")

# Enable USB output for your calibration menu, disable UART so it doesn't spam the Pi 4B
pico_enable_stdio_uart(PIDCalibrator 0)
pico_enable_stdio_usb(PIDCalibrator 1)

# Link the standard libraries to PIDCalibrator
target_link_libraries(PIDCalibrator
        pico_stdlib
        hardware_gpio
        hardware_pwm
        hardware_adc
        hardware_i2c
        hardware_irq)

target_include_directories(PIDCalibrator PRIVATE ${CMAKE_CURRENT_LIST_DIR})