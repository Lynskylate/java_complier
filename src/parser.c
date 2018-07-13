//
// Created by lynskylate on 2018/7/6.
//
#include <stdio.h>
#include "parser.h"
#include "lexer.h"
#include "token.h"


#define next (self->tok = (java_scan(self->lex), &self->lex->tok))

#ifdef EBUG_PARSER
#define debug(name) \
  fprintf(stderr, "\n%s\n", name); \
  java_token_inspect(&self->lex->tok);
#else
#define debug(name)
#endif

#ifdef EBUG_PARSER
#define accept(t) \
  (is(t)) \
    ? (fprintf(stderr, "\e[90maccepted \e[33m%s\e[0m\n", #t), next, 1) \
    : 0)
#else
#define accept(t) (is(t) && next)
#endif

#define is(type) (self->tok->token_type == type)

#define context(str) (self->ctx = str)

#define error(str) \
  ((self->err = self->err \
    ? self->err \
    : str), NULL)

#define lineno self->lex->lineno

void java_parser_init(java_parser_t *self, java_lexer_t *lex) {
    self->lex = lex;
    self->tok = NULL;
    self->ctx = NULL;
    self->err = NULL;
}

static java_node_t *expr(java_parser_t *self);

static java_node_t *
paren_expr(java_parser_t *self) {
    java_node_t *node;
    debug("paren expr");
    if (!accept(JAVA_TOKEN_LPAREN))return NULL;
    if (!(node = expr(self))) return NULL;
    if (!accept(JAVA_TOKEN_RPAREN)) return error("expression missing closing )");
    return node;
}

static java_node_t *
primary_expr(java_parser_t *self) {
    debug("Primary_expr");
    java_node_t *ret = NULL;
    java_token_t *tok = self->tok;
    switch (tok->token_type) {
        case JAVA_TOKEN_ID:
            ret = (java_node_t *) java_id_node_new(tok->value.as_string, lineno);
            break;
        case JAVA_TOKEN_CONSTANT_INT:
            ret = (java_node_t *) java_int_node_new(tok->value.as_int, lineno);
            break;
        case JAVA_TOKEN_CONSTANT_FLOAT:
            ret = (java_node_t *) java_float_node_new(tok->value.as_real, lineno);
            break;
        case JAVA_TOKEN_CONSTANT_STRING:
            ret = (java_node_t *) java_string_node_new(tok->value.as_string, lineno);
            break;
    }

    if (ret) {
        next;
        return ret;
    }
    return paren_expr(self);
}

static java_node_t *
postfix_expr(java_parser_t *self) {
    java_node_t *node;
    int line = lineno;
    debug("postifix_expr");
    if (!(node = primary_expr(self))) return NULL;
    if (is(JAVA_TOKEN_OP_INCR) || is (JAVA_TOKEN_OP_DECR)) {
        node = (java_node_t *) java_unary_op_node_new(self->tok->token_type, node, line);
        next;
    }
    return node;
}

static java_node_t *
unary_expr(java_parser_t *self) {
    debug("unary_expr");
    int line = lineno;
    if (is(JAVA_TOKEN_OP_INCR) ||
        is(JAVA_TOKEN_OP_DECR) ||
        is(JAVA_TOKEN_OP_BIT_NOT) ||
        is(JAVA_TOKEN_OP_PLUS) ||
        is(JAVA_TOKEN_OP_MINUS) ||
        is(JAVA_TOKEN_OP_NOT)) {
        java_token op = self->tok->token_type;
        next;
        return (java_node_t *) java_unary_op_node_new(op, unary_expr(self), line);
    }
    return (java_node_t *) postfix_expr(self);
}

static java_node_t *
multiplicative_expr(java_parser_t *self) {
    java_token op;
    java_node_t *node, *right;
    int line = lineno;
    debug("multiplicative expr");
    if (!(node = unary_expr(self))) return NULL;
    while (is(JAVA_TOKEN_OP_MUL) || is(JAVA_TOKEN_OP_DIV) || is(JAVA_TOKEN_OP_MOD)) {
        op = self->tok->token_type;
        next;
        context("addicitive expression");
        if ((right = unary_expr(self))) {
            node = (java_node_t *) java_binary_op_node_new(op, node, right, line);
        } else {
            return error("missing right_hand expression");
        }
    }
    return node;

}

static java_node_t *
additive_expr(java_parser_t *self) {
    java_token op;
    java_node_t *node, *right;
    int line = lineno;
    debug("addictive expr");
    if (!(node = multiplicative_expr(self))) return NULL;
    while (is(JAVA_TOKEN_OP_PLUS) || is(JAVA_TOKEN_OP_MINUS)) {
        op = self->tok->token_type;
        next;
        context("addicitive expression");
        if ((right = multiplicative_expr(self))) {
            node = (java_node_t *) java_binary_op_node_new(op, node, right, line);
        } else {
            return error("missing right_hand expression");
        }
    }
    return node;
}

static java_node_t *
shift_expr(java_parser_t *self) {
    java_token op;
    java_node_t *node, *right;
    int line = lineno;
    debug("shift expr");
    if (!(node = additive_expr(self))) return NULL;
    while (is(JAVA_TOKEN_OP_BIT_SHL) || is(JAVA_TOKEN_OP_BIT_SHR)) {
        op = self->tok->token_type;
        next;
        context("shift expression");
        if ((right = additive_expr(self))) {
            node = (java_node_t *) java_binary_op_node_new(op, node, right, line);
        } else {
            return error("missing right_hand expression");
        }
    }
    return node;
}

static java_node_t *
relational_expr(java_parser_t *self) {
    java_token op;
    java_node_t *node, *right;
    int line = lineno;
    debug("relational_expr");
    if (!(node = shift_expr(self))) return NULL;
    while (is(JAVA_TOKEN_OP_LT) || is(JAVA_TOKEN_OP_LTE)
           || is(JAVA_TOKEN_OP_GT) || is(JAVA_TOKEN_OP_GTE)) {
        op = self->tok->token_type;
        next;
        if (right = shift_expr(self)) {
            node = (java_node_t *) java_binary_op_node_new(op, node, right, line);
        } else {
            return error("missing right_hand expression");
        }
    }
    return node;
}

static java_node_t *
equality_expr(java_parser_t *self) {
    java_token op;
    java_node_t *node, *right;
    int line = lineno;
    debug("equality_expr");
    if (!(node = relational_expr(self))) return NULL;
    while (is(JAVA_TOKEN_OP_EQ) || is(JAVA_TOKEN_OP_NEQ)) {
        op = self->tok->token_type;
        next;
        context("equality expression");
        if (right = relational_expr(self)) {
            node = (java_node_t *) java_binary_op_node_new(op, node, right, line);
        } else {
            return error("missiong right-hand expression");
        }
    }
    return node;
}

static java_node_t *
bitwise_and_expr(java_parser_t *self) {
    java_node_t *node, *right;
    debug('bitwise_xor_expr');
    int line = lineno;
    if (!(node = equality_expr(self))) return NULL;
    while (accept(JAVA_TOKEN_OP_BIT_AND)) {
        context("& operation");
        if ((right = bitwise_and_expr(self))) {
            node = (java_node_t *) java_binary_op_node_new(JAVA_TOKEN_OP_BIT_AND, node, right, line);
        } else {
            return error("Missing right_hand expression");
        }
    }
    return node;
}

static java_node_t*
bitwise_xor_expr(java_parser_t *self) {
    java_node_t *node, *right;
    debug('bitwise_xor_expr');
    int line = lineno;
    if (!(node = bitwise_and_expr(self))) return NULL;
    while (accept(JAVA_TOKEN_OP_BIT_XOR)) {
        context("~ operation");
        if ((right = bitwise_and_expr(self))) {
            node = (java_node_t *) java_binary_op_node_new(JAVA_TOKEN_OP_BIT_XOR, node, right, line);
        } else {
            return error("Missing right_hand expression");
        }
    }
    return node;
}

static java_node_t *
bitwise_or_expr(java_parser_t *self) {
    java_node_t *node, *right;
    debug('bitwise_or_expr');
    int line = lineno;
    if (!(node = bitwise_xor_expr(self))) return NULL;
    while (accept(JAVA_TOKEN_OP_BIT_OR)) {
        context("| operation");
        if ((right = bitwise_xor_expr(self))) {
            node = (java_node_t *) java_binary_op_node_new(JAVA_TOKEN_OP_BIT_OR, node, right, line);
        } else {
            return error("Missing right_hand expression");
        }
    }
    return node;
}

static java_node_t *
logical_and_expr(java_parser_t *self) {
    java_node_t *node, *right;
    debug("logical and expr");
    int line = lineno;
    if (!(node = bitwise_or_expr(self))) return NULL;

    while (accept(JAVA_TOKEN_OP_OR)) {
        context("&& Operation");
        if ((right = bitwise_or_expr(self))) {
            node = (java_node_t *) java_binary_op_node_new(JAVA_TOKEN_OP_AND, node, right, line);
        } else {
            return error("Missing right_hand expression");
        }
    }
    return node;
}

static java_node_t *
logical_or_expr(java_parser_t *self) {
    java_node_t *node, *right;
    int line = lineno;
    debug("logical_or_expr");
    if (!(node = logical_and_expr(self))) return NULL;

    while (accept(JAVA_TOKEN_OP_OR)) {
        context("|| Operation");
        if ((right = logical_and_expr(self))) {
            node = (java_node_t *) java_binary_op_node_new(JAVA_TOKEN_OP_OR, node, right, line);
        } else {
            return error("Missing right_hand expression");
        }
    }

    return node;
}

/*
 *
 */
static java_node_t *
assignment_expr(java_parser_t *self) {
    java_token op;
    java_node_t *node, *right;
    int line = lineno;

    debug("assignment_expr");
    //TODO: ADD assign_identifier

    if (is(JAVA_TOKEN_OP_ASSIGN)) {
        op = self->tok->token_type;
        next;
        context("assignment");
        if (!(right = expr(self))) return NULL;
        java_binary_op_node_t *ret = java_binary_op_node_new(op, node, right, line);
    }
    return logical_or_expr(self);
}


static java_node_t *
expr(java_parser_t *self) {
    java_node_t *node;
    debug("expr");
    if (!(node = assignment_expr(self))) return NULL;
    return node;
}

/*
 * Parse input.
 */

void
java_parse(java_parser_t *self) {
    debug("program");
    java_node_t *node;

    next;
    while (!is(JAVA_TOKEN_EOS)) {
        expr(self);
    }

}


