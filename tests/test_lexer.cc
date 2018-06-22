//
// Created by lynskylate on 2018/6/23.
//



#include <lexer.h>

#define CATCH_CONFIG_MAIN

#include <catch.hpp>


TEST_CASE("Test fro lexer multiline comment", "[factorial]") {
	java_lexer_t lexer;
	java_lexer_init(&lexer, "/*sdsads\na\nsdsad*/\"asdsad\"", "multi_line_comment");
	java_scan(&lexer);
	REQUIRE(lexer.tok.token_type == JAVA_TOKEN_CONSTANT_STRING);
	REQUIRE(lexer.lineno == 3);
}

