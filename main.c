#include "lexer.h"

int main() {
	java_lexer_t lexer;
	java_lexer_init(&lexer, "\"asdsad\"+;*=+=++", "sdsa");
	java_scan(&lexer);
	java_token_inspect(&(lexer.tok));
	java_scan(&lexer);
	java_token_inspect(&(lexer.tok));
	java_scan(&lexer);
	java_token_inspect(&(lexer.tok));
	java_scan(&lexer);
	java_token_inspect(&(lexer.tok));
	java_scan(&lexer);
	java_token_inspect(&(lexer.tok));
	return 0;
}