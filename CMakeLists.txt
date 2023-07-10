# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(DS4toPS2 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(DS4toPS2)

pico_set_program_name(DS4toPS2 "DS4toPS2")
pico_set_program_version(DS4toPS2 "1.0")

pico_enable_stdio_usb(DS4toPS2 1)

pico_set_linker_script(DS4toPS2 ${CMAKE_SOURCE_DIR}/memmap.ld)
pico_generate_pio_header(DS4toPS2 ${CMAKE_CURRENT_LIST_DIR}/psxSPI.pio)

# Add the standard library to the build
target_link_libraries(DS4toPS2
        pico_stdlib
        pico_time
        hardware_pio
        hardware_gpio
        pico_cyw43_arch_none
        pico_btstack_classic
        pico_btstack_ble
        pico_btstack_cyw43
        pico_multicore
)

target_sources(DS4toPS2 PUBLIC
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/controller_simulator.cpp
    ${CMAKE_SOURCE_DIR}/src/ds4/ds4.cpp
    ${CMAKE_SOURCE_DIR}/src/ds4/ds4InputState.cpp
    ${CMAKE_SOURCE_DIR}/src/ds4/ds4OutputState.cpp
)

# Add the standard include files to the build
target_include_directories(DS4toPS2 PUBLIC
  ${CMAKE_SOURCE_DIR}/inc
)

pico_add_extra_outputs(DS4toPS2)
