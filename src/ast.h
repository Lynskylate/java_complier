//
// Created by lynskylate on 2018/7/6.
//

#ifndef JCC_AST_H
#define JCC_AST_H

#include <stdio.h>
#include <stdbool.h>
#include "token.h"

#define JAVA_NODE_LIST \
    n(IF) \
    n(WHILE) \
    n(ID) \
    n(INT) \
    n(FLOAT) \
    n(STRING) \
    n(UNARY_OP) \
    n(BINARY_OP) \
    n(TRIPLE_OP) \
    n(PREFIX_OP) \
    n(POSTFIX_OP) \
    n(ASSIGNMENT_OP) \
    n(EXPRESSION) \

typedef enum {
#define n(NODE) JAVA_NODE_##NODE,
	JAVA_NODE_LIST
#undef n
} java_node_type;

typedef struct {
	java_node_type type;
	int lineno;
} java_node_t;

typedef struct {
	java_node_t base;
	double value;
} java_node_double;

typedef java_node_double java_node_float;

typedef struct {
	java_node_t base;
	long long int value;
} java_node_int;
typedef java_node_int java_node_long;

typedef struct {
	java_node_t base;
	int string_length;
	char *str;
} java_node_string;

typedef struct {

} ;


#endif //JCC_AST_H
