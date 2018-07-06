//
// Created by lynskylate on 2018/6/26.
//

#include "utils.h"
#include "lexer.h"
#include <stdlib.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

static const char *java_token_xml_type[] = {
#define t(tok, str) STR(tok),
		JCC_KEYWORD_LIST
		JCC_OP_LIST
		JCC_BASE_TOKEN_LIST
#undef t
};

void java_token_to_xml(java_token_t *tok) {
	fprintf(stderr, "<%s>", java_token_xml_type[tok->token_type]);
	if (tok->token_type == JAVA_TOKEN_CONSTANT_STRING || tok->token_type == JAVA_TOKEN_ID) {
		fprintf(stderr, "%s", tok->value.as_string);
	} else if (tok->token_type == JAVA_TOKEN_CONSTANT_INT) {
		fprintf(stderr, "%lld", tok->value.as_int);
	} else if (tok->token_type == JAVA_TOKEN_CONSTANT_FLOAT) {
		fprintf(stderr, "%lf", tok->value.as_real);
	}
	fprintf(stderr, "</%s>\n", java_token_xml_type[tok->token_type]);
}
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
		java_token_to_xml(&lexer->tok);
	}
}
