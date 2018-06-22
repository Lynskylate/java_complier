project(test_jlib CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${JLIB_INCLUDE_DIRECTORY} ../src ${PROJECT_SOURCE_DIR}/include)
set(CMAKE_CXX_FLAGS -Wno-write-strings)

add_executable(test_lexer ${PROJECT_SOURCE_DIR}/test_lexer.cc)
target_link_libraries(test_lexer jlib)