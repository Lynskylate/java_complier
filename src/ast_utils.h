//
// Created by Lynn on 2018/7/8.
//

#ifndef JCC_AST_UTILS_H
#define JCC_AST_UTILS_H
#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include "token.h"

bool is_basic_type(java_token_t *tok);

#ifdef __cplusplus
}
#endif
#endif //JCC_AST_UTILS_H
