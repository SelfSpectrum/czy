#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node Node;
typedef struct Queue Queue;
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
struct Node {
	Token token;
	Node *prev;
};
struct Queue{
	Node *first;
	Node *last;
	int length;
};

Token GetNextToken(char **input);
int TokenPrint(Token token);
int TokenFree(Token *token);
int QueuePush(Queue *queue, Token token);
Token QueuePop(Queue *queue);
int QueuePrint(Queue queue);
int QueueFree(Queue *queue);

int main() {
	Queue queue = { NULL, NULL, 0 };
	char *czy = "int main() { return 0; }";
	Token token;
	while (1) {
		token = GetNextToken(&czy);
		if (token.type == TOK_EOF) break;
		QueuePush(&queue, token);
	}
	QueuePrint(queue);
	QueueFree(&queue);
	return 0;
}

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
			"TOK_INTLIT",
			"TOK_ID",
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
int QueuePush(Queue *queue, Token token) {
	Node *node = (Node *) malloc(sizeof(Node));
	if (node == NULL) return 0;

	node->token = token;
	node->prev = NULL;

	if (queue->length) {
		queue->last->prev = node;
		queue->last = queue->last->prev;
		node = NULL;
	}
	else {
		queue->first = node;
		queue->last = node;
	}
	queue->length++;

	return 1;
}
Token QueuePop(Queue *queue) {
	Node *node;
	Token token;
	if (queue->length == 0) return (Token) { TOK_EOF, NULL };

	node = queue->first;
	queue->first = queue->first->prev;
	node->prev = NULL;
	queue->length--;

	token = node->token;
	free(node);
	printf("Queue's current size %d.\n", queue->length);

	return token;
}
int QueuePrint(Queue queue) {
	int i;
	Node *node;
	if (queue.length == 0) return 0;

	for (i = 0, node = queue.first; i < queue.length; i++, node = node->prev) {
		TokenPrint(node->token);
	}
	node = NULL;
	return 1;
}
int QueueFree(Queue *queue) {
	Node *node;
	Token token;
	if (queue->length == 0) return 0;

	int i;
	int goal = queue->length;
	queue->last = NULL;
	for (i = 0; i < goal; i++) {
		node = queue->first;
		queue->first = node->prev;
		token = node->token;
		node->prev = NULL;
		TokenFree(&token);
		free(node);
	}

	queue->length = 0;
	return 1;
}
