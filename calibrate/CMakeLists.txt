# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
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

# Set Ninja build tool location for Windows
if(WIN32)
    set(CMAKE_MAKE_PROGRAM "${USERHOME}/.pico-sdk/ninja/v1.12.1/ninja.exe" CACHE FILEPATH "Ninja executable")
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(PIDCalibrator C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
# Include all source files from src/

add_executable(PIDCalibrator
        calibrate_main.cpp
        src/RaykhaS8.cpp
        src/RobotPID.cpp
        src/robot_2wd.cpp
)

pico_set_program_name(PIDCalibrator "PIDCalibrator")
pico_set_program_version(PIDCalibrator "0.1")

# Generate PIO headers for encoders (when encoder.pio is created)
# pico_generate_pio_header(LineFolower ${CMAKE_CURRENT_LIST_DIR}/encoder.pio)

# Modify the below lines to enable/disable output over UART/USB
# UART is reserved for Pi 4B communication (GP0/GP1)
pico_enable_stdio_uart(PIDCalibrator 0)
pico_enable_stdio_usb(PIDCalibrator 1)

# Add the standard library to the build
target_link_libraries(PIDCalibrator
        pico_stdlib
        hardware_gpio
        hardware_pwm
        hardware_adc
        hardware_i2c
        hardware_irq)

# Add the standard include files to the build
target_include_directories(PIDCalibrator PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/headers
)

# Add any user requested libraries
target_link_libraries(PIDCalibrator 
        hardware_pio
        )

pico_add_extra_outputs(PIDCalibrator)

