//
// Created by lynskylate on 2018/6/18.
//

#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include "lexer.h"

#ifndef _DEBUG_LEXER
#define next \
    (self->stash = self->source[self->offset++])
#else
#define next \
	(self->stash = self->source[self->offset++])
#endif

#define undo (--self->offset)

#define token(t) (self->tok.token_type = t)

#define error(message) (self->error=message, token(JAVA_TOKEN_TOKEN_ILLEGAL))

void java_lexer_init(java_lexer_t *self, char *source, const char *filename) {
	self->error = NULL;
	self->lineno = 1;
	self->filename = filename;
	self->offset = 0;
	self->source = source;
}

static int hex(const char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
	return -1;
}

static int hex_literal(java_lexer_t *self) {
	int a = hex(next);
	int b = hex(next);
	if (a > -1 && b > -1)return a << 4 | b;
	error("string hex literal \\x contains invalid digits");
	return -1;
}

static int scan_string(java_lexer_t *self, int quote) {
	int c, len = 0;
	int buffer_len = 128;
	char *buf = (char *) malloc(sizeof(char) * buffer_len);
	token(JAVA_TOKEN_CONSTANT_STRING);
	while (quote != (c = next)) {
		switch (c) {
			case '\n':
				self->lineno++;
				break;
			case '\\':
				switch (c = next) {
					case 'a':
						c = '\a';
						break;
					case 'n':
						c = '\n';
						break;
					case 'b':
						c = '\b';
						break;
					case 'r':
						c = '\r';
						break;
					case 't':
						c = '\t';
						break;
					case '\'':
						c = '\'';
						break;
					case '"':
						c = '"';
						break;
					case 'f':
						c = '\f';
						break;
					case 'v':
						c = '\v';
						break;
					case 'x':
						if (-1 == hex_literal(self)) {
							goto error;
						}
				}
				break;
		}
		buf[len++] = c;
		if (len >= buffer_len) {
			buffer_len = buffer_len + 128;
			buf = (char *) realloc(buf, sizeof(char) * buffer_len);
		}
	}
	buf[len] = 0;
	self->tok.value.as_string = strdup(buf);
	free(buf);
	return 1;

error:
	free(buf);
	return 0;
}

static int scan_ident(java_lexer_t *self, int c) {
	int len = 0;

}

static int scan_number(java_lexer_t *self, int c) {}


int java_scan(java_lexer_t *self) {
	int c;

scan:
	switch (c = next) {
		case '\t':
		case ' ':
			goto scan;
		case '(':
			return token(JAVA_TOKEN_LPAREN);
		case ')':
			return token(JAVA_TOKEN_RPAREN);
		case '{':
			return token(JAVA_TOKEN_LBRACE);
		case '}':
			return token(JAVA_TOKEN_RBRACE);
		case '[':
			return token(JAVA_TOKEN_LBRACK);
		case ']':
			return token(JAVA_TOKEN_RBRACK);
		case ',':
			return token(JAVA_TOKEN_COMMA);
		case '.':
			return token(JAVA_TOKEN_OP_DOT);
		case '%':
			return token(JAVA_TOKEN_OP_MOD);
		case '^':
			return token(JAVA_TOKEN_OP_BIT_XOR);
		case '~':
			return token(JAVA_TOKEN_OP_BIT_NOT);
		case '?':
			return token(JAVA_TOKEN_QMARK);
		case ':':
			return token(JAVA_TOKEN_COLON);
		case ';':
			return token(JAVA_TOKEN_SEMICOLON);
		case '+':
			switch (c = next) {
				case '+':
					return token(JAVA_TOKEN_OP_INCR);
				case '=':
					return token(JAVA_TOKEN_OP_PLUS_ASSIGN);
				default: {
					return token(JAVA_TOKEN_OP_PLUS);
				}
			}
		case '-':
			switch (c = next) {
				case '+':
					return token(JAVA_TOKEN_OP_DECR);
				case '=':
					return token(JAVA_TOKEN_OP_MINUS_ASSIGN);
				default:
					return undo, token(JAVA_TOKEN_OP_MINUS);
			}
		case '*':
			switch (c = next) {
				case '=':
					return token(JAVA_TOKEN_OP_MUL_ASSIGN);
				default:
					return undo, token(JAVA_TOKEN_OP_MINUS);
			}
		case '/':
			switch (c = next) {
				case '/':
					while ((c = next) != '\n' && c);
					undo;
					goto scan;
				case '=':
					return token(JAVA_TOKEN_OP_DIV_ASSIGN);
				default:
					return undo, token(JAVA_TOKEN_OP_DIV);
				case '*':
					while (1) {
						c = next;
						if (c == '\n') self->lineno++;
						if (c == '*') {
							if ((c = next) == '/') {
								goto scan;
							} else {
								undo;
							}
						}
						if (c == 0) {
							error("Unclose comment");
							return token(JAVA_TOKEN_TOKEN_ILLEGAL);
						}
					}
			}
		case '>':
			switch (c = next) {
				case '=':
					return token(JAVA_TOKEN_OP_GTE);
				case '>':
					return '=' == next ? token(JAVA_TOKEN_OP_BIT_SHR_ASSIGN) : (undo, token(JAVA_TOKEN_OP_BIT_SHR));
				default:
					return undo, token(JAVA_TOKEN_OP_GT);
			}
		case '<':
			switch (c = next) {
				case '=':
					return token(JAVA_TOKEN_OP_LTE);
				case '<':
					return '=' == next ? token(JAVA_TOKEN_OP_BIT_SHR_ASSIGN) : (undo, token(JAVA_TOKEN_OP_BIT_SHR));
				default:
					return token(JAVA_TOKEN_OP_LT);
			}
		case '|':
			return '|' == next ? token(JAVA_TOKEN_OP_OR) : token(JAVA_TOKEN_OP_BIT_OR);
		case '&':
			return '&' == next ? token(JAVA_TOKEN_OP_AND) : token(JAVA_TOKEN_OP_BIT_AND);
		case '"':
			return scan_string(self, c);
		case 0:
			token(JAVA_TOKEN_TOKEN_EOF);
		default:
			if (isalpha(c) || c == '_') return scan_ident(self, c);
			if (isdigit(c) || c == '.') return scan_number(self, c);
			token(JAVA_TOKEN_TOKEN_ILLEGAL);
			error("Illegal Token");
			return 0;
	}

}