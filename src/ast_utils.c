//
// Created by lynskylate on 2018/7/6.
//
#include <stdbool.h>
#include "ast_utils.h"

bool is_prefix_operation(java_token_t *tok) {
    if (tok->token_type == JAVA_TOKEN_OP_INCR ||
        tok->token_type == JAVA_TOKEN_OP_DECR ||
        tok->token_type == JAVA_TOKEN_OP_PLUS ||
        tok->token_type == JAVA_TOKEN_OP_MINUS ||
        tok->token_type == JAVA_TOKEN_OP_NOT ||
        tok->token_type == JAVA_TOKEN_OP_BIT_NOT)
        return true;
    return false;
}


bool is_postfix_operation(java_token_t *tok) {
    if (tok->token_type == JAVA_TOKEN_OP_INCR ||
        tok->token_type == JAVA_TOKEN_OP_DECR)
        return true;
    return false;
}
