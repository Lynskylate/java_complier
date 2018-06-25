//
// Created by lynskylate on 2018/6/23.
//



#include <catch.hpp>
#include <lexer.h>
#include <token.h>


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

TEST_CASE("Test for lexer ID", "[ID]") {
	java_lexer_t lexer;
	java_lexer_init(&lexer, "int a = ", "ID");
	java_scan(&lexer);
	REQUIRE(lexer.tok.token_type == JAVA_TOKEN_INT);
	REQUIRE(lexer.lineno == 1);
	java_scan(&lexer);
	REQUIRE(lexer.tok.token_type == JAVA_TOKEN_ID);
	REQUIRE(strcmp(lexer.tok.value.as_string, "a") == 0);
	REQUIRE(lexer.lineno == 1);
	java_scan(&lexer);
	REQUIRE(lexer.tok.token_type == JAVA_TOKEN_OP_ASSIGN);
}

TEST_CASE("Test for lexer number", "[number]") {
	java_lexer_t lexer;
	java_lexer_init(&lexer, "13.2134 234 1e-15 -13 23 345", "ID");
	const float res[] = {13.2134, 234, 1e-15, 13, 23, 345};
	int len = 0;
	do {
		java_lexer_tok_reset(&lexer);
		java_scan(&lexer);
		if (lexer.tok.token_type == JAVA_TOKEN_CONSTANT_FLOAT) {
			REQUIRE(lexer.tok.value.as_real == res[len++]);
		} else if (lexer.tok.token_type == JAVA_TOKEN_CONSTANT_INT) {
			REQUIRE(lexer.tok.value.as_int == int(res[len++]));
		}
	} while (lexer.tok.token_type != JAVA_TOKEN_EOS);
}
