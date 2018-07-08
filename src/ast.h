//
// Created by Lynn on 2018/7/7.
//

#ifndef JCC_AST_H
#define JCC_AST_H
#ifdef __cplusplus
extern "C"{
#endif

#include "token.h"
#include "lexer.h"
#include <stdbool.h>


#define JAVA_NODE_LIST \
    n(BLOCK) \
    n(EXPR_STMT) \
    n(RETURN) \
    n(UNARY_OP) \
    n(BINARY_OP) \
    n(TERNARY_OP) \
    n(BOOL) \
    n(INT) \
    n(FLOAT) \
    n(STRING) \
    n(ID)

typedef enum {
#define n(node) JAVA_NODE_##node,
    JAVA_NODE_LIST
#undef n
} java_node_type;

typedef struct {
    java_node_type type;
    int lineno;
} java_node_t;

typedef struct {
    java_node_t base;
    java_token op_type;
    java_node_t *left;
    java_node_t *right;
} java_binary_op_node_t;

typedef struct {
    java_node_t base;
    java_token op_type;
    java_node_t *expr;
} java_unary_op_node_t;

typedef struct {
    java_node_t base;
    const char *name;
} java_id_node_t;

typedef struct {
    java_node_t base;
    union {
        long long int as_int;
        double as_real;
        bool as_bool;
    } value;
} java_constant_number_node_t;

typedef struct {
    java_node_t base;
    int str_length;
    char *str;
} java_constant_string_node_t;

java_constant_number_node_t *java_float_node_new(double, int);

java_constant_number_node_t *java_int_node_new(long long int, int);

java_constant_number_node_t *java_bool_node_new(bool, int);

java_constant_string_node_t *java_string_node_new(char *str, int);

java_unary_op_node_t *java_unary_op_node_new(java_token, java_node_t *, int);

java_id_node_t *java_id_node_new(const char *, int);

java_binary_op_node_t *java_binary_op_node_new(java_token, java_node_t *, java_node_t *, int);

#ifdef __cplusplus
}
#endif
#endif //JCC_AST_H
