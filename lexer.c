#include "lexer.h"

Token GetNextToken(char **input) {
	while (isspace(**input)) (*input)++;
	if (**input == '\0') return (Token) { .type = TOK_EOF };

	switch (**input) {
		case '(': (*input)++; return (Token) { TOK_OPENPARENTHESIS, "(" };
		case ')': (*input)++; return (Token) { TOK_CLOSEPARENTHESIS, ")" };
		case '{': (*input)++; return (Token) { TOK_OPENCURLYBRACES, "{" };
		case '}': (*input)++; return (Token) { TOK_CLOSECURLYBRACES, "}" };
		case ';': (*input)++; return (Token) { TOK_SEMICOLON, ";" };
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
int TokenPrint(Token token) {
	if (token.id == NULL) return 0;
	char *type[] = {"TOK_INT",
			"TOK_FLOAT",
			"TOK_CHAR",
			"TOK_VOID",
			"TOK_INTP",
			"TOK_FLOATP",
			"TOK_CHARP",
			"TOK_VOIDP",
			"TOK_INTLIT",
			"TOK_CHARLIT",
			"TOK_FLOATLIT",
			"TOK_ID",
			"TOK_ASSIGN",
			"TOK_RETURN",
			"TOK_OPENPARENTHESIS",
			"TOK_CLOSEPARENTHESIS",
			"TOK_OPENCURLYBRACES",
			"TOK_CLOSECURLYBRACES",
			"TOK_SEMICOLON",
			"TOK_EOF"};
	printf("[%s, \"%s\"] ", type[(int) token.type] ,token.id);
	return 1;
}
int TokenFree(Token *token) {
	switch (token->type) {
		case TOK_INT:
		case TOK_RETURN:
		case TOK_ID:
		case TOK_INTLIT:
			free(token->id);
			return 1;
		default: return 0;
	}
}
int TokenExpect(TokenQueue *q, TokenType type) {
	return TokenQueuePeek(q).type == type;
}
int TokenIsType(TokenQueue *q) {
	switch (q->first->token.type) {
		case TOK_INT:
		case TOK_CHAR:
		case TOK_FLOAT:
		case TOK_INTP:
		case TOK_CHARP:
		case TOK_FLOATP:
			return 1;
		default: return 0;
	}
}
int TokenQueuePush(TokenQueue *q, Token token) {
	TokenNode *node = (TokenNode *) malloc(sizeof(TokenNode));
	if (node == NULL) return 0;

	node->token = token;
	node->prev = NULL;

	if (q->length) {
		q->last->prev = node;
		q->last = q->last->prev;
		node = NULL;
	}
	else {
		q->first = node;
		q->last = node;
	}
	q->length++;

	return 1;
}
Token TokenQueuePop(TokenQueue *q) {
	TokenNode *node;
	Token token;
	if (q->length == 0) return (Token) { TOK_EOF, NULL };

	node = q->first;
	q->first = q->first->prev;
	node->prev = NULL;
	q->length--;

	token = node->token;
	free(node);
	printf("TokenQueue's current size %d.\n", q->length);

	return token;
}
Token TokenQueuePeek(TokenQueue *q) {
	if (q->length == 0) return (Token) { TOK_EOF, NULL };

	return q->first->token;
}
int TokenQueuePrint(TokenQueue q) {
	int i;
	TokenNode *node;
	if (q.length == 0) return 0;

	for (i = 0, node = q.first; i < q.length; i++, node = node->prev) {
		TokenPrint(node->token);
	}
	node = NULL;
	return 1;
}
int TokenQueueFree(TokenQueue *q) {
	TokenNode *node;
	Token token;
	if (q->length == 0) return 0;

	int i;
	int goal = q->length;
	q->last = NULL;
	for (i = 0; i < goal; i++) {
		node = q->first;
		q->first = node->prev;
		token = node->token;
		node->prev = NULL;
		TokenFree(&token);
		free(node);
	}

	q->length = 0;
	return 1;
}