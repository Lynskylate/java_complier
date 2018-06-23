//
// Created by lynskylate on 2018/6/18.
//

#ifndef JCC_LEXER_H
#define JCC_LEXER_H

#ifdef __cplusplus
extern "C"{
#endif

#include <sys/stat.h>
#include "token.h"

#ifndef LEXER_BUFFER_SIZE
#define LEXER_BUFFER_SIZE 1024
#endif

typedef struct {
	char *error;
	int stash;
	int lineno;
	off_t offset;
	char *source;
	const char *filename;
	java_token_t tok;
	char buf[LEXER_BUFFER_SIZE];
} java_lexer_t;

int java_scan(java_lexer_t *self);

void java_lexer_init(java_lexer_t *self, char *source, const char *filename);

int java_lexer_tok_reset(java_lexer_t *self);

#ifdef __cplusplus
}
#endif

#endif //JCC_LEXER_H
