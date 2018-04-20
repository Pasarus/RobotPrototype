cmake_minimum_required(VERSION 3.6)
project(RobotPrototype2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        allcode_api.h
        allcode_api.o
        main.c
        main.o
        Makefile
        NMakefile
        robot.c
        robot.h
        robot.o
        RobotPrototype2.map
        RobotPrototype2.mcp
        RobotPrototype2.mcw
        states.h)

add_executable(RobotPrototype2 ${SOURCE_FILES})