cmake_minimum_required(VERSION 3.16.3)
project(timer_test)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ../../CrCanon
        ../../CrCanon/component
        ../../CrCanon/log
        ../../CrCanon/timer)

add_executable(
        ${PROJECT_NAME}
        main.cpp
)

