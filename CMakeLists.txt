cmake_minimum_required(VERSION 3.1.0)
project(hifive LANGUAGES C ASM)

set(SOURCES
    src/bootstrap.s
    src/sleep.s
    src/clocks.c
    src/gpio.c
    src/system.c
    src/uart.c)

set(CMAKE_ASM_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medlow")
set(CMAKE_C_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medlow -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medlow -ffunction-sections -fdata-sections")

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

