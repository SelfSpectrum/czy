#include "lexer.h"

// Helper function to check for keywords
static bool is_keyword(const char *str) {
	static const char *keywords[] = {"int", "float", "char",
					 "void", "double", "short",
					 "long", "signed", "unsigned",
					 "return", "if", "else",
					 "while", "for", "do",
					 "switch", "case", "default",
					 "break", "continue", "typedef",
					 "struct", "union","enum",
					 "sizeof", NULL};
	for (const char **kw = keywords; *kw; kw++) {
		if (strcmp(str, *kw) == 0) return true;
	}
	return false;
}

// Helper function to convert keyword to token type
static TokenType keyword_to_token_type(const char *str) {
	if (strcmp(str, "int") == 0) return TOK_INT;
	if (strcmp(str, "float") == 0) return TOK_FLOAT;
	if (strcmp(str, "char") == 0) return TOK_CHAR;
	if (strcmp(str, "void") == 0) return TOK_VOID;
	if (strcmp(str, "double") == 0) return TOK_DOUBLE;
	if (strcmp(str, "short") == 0) return TOK_SHORT;
	if (strcmp(str, "long") == 0) return TOK_LONG;
	if (strcmp(str, "signed") == 0) return TOK_SIGNED;
	if (strcmp(str, "unsigned") == 0) return TOK_UNSIGNED;
	if (strcmp(str, "return") == 0) return TOK_RETURN;
	if (strcmp(str, "if") == 0) return TOK_IF;
	if (strcmp(str, "else") == 0) return TOK_ELSE;
	if (strcmp(str, "while") == 0) return TOK_WHILE;
	if (strcmp(str, "for") == 0) return TOK_FOR;
	if (strcmp(str, "do") == 0) return TOK_DO;
	if (strcmp(str, "switch") == 0) return TOK_SWITCH;
	if (strcmp(str, "case") == 0) return TOK_CASE;
	if (strcmp(str, "default") == 0) return TOK_DEFAULT;
	if (strcmp(str, "break") == 0) return TOK_BREAK;
	if (strcmp(str, "continue") == 0) return TOK_CONTINUE;
	if (strcmp(str, "typedef") == 0) return TOK_TYPEDEF;
	if (strcmp(str, "struct") == 0) return TOK_STRUCT;
	if (strcmp(str, "union") == 0) return TOK_UNION;
	if (strcmp(str, "enum") == 0) return TOK_ENUM;
	if (strcmp(str, "sizeof") == 0) return TOK_SIZEOF;
	return TOK_ID;
}
Token GetNextToken(char **input, int *line, int *column) {
	while (isspace(**input)) {
		if (**input == '\n') {
			(*line)++;
			*column = 0;
		}
		else (*column)++;
		(*input)++;
	}
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
	static const char *type[] = {};
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