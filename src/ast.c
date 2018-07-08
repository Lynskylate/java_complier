//
// Created by lynskylate on 2018/7/6.
//

#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "ast.h"
#include "internal.h"

#define is_op(t) assert((op>=JAVA_TOKEN_OP_DOT) &&(op<=JAVA_TOKEN_OP_BIT_SHL_ASSIGN))

java_constant_number_node_t *java_int_node_new(long long int value, int lineno) {
    java_constant_number_node_t *self = (java_constant_number_node_t *) malloc(sizeof(java_constant_number_node_t));
    if (unlikely(!self)) return NULL;
    self->base.type = JAVA_NODE_INT;
    self->base.lineno = lineno;
    self->value.as_int = value;
    return self;
}

java_constant_number_node_t *java_bool_node_new(bool value, int lineno) {
    java_constant_number_node_t *self = (java_constant_number_node_t *) malloc(sizeof(java_constant_number_node_t));
    if (unlikely(!self)) return NULL;
    self->base.type = JAVA_NODE_BOOL;
    self->base.lineno = lineno;
    self->value.as_bool = value;
    return self;
}

java_constant_number_node_t *java_float_node_new(double value, int lineno) {
    java_constant_number_node_t *self = (java_constant_number_node_t *) malloc(sizeof(java_constant_number_node_t));
    if (unlikely(!self)) return NULL;
    self->base.type = JAVA_NODE_FLOAT;
    self->base.lineno = lineno;
    self->value.as_real = value;
    return self;
}

java_constant_string_node_t *java_string_node_new(char *str, int lineno) {
    java_constant_string_node_t *self = (java_constant_string_node_t *) malloc(sizeof(java_constant_string_node_t));
    if (unlikely(!self)) return NULL;
    self->base.type = JAVA_NODE_STRING;
    self->base.lineno = lineno;
    self->str_length = strlen(str);
    self->str = str;
    return self;
}

java_unary_op_node_t *java_unary_op_node_new(java_token op, java_node_t *expr, int lineno) {
    is_op(op);
    java_unary_op_node_t *self = (java_unary_op_node_t *) malloc(sizeof(java_unary_op_node_t));
    if (unlikely(!self)) return NULL;
    self->base.type = JAVA_NODE_UNARY_OP;
    self->base.lineno = lineno;
    self->op_type = op;
    self->expr = expr;
    return self;
}

java_id_node_t *java_id_node_new(const char *name, int lineno){
    java_id_node_t *self = (java_id_node_t *)malloc(sizeof(java_id_node_t));
    if (unlikely(!self)) return NULL;
    self->base.type = JAVA_NODE_ID;
    self->base.lineno = lineno;
    self->name = name;
    return self;
}

java_binary_op_node_t *java_binary_op_node_new(java_token op, java_node_t *left, java_node_t *right, int lineno){
    java_binary_op_node_t *self = (java_binary_op_node_t *)malloc(sizeof(java_binary_op_node_t));
    if (unlikely(!self)) return NULL;
    self->base.type = JAVA_NODE_BINARY_OP;
    self->base.lineno = lineno;
    self->left = left;
    self->right = right;
    return self;
}
