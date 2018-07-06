PROJECT(jlib C)

set(CMAKE_C_STANDARD 99)

set(JLIB_INCLUDE_DIRECTORY ${PROJECT_SOURCE_DIR} .)

include_directories(${JLIB_INCLUDE_DIRECTORY})

set(JCC_SOURCE
        lexer.c
        token.c
        ast.c
        parser.c
        ast_utils.c
        utils.c)

add_library(jlib STATIC ${JCC_SOURCE})
add_executable(lex ${PROJECT_SOURCE_DIR}/bin/lex.c)
target_link_libraries(lex jlib)
