//
// Created by lynskylate on 2018/6/26.
//

#include "utils.h"
#include "lexer.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "%s <filename>", argv[0]);
		return -1;
	}
	char *source = file_read(argv[1]);
	java_lexer_t *lexer = (java_lexer_t *) malloc(sizeof(java_lexer_t));
	java_lexer_init(lexer, source, argv[1]);
	while (lexer->tok.token_type != JAVA_TOKEN_EOS) {
		java_lexer_tok_reset(lexer);
		java_scan(lexer);
		java_token_inspect(&lexer->tok);
	}
}
