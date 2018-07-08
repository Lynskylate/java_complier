//
// Created by lynskylate on 2018/7/6.
//

#ifndef JCC_PARSER_H
#define JCC_PARSER_H
#ifdef __cplusplus
extern "C"{
#endif

#include "ast.h"
#include "token.h"
#include "lexer.h"


typedef struct {
	char *ctx;
	char *err;
	java_token_t *tok;
	java_lexer_t *lex;
} java_parser_t;

void java_parser_init(java_parser_t *self, java_lexer_t *lex);

void java_parse(java_parser_t *self);
#ifdef __cplusplus
}
#endif
#endif //JCC_PARSER_H
