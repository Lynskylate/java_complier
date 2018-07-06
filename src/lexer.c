//
// Created by lynskylate on 2018/6/18.
//

#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <math.h>

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

#define error(message) (self->error=message, token(JAVA_TOKEN_ILLEGAL))

#define clear_token(tok) (free(buf), token(tok))

void java_lexer_init(java_lexer_t *self, char *source, const char *filename) {
	self->error = NULL;
	self->lineno = 1;
	self->filename = filename;
	self->offset = 0;
	self->source = source;
	self->tok.value.as_string = NULL;
	java_lexer_tok_reset(self);
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
	int buffer_len = 128;
	char *buf = (char *) malloc(buffer_len * sizeof(char));
	token(JAVA_TOKEN_ID);
	do {
		if (len >= buffer_len) {
			buffer_len = buffer_len + 128;
			buf = (char *) realloc(buf, buffer_len * sizeof(char));
		}
		buf[len++] = c;
	} while (isalpha(c = next) || c == '_' || c == '$');
	undo;

	buf[len] = 0;
	switch (len) {
		case 2: {
			if (!strcmp(buf, "if")) return clear_token(JAVA_TOKEN_IF);
			if (!strcmp(buf, "do")) return clear_token(JAVA_TOKEN_DO);
			break;
		}
		case 3: {
			if (!strcmp(buf, "for")) return clear_token(JAVA_TOKEN_FOR);
			if (!strcmp(buf, "int")) return clear_token(JAVA_TOKEN_INT);
			if (!strcmp(buf, "new")) return clear_token(JAVA_TOKEN_NEW);
			if (!strcmp(buf, "try")) return clear_token(JAVA_TOKEN_TRY);
			break;
		}
		case 4: {
			if (!strcmp(buf, "void")) return clear_token(JAVA_TOKEN_VOID);
			if (!strcmp(buf, "this")) return clear_token(JAVA_TOKEN_VOID);
			if (!strcmp(buf, "char")) return clear_token(JAVA_TOKEN_CHAR);
			if (!strcmp(buf, "byte")) return clear_token(JAVA_TOKEN_BYTE);
			if (!strcmp(buf, "case")) return clear_token(JAVA_TOKEN_CASE);
			if (!strcmp(buf, "else")) return clear_token(JAVA_TOKEN_ELSE);
			if (!strcmp(buf, "long")) return clear_token(JAVA_TOKEN_LONG);
			if (!strcmp(buf, "enum")) return clear_token(JAVA_TOKEN_ENUM);
			if (!strcmp(buf, "goto")) return clear_token(JAVA_TOKEN_GOTO);
			break;
		}
		case 5: {
			if (!strcmp(buf, "const")) return clear_token(JAVA_TOKEN_CONST);
			if (!strcmp(buf, "short")) return clear_token(JAVA_TOKEN_SHORT);
			if (!strcmp(buf, "break")) return clear_token(JAVA_TOKEN_BREAK);
			if (!strcmp(buf, "catch")) return clear_token(JAVA_TOKEN_CATCH);
			if (!strcmp(buf, "class")) return clear_token(JAVA_TOKEN_CLASS);
			if (!strcmp(buf, "final")) return clear_token(JAVA_TOKEN_FINAL);
			if (!strcmp(buf, "float")) return clear_token(JAVA_TOKEN_FLOAT);
			if (!strcmp(buf, "super")) return clear_token(JAVA_TOKEN_SUPER);
			if (!strcmp(buf, "throw")) return clear_token(JAVA_TOKEN_THROW);
			if (!strcmp(buf, "while")) return clear_token(JAVA_TOKEN_WHILE);
			break;
		}
		case 6: {
			if (!strcmp(buf, "throws")) return clear_token(JAVA_TOKEN_THROWS);
			if (!strcmp(buf, "native")) return clear_token(JAVA_TOKEN_NATIVE);
			if (!strcmp(buf, "static")) return clear_token(JAVA_TOKEN_STATIC);
			if (!strcmp(buf, "switch")) return clear_token(JAVA_TOKEN_SWITCH);
			if (!strcmp(buf, "import")) return clear_token(JAVA_TOKEN_IMPORT);
			if (!strcmp(buf, "double")) return clear_token(JAVA_TOKEN_DOUBLE);
			if (!strcmp(buf, "public")) return clear_token(JAVA_TOKEN_PUBLIC);
			if (!strcmp(buf, "return")) return clear_token(JAVA_TOKEN_RETURN);
			break;
		}
		case 7: {
			if (!strcmp(buf, "boolean")) return clear_token(JAVA_TOKEN_BOOLEAN);
			if (!strcmp(buf, "package")) return clear_token(JAVA_TOKEN_PACKAGE);
			if (!strcmp(buf, "private")) return clear_token(JAVA_TOKEN_PRIVATE);
			if (!strcmp(buf, "default")) return clear_token(JAVA_TOKEN_DEFAULT);
			if (!strcmp(buf, "extends")) return clear_token(JAVA_TOKEN_EXTENDS);
			if (!strcmp(buf, "finally")) return clear_token(JAVA_TOKEN_FINALLY);
			break;
		}
		case 8: {
			if (!strcmp(buf, "volatile")) return clear_token(JAVA_TOKEN_VOLATILE);
			if (!strcmp(buf, "abstract")) return clear_token(JAVA_TOKEN_ABSTRACT);
			if (!strcmp(buf, "continue")) return clear_token(JAVA_TOKEN_CONTINUE);
			break;
		}
		case 9: {
			if (!strcmp(buf, "interface")) return clear_token(JAVA_TOKEN_INTERFACE);
			if (!strcmp(buf, "protected")) return clear_token(JAVA_TOKEN_PROTECTED);
			if (!strcmp(buf, "transient")) return clear_token(JAVA_TOKEN_TRANSIENT);
			break;
		}
		case 10: {
			if (!strcmp(buf, "implements")) return clear_token(JAVA_TOKEN_IMPLEMENTS);
			if (!strcmp(buf, "instanceof")) return clear_token(JAVA_TOKEN_INSTANCEOF);
			break;
		}
		case 11: {
			if (!strcmp(buf, "synchronized")) return clear_token(JAVA_TOKEN_SYNCHRONIZED);
			break;
		}
	}
	self->tok.value.as_string = strdup(buf);
	free(buf);
	return 1;
}

static int scan_number(java_lexer_t *self, int c) {
	int n = 0, type = 0, expo = 0, e;
	int expo_type = 1;

	token(JAVA_TOKEN_CONSTANT_INT);

	switch (c) {
		case '0':
			goto scan_hex;
		default:
			goto scan_int;
	}

scan_hex:
	switch (c = next) {
		case 'x':
			if (!isxdigit(c = next)) {
				error("hex literal expects one or more digits");
				return 0;
			} else {
				do n = n << 4 | hex(c);
				while (isxdigit(c = next));
			}
			self->tok.value.as_int = n;
			undo;
			return 1;
		default:
			undo;
			c = '0';
			goto scan_int;
	}

	// [0-9_]+

scan_int:
	do {
		if ('_' == c) continue;
		else if ('.' == c) goto scan_float;
		else if ('e' == c || 'E' == c) goto scan_expo;
		n = n * 10 + c - '0';
	} while (isdigit(c = next) || '_' == c || '.' == c || 'e' == c || 'E' == c);
	undo;
	self->tok.value.as_int = n;
	return 1;

	// [0-9_]+

scan_float:
	{
		e = 1;
		type = 1;
		token(JAVA_TOKEN_CONSTANT_FLOAT);
		while (isdigit(c = next) || '_' == c || 'e' == c || 'E' == c) {
			if ('_' == c) continue;
			else if ('e' == c || 'E' == c) goto scan_expo;
			n = n * 10 + c - '0';
			e *= 10;
		}
		undo;
		self->tok.value.as_real = (float) n / e;
		return 1;
	}

	// [\+\-]?[0-9]+

scan_expo:
	{
		while (isdigit(c = next) || '+' == c || '-' == c) {
			if ('-' == c) {
				expo_type = 0;
				continue;
			}
			expo = expo * 10 + c - '0';
		}

		undo;
		if (expo_type == 0) expo *= -1;
		if (type == 0)
			self->tok.value.as_int = n * pow(10, expo);
		else
			self->tok.value.as_real = ((double) n / e) * pow(10, expo);
	}

	return 1;
}


int java_scan(java_lexer_t *self) {
	int c;

scan:
	switch (c = next) {
		case '\n':
			self->lineno += 1;
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
							return token(JAVA_TOKEN_ILLEGAL);
						}
					}
			}
		case '!':
			if (next == '='){
				return token(JAVA_TOKEN_OP_NEQ);
			}else{
				return token(JAVA_TOKEN_OP_NOT);
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
					return (undo, token(JAVA_TOKEN_OP_LT));
			}
		case '=':
			return '=' == next ? token(JAVA_TOKEN_OP_EQ) : (undo, token(JAVA_TOKEN_OP_ASSIGN));
		case '|':
			return '|' == next ? token(JAVA_TOKEN_OP_OR) : token(JAVA_TOKEN_OP_BIT_OR);
		case '&':
			return '&' == next ? token(JAVA_TOKEN_OP_AND) : token(JAVA_TOKEN_OP_BIT_AND);
		case '"':
			return scan_string(self, c);
		case 0:
			return token(JAVA_TOKEN_EOS);
		default:
			if (isalpha(c) || c == '_' || c == '$') return scan_ident(self, c);
			if (isdigit(c) || c == '.') return scan_number(self, c);
			token(JAVA_TOKEN_ILLEGAL);
			error("Illegal Token");
			return 0;
	}

}

int java_lexer_tok_reset(java_lexer_t *self) {
//	self->tok.token_type = NULL;
	if (self->tok.value.as_string != NULL) {
		free(self->tok.value.as_string);
	}
	self->tok.value.as_string = NULL;
	self->tok.value.as_real = 0;
	self->tok.value.as_int = 0;
	return 0;
}
