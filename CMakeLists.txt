cmake_minimum_required(VERSION 3.9)
project(jcc C)

set(CMAKE_C_FLAGS, "-Wall -DEBUG_PARSER")
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
