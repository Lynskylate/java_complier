//
// Created by lynskylate on 2018/7/6.
//
#include "parser.h"
#include "lexer.h"
#include "token.h"


#define next (self->tok = (java_scan(self->lex), &self->lex->tok))

#define is(type) (self->tok->token_type == type)

void java_parser_init(java_parser_t *self, java_lexer_t *lex) {
	self->lex = lex;
	self->tok = NULL;
	self->ctx = NULL;
	self->err = NULL;
}

