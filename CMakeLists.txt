cmake_minimum_required(VERSION 3.9)
project(jcc C)

set(CMAKE_C_STANDARD 99)
#set(CMAKE_CXX_STANDARD 11)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
)

SET(COMMON
        ${PROJECT_SOURCE_DIR}/src/token.c
        ${PROJECT_SOURCE_DIR}/src/lexer.c
        )

set(SRC
        ${COMMON}
        ${PROJECT_SOURCE_DIR}/main.c)

set(TEST_SOURCES
        ${PROJECT_SOURCE_DIR}/tests/test_lexer.c)

add_executable(jcc ${SRC})

#add_executable(tests ${COMMON} ${TEST_SOURCES})
