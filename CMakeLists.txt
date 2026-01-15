cmake_minimum_required(VERSION 3.10)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(googletest-master/googletest-1.17.0)


add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib gtest)

