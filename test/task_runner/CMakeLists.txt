cmake_minimum_required(VERSION 3.16.3)
project(test_task_runner)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ../../CrCanon
        ../../CrCanon/component
        ../../CrCanon/task_runner)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        )

target_link_libraries(
        ${PROJECT_NAME}
        # gtest
        pthread
        )
