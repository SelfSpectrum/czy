#include <stdio.h>
#include <string.h>

typedef enum TokenType TokenType;
typedef struct Token Token;

enum TokenType {
	TOK_INT = 0,
	TOK_ID,
	TOK_RETURN,
	TOK_OPENPARENTHESIS,
	TOK_CLOSEPARENTHESIS,
	TOK_OPENCURLYBRACES,
	TOK_CLOSECURLYBRACES,
	TOK_SEMICOLON
};

struct Token {
	TokenType type;
	char *id;
};

int PrintToken(Token token);

int main() {
	char czy[] = "int main() { return 0; }";
	char del[] = " (){};";
	char *ptr;
	
	Token token = { 0 };
	token.type = TOK_INT;
	token.id = strtok_r(czy, del, &ptr);
	PrintToken(token);
	token.type = TOK_ID;
	token.id = strtok_r(NULL, del, &ptr);
	PrintToken(token);

	return 0;
}

int PrintToken(Token token) {
	char *type[] = {"TOK_INT",
			"TOK_ID",
			"TOK_RETURN",
			"TOK_OPENPARENTHESIS",
			"TOK_CLOSEPARENTHESIS",
			"TOK_OPENCURLYBRACES",
			"TOK_CLOSECURLYBRACES",
			"TOK_SEMICOLON"};
	printf("[%s, %s] ", type[(int) token.type] ,token.id);
	return 1;
}
