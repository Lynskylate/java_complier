//
// Created by Lynn on 2018/7/9.
//

#include "parser.h"
#include "catch.hpp"
#include <string.h>

TEST_CASE("Test for parse", "[primary]") {
    java_lexer_t lexer;
    java_lexer_init(&lexer, "1 + 2 * 3", "ID");
    java_parser_t parser;
    java_parser_init(&parser, &lexer);
    java_parse(&parser);
}

