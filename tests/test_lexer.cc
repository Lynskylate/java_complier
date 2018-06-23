//
// Created by lynskylate on 2018/6/23.
//



#include <catch.hpp>
#include <lexer.h>


TEST_CASE("Test for lexer multiline comment", "[comment]") {
	java_lexer_t lexer;
	java_lexer_init(&lexer, "/*sdsads\na\nsdsad*/\"asdsad\"", "multi_line_comment");
	java_scan(&lexer);
	REQUIRE(lexer.tok.token_type == JAVA_TOKEN_CONSTANT_STRING);
	REQUIRE(lexer.lineno == 3);
}


TEST_CASE("Test for lexer string", "[string]") {
	java_lexer_t lexer;
	java_lexer_init(&lexer, "\"loveforyou\xaa\"", "string");
	java_scan(&lexer);
	REQUIRE(strcmp(lexer.tok.value.as_string, "loveforyou\xaa") == 0);
	REQUIRE(lexer.lineno == 1);
}
