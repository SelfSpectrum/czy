#include <stdio.h>
#include <string.h>

typedef enum TokenType TokenType;
typedef struct Token Token;

enum TokenType {
	TOK_INT = 0,
	TOK_ID,
	TOK_RETURN
};

struct Token {
	TokenType type;
	char *id;
};

int PrintToken(Token token);

int main() {
	char *czy = "int";
	char *ptr;
	
	Token token = strtok_r(czy, " ", &ptr);
	
	PrintToken(token);

	return 0;
}

int PrintToken(Token token) {
	printf("[%s]", token.id);
	return 1;
}
