#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum TokenType TokenType;
typedef struct Token Token;
typedef struct Node Node;
typedef struct Queue Queue;
typedef enum NodeType NodeType;
typedef struct ASTNode ASTNode;

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
enum NodeType{
	AST_FUNCTION = 0,
	AST_RETURN,
	AST_INTLIT,
	AST_BINARYOP
};
struct ASTNode{
	NodeType type;
	union {
		// AST_FUNCTION
		struct {
			char *name;
			ASTNode *body;
		} Function;
		// AST_RETURN
		struct {
			ASTNode *expression;
		} ReturnStatement;
		// AST_INTLIT; that's it, an int
		int intLit;
		// AST_BINARYOP; left op right; ej: 2 - 3; 'a' + 'b'; 3 & 10
		struct BinaryOp{
			ASTNode *left;
			ASTNode *right;
			ASTNode *op;
		} BinaryOp;
	};
};

Token GetNextToken(char **input);
int TokenPrint(Token token);
int TokenFree(Token *token);
int QueuePush(Queue *q, Token token);
Token QueuePop(Queue *q);
Token QueuePeek(Queue *q);
int QueuePrint(Queue q);
int QueueFree(Queue *q);
ASTNode *ASTParseExpression(Queue *q);
ASTNode *ASTParseReturn(Queue *q);
ASTNode *ASTParseFunction(Queue *q);

int main() {
	Queue q1 = { NULL, NULL, 0 };
	Queue q2 = { NULL, NULL, 0 };
	char *czy = "int main() { return 0; }";
	Token token;
	while (1) {
		token = GetNextToken(&czy);
		if (token.type == TOK_EOF) break;
		QueuePush(&q1, token);
	}
	QueuePrint(q1);
	QueueFree(&q1);
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
int TokenExpect(Queue *q, TokenType type) {
	return QueuePeek(q).type == type;
}
int QueuePush(Queue *q, Token token) {
	Node *node = (Node *) malloc(sizeof(Node));
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
Token QueuePop(Queue *q) {
	Node *node;
	Token token;
	if (q->length == 0) return (Token) { TOK_EOF, NULL };

	node = q->first;
	q->first = q->first->prev;
	node->prev = NULL;
	q->length--;

	token = node->token;
	free(node);
	printf("Queue's current size %d.\n", q->length);

	return token;
}
Token QueuePeek(Queue *q) {
	if (q->length == 0) return (Token) { TOK_EOF, NULL };

	return q->first->token;
}
int QueuePrint(Queue q) {
	int i;
	Node *node;
	if (q.length == 0) return 0;

	for (i = 0, node = q.first; i < q.length; i++, node = node->prev) {
		TokenPrint(node->token);
	}
	node = NULL;
	return 1;
}
int QueueFree(Queue *q) {
	Node *node;
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
ASTNode *ASTParseExpression(Queue *q) {
	if (q->length == 0) return NULL;
	if (TokenExpect(q, TOK_INTLIT)) {
		ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
		return node;
	}
	fprintf(stderr, "Unexpected token in expression: %s\n", QueuePeek(q).id);
	exit(1);
}
