PROJECT(jlib C)

set(CMAKE_C_STANDARD 99)

set(JLIB_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR} .)

include_directories(${JLIB_INCLUDE_DIRECTORY})

set(JCC_SOURCE
        lexer.c
        token.c)

add_library(jlib SHARED ${JCC_SOURCE})
