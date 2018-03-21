cmake_minimum_required(VERSION 3.6)
project(RobotPrototype)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c states.h robot.c robot.h)
add_executable(RobotPrototype ${SOURCE_FILES})