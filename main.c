#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum TokenType TokenType;
typedef struct Token Token;

enum TokenType {
	TOK_INT = 0,
	TOK_INTLIT,
	TOK_ID,
	TOK_RETURN,
	TOK_OPENPARENTHESIS,
	TOK_CLOSEPARENTHESIS,
	TOK_OPENCURLYBRACES,
	TOK_CLOSECURLYBRACES,
	TOK_SEMICOLON,
	TOK_EOF
};

struct Token {
	TokenType type;
	char *id;
};

Token GetNextToken(char **input);
int PrintToken(Token token);

int main() {
	char *czy = "int main() { return 0; }";
	Token token;
	while (1) {
		token = GetNextToken(&czy);
		if (token.type == TOK_EOF) break;
		PrintToken(token);
		switch (token.type) {
			case TOK_INT:
			case TOK_RETURN:
			case TOK_ID:
			case TOK_INTLIT:
				free(token.id);
				break;
			default: break;
		}
	}
	return 0;
}

Token GetNextToken(char **input) {
	while (isspace(**input)) (*input)++;
	if (**input == '\0') return (Token) { .type = TOK_EOF };

	printf("\nTest.\n");
	switch (**input) {
		case '(': return (Token) { TOK_OPENPARENTHESIS, "(" };
		case ')': return (Token) { TOK_CLOSEPARENTHESIS, ")" };
		case '{': return (Token) { TOK_OPENCURLYBRACES, "{" };
		case '}': return (Token) { TOK_CLOSECURLYBRACES, "}" };
		case ';': return (Token) { TOK_SEMICOLON, ";" };
	}
	
	if (isalpha(**input)) {
		char *start = *input;
		while (isalnum(**input)) (*input)++;
		size_t len = *input - start;
		char *id = strndup(start, len);

		if (strcmp(id, "int"))		return (Token) { TOK_INT, id };
		else if (strcmp(id, "return"))	return (Token) { TOK_RETURN, id };
		else				return (Token) { TOK_ID, id };
	}

	if (isdigit(**input)) {
		char *start = *input;
		while (isdigit(**input)) (*input)++;
		return (Token) { TOK_INTLIT, strndup(start, *input - start) };
	}
	fprintf(stderr, "Unknown character: %c\n", **input);
	exit(1);
}
int PrintToken(Token token) {
	if (token.id == NULL) return 0;
	char *type[] = {"TOK_INT",
			"TOK_INTLIT",
			"TOK_ID",
			"TOK_RETURN",
			"TOK_OPENPARENTHESIS",
			"TOK_CLOSEPARENTHESIS",
			"TOK_OPENCURLYBRACES",
			"TOK_CLOSECURLYBRACES",
			"TOK_SEMICOLON",
			"TOK_EOF"};
	printf("[%s, %s] ", type[(int) token.type] ,token.id);
	return 1;
}
