cmake_minimum_required(VERSION 3.16.3)
project(shared_ptr_test)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        ../../CrCanon
        ../../CrCanon/component
        ../../CrCanon/log
)

add_executable(
        ${PROJECT_NAME}
        shared_ptr_test.cpp
        ../../CrCanon/log/log.cpp
)

target_link_libraries(
        ${PROJECT_NAME}
)
