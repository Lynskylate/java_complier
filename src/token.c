//
// Created by lynskylate on 2018/6/17.
//
#include <stdio.h>

#include "token.h"


void java_token_inspect(java_token_t *tok) {
    fprintf(stderr, "\033[1;31mTokenType: %s\033[0m", java_token_type_string(tok->token_type));
    if (tok->token_type == JAVA_TOKEN_CONSTANT_STRING) {
        fprintf(stderr, "\033[1;31m %s \033[0m", tok->value.as_string);
    }
    else if (tok->token_type == JAVA_TOKEN_CONSTANT_INT) {

        fprintf(stderr, "\033[1;31m %d \033[0m", tok->value.as_int);
    } else if (tok->token_type == JAVA_TOKEN_CONSTANT_FLOAT) {
        fprintf(stderr, "\033[1;31m %f \033[0m", tok->value.as_real);
    }
    fprintf(stderr, "\n");
}
