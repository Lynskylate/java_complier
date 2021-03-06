//
// Created by lynskylate on 2018/6/16.
//

#ifndef JCC_TOKEN_H
#define JCC_TOKEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#define JCC_KEYWORD_LIST \
    t(DO, "do") \
    t(IF, "if") \
    t(FOR, "for") \
    t(INT, "int") \
    t(NEW, "new") \
    t(TRY, "try") \
    t(VOID, "void") \
    t(THIS, "this") \
    t(CHAR, "char") \
    t(BYTE, "byte") \
    t(CASE, "case") \
    t(ELSE, "else") \
    t(LONG, "long") \
    t(ENUM, "enum") \
    t(GOTO, "goto") \
    t(CONST, "const") \
    t(SHORT, "short") \
    t(BREAK, "break") \
    t(CATCH, "catch") \
    t(CLASS, "class") \
    t(FINAL, "final") \
    t(FLOAT, "float") \
    t(SUPER, "super") \
    t(THROW, "throw") \
    t(WHILE, "while") \
    t(THROWS, "throws") \
    t(NATIVE, "native") \
    t(STATIC, "static") \
    t(SWITCH, "switch") \
    t(IMPORT, "import") \
    t(DOUBLE, "double") \
    t(PUBLIC, "public") \
    t(RETURN, "return") \
    t(BOOLEAN, "boolean") \
    t(PACKAGE, "package") \
    t(PRIVATE, "private") \
    t(DEFAULT, "default") \
    t(EXTENDS, "extends") \
    t(FINALLY, "finally") \
    t(VOLATILE, "volatile") \
    t(ABSTRACT, "abstract") \
    t(CONTINUE, "continue") \
    t(INTERFACE, "interface") \
    t(PROTECTED, "protected") \
    t(TRANSIENT, "transient") \
    t(IMPLEMENTS, "implements") \
    t(INSTANCEOF, "instanceof") \
    t(SYNCHRONIZED, "synchronized")

#define JCC_OP_LIST \
  t(OP_DOT, ".") \
  t(OP_NOT, "!") \
  t(OP_PLUS, "+") \
  t(OP_INCR, "++") \
  t(OP_MINUS, "-") \
  t(OP_DECR, "--") \
  t(OP_MUL, "*") \
  t(OP_DIV, "/") \
  t(OP_MOD, "%") \
  t(OP_GT, ">") \
  t(OP_LT, "<") \
  t(OP_GTE, ">=") \
  t(OP_LTE, "<=") \
  t(OP_EQ, "==") \
  t(OP_NEQ, "!=") \
  t(OP_AND, "&&") \
  t(OP_OR, "||") \
  t(OP_ASSIGN, "=") \
  t(OP_PLUS_ASSIGN, "+=") \
  t(OP_MINUS_ASSIGN, "-=") \
  t(OP_MUL_ASSIGN, "*=") \
  t(OP_DIV_ASSIGN, "/=") \
  t(OP_BIT_OR, "|") \
  t(OP_BIT_AND, "&") \
  t(OP_BIT_XOR, "^") \
  t(OP_BIT_NOT, "~") \
  t(OP_BIT_SHL, "<<") \
  t(OP_BIT_SHR, ">>") \
  t(OP_BIT_SHR_ASSIGN, ">>=") \
  t(OP_BIT_SHL_ASSIGN, "<<=")

#define JCC_BASE_TOKEN_LIST \
    t(LBRACE, "{") \
    t(RBRACE, "}") \
    t(LPAREN, "(") \
    t(RPAREN, ")") \
    t(LBRACK, "[") \
    t(RBRACK, "]") \
    t(COLON, ":") \
    t(QMARK, "?") \
    t(SEMICOLON, ";") \
    t(COMMA, ",") \
    t(ID, "ID")\
    t(ILLEGAL, "illegal") \
    t(EOS, "EOS") \
    t(CONSTANT_TRUE, "true") \
    t(CONSTANT_FALSE, "false") \
    t(CONSTANT_STRING, "string") \
    t(CONSTANT_INT, "int") \
    t(CONSTANT_CHAR, "char") \
    t(CONSTANT_FLOAT, "float")

typedef enum {
#define t(tok, str) JAVA_TOKEN_##tok,
    JCC_KEYWORD_LIST
    JCC_OP_LIST
    JCC_BASE_TOKEN_LIST
#undef t
} java_token;

typedef struct {
    java_token token_type;
    struct {
        char *as_string;
        double as_real;
        long long int as_int;
    } value;
} java_token_t;

static const char *java_token_strings[] = {
#define t(tok, str) str,
        JCC_KEYWORD_LIST
        JCC_OP_LIST
        JCC_BASE_TOKEN_LIST
#undef t
};

static inline const char *
java_token_type_string(java_token type) {
    assert(type <= JAVA_TOKEN_CONSTANT_FLOAT);
    return java_token_strings[type];
}

void java_token_inspect(java_token_t *tok);

#ifdef __cplusplus
}
#endif

#endif //JCC_TOKEN_H
