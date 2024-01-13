cmake_minimum_required(VERSION 3.16.3)
project(test_memory_pool)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ../../CrCanon
        ../../CrCanon/component
        ../../CrCanon/memory_pool)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        )

target_link_libraries(
        ${PROJECT_NAME}
        # gtest
        pthread

        )
