#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dataStructures.h"

typedef struct Token Token;
typedef enum TokenType TokenType;
typedef enum NodeType NodeType;
typedef struct ASTNode ASTNode;
typedef struct ASTNodeNode ASTNodeNode;
typedef struct ASTQueue ASTQueue;

enum TokenType {
	TOK_INT = 0,
	TOK_FLOAT,
	TOK_CHAR,
	TOK_VOID,
	TOK_INTP,
	TOK_FLOATP,
	TOK_CHARP,
	TOK_VOIDP,
	TOK_INTLIT,
	TOK_CHARLIT,
	TOK_FLOATLIT,
	TOK_ID,
	TOK_ASSIGN,
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
enum NodeType{
	AST_EXPRESSION = 0,
	AST_SCOPE,
	AST_RETURN,
	AST_FUNCTION,
	AST_INTLIT,
	AST_CHARLIT,
	AST_FLOATLIT,
	AST_TERNARYOP,
	AST_BINARYOP,
	AST_UNARYOP,
	AST_ERROR
};
struct ASTNode{
	NodeType type;
	union {
		// AST_EXPRESSION
		struct {
			char *type;
			char *name;
			ASTNode *body;
		} expression;
		// AST_SCOPE
		struct {
			ASTQueue *body;
		} scope;
		// AST_RETURN
		struct {
			ASTNode *expression;
		} returnStatement;
		// AST_INTLIT; that's it, an int
		int intLit;
		// AST_CHARLIT; that's it, an char
		char charLit;
		// AST_FLOATLIT; that's it, an float
		float floatLit;
		// AST_BINARYOP; left op right; ej: 2 - 3; 'a' + 'b'; 3 & 10
		struct {
			ASTNode *left;
			ASTNode *right;
			ASTNode *op;
		} binaryOp;
		// AST_UNARYOP; op value; ej: -2; !true; ~0xFF
		struct unaryOp {
			ASTNode *value;
			ASTNode *op;
		} unaryOp;
		// AST_TERNARYOP; condition ? true : false; ej: 2 > 3 ? 4 : 5
		struct {
			ASTNode *condition;
			ASTNode *trueValue;
			ASTNode *falseValue;
		} ternaryOp;
	};
};
struct ASTNodeNode {
	ASTNode *node;
	ASTNodeNode *prev;
};
struct ASTQueue {
	ASTNodeNode *first;
	ASTNodeNode *last;
	int length;
};

Token GetNextToken(char **input);
int TokenPrint(Token token);
int TokenFree(Token *token);
int TokenExpect(TokenQueue *q, TokenType type);
int TokenIsType(TokenQueue *q);
ASTNode *ASTParseScope(TokenQueue *q1, TokenQueue *q2);
ASTNode *ASTParseExpression(TokenQueue *q1, TokenQueue *q2);
ASTNode *ASTParseFunction(TokenQueue *q1, TokenQueue *q2);
ASTNode *ASTParseValue(TokenQueue *q1, TokenQueue *q2);
ASTNode *ASTParseReturn(TokenQueue *q1, TokenQueue *q2);
void ASTVisualize(ASTNode *node);
int ASTQueuePush(ASTQueue *q, ASTNode *node);
ASTNode *ASTQueuePop(ASTQueue *q);
ASTNode *ASTQueuePeek(ASTQueue *q);
int ASTQueueFree(ASTQueue *q);

int main() {
	TokenQueue q1 = { NULL, NULL, 0 };
	TokenQueue q2 = { NULL, NULL, 0 };
	char *czy = "int main() { return 0; }";
	Token token;
	while (1) {
		token = GetNextToken(&czy);
		if (token.type == TOK_EOF) break;
		TokenQueuePush(&q1, token);
	}
	TokenQueuePrint(q1);
	TokenQueueFree(&q1);
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
ASTNode *ASTParseScope(TokenQueue *q1, TokenQueue *q2) {
	if (q1->length < 2) {
		fprintf(stderr, "Insufficient tokens in scope.\n");
		exit(1);
	}

	ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
	if (node == NULL) return NULL;

	node->type = AST_SCOPE;
	if (!TokenExpect(q1, TOK_OPENCURLYBRACES)) {
		fprintf(stderr, "Unexpected token in scope: %s\nAn opening curly brace was expected.\n", TokenQueuePeek(q1).id);
		exit(1);
	}
	TokenQueuePush(q2, TokenQueuePop(q1));

	while (q1->length > 0 && !TokenExpect(q1, TOK_CLOSECURLYBRACES)) {
		if (TokenExpect(q1, TOK_RETURN)) {
			node->returnStatement.expression = ASTParseReturn(q1, q2);
			continue;
		}
		else if (TokenIsType(q1)) {
			node->expression.body = ASTParseExpression(q1, q2);
			continue;
		}
		else if (TokenExpect(q1, TOK_ID)) {
			node->expression.body = ASTParseFunction(q1, q2);
			continue;
		}
		else if (TokenExpect(q1, TOK_INTLIT)) {
			node->intLit = ASTParseValue(q1, q2)->intLit;
			continue;
		}
		else {
			fprintf(stderr, "Unexpected token in scope: %s\n", TokenQueuePeek(q1).id);
			exit(1);
		}
	}

	if (!TokenExpect(q1, TOK_CLOSECURLYBRACES)) {
		fprintf(stderr, "Unexpected end of scope: %s\nA closing curly brace was expected.\n", TokenQueuePeek(q1).id);
		exit(1);
	}
	
	return node;
}
ASTNode *ASTParseExpression(TokenQueue *q1, TokenQueue *q2) {
	if (q1->length <= 2) {
		fprintf(stderr, "Insufficient tokens in expression.\n");
		exit(1);
	}

	ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
	if (node == NULL) return NULL;

	node->type = AST_EXPRESSION;
	if (!TokenIsType(q1)) {
		fprintf(stderr, "Unexpected token in expression: %s\nA type was expected.\n", TokenQueuePeek(q1).id);
		exit(1);
	}
	node->expression.type = strdup(TokenQueuePeek(q1).id);

	TokenQueuePush(q2, TokenQueuePop(q1));
	if (!TokenExpect(q1, TOK_ID)) {
		fprintf(stderr, "Unexpected token in expression: %s\nAn ID was expected.\n", TokenQueuePeek(q1).id);
		exit(1);
	}
	node->expression.name = strdup(TokenQueuePeek(q1).id);

	TokenQueuePush(q2, TokenQueuePop(q1));
	return node;
}
ASTNode *ASTParseFunction(TokenQueue *q1, TokenQueue *q2) {
	if (q1->length < 1) {
		fprintf(stderr, "Insufficient tokens in expression.\n");
		exit(1);
	}

	ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
	if (node == NULL) return NULL;

	node->type = AST_FUNCTION;
	if (!TokenExpect(q1, TOK_OPENPARENTHESIS)) {
		fprintf(stderr, "Unexpected token in expression: %s\nA type was expected.\n", TokenQueuePeek(q1).id);
		exit(1);
	}
	node->expression.type = strdup(TokenQueuePeek(q1).id);

	return node;

}
ASTNode *ASTParseValue(TokenQueue *q1, TokenQueue *q2) {
	if (q1->length == 0) {
		fprintf(stderr, "Insufficient tokens in expression.\n");
		exit(1);
	}

	ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
	if (node == NULL) return NULL;

	node->type = AST_INTLIT;
	node->intLit = atoi(TokenQueuePeek(q1).id);
	return node;
}
ASTNode *ASTParseReturn(TokenQueue *q1, TokenQueue *q2) {
	if (q1->length == 0) {
		fprintf(stderr, "Insufficient tokens in expression.\n");
		exit(1);
	}

	ASTNode *node = (ASTNode *) malloc(sizeof(ASTNode));
	if (node == NULL) return NULL;

	return node;
}
void ASTVisualize(ASTNode *node) {

}
int ASTQueuePush(ASTQueue *q, ASTNode *node) {
	ASTNodeNode *temp = (ASTNodeNode *) malloc(sizeof(ASTNodeNode));
	if (node == NULL) return 0;

	temp->node = node;
	temp->prev = NULL;

	if (q->length) {
		q->last->prev = temp;
		q->last = q->last->prev;
		temp = NULL;
	}
	else {
		q->first = temp;
		q->last = temp;
	}
	q->length++;

	return 1;
}
ASTNode *ASTQueuePop(ASTQueue *q) {
	ASTNodeNode *temp;
	ASTNode *node = NULL;
	if (q->length == 0) return NULL;
	
	temp = q->first;
	q->first = q->first->prev;
	temp->prev = NULL;
	q->length--;
	
	node = temp->node;
	free(temp);

	return node;
}
int ASTQueueFree(ASTQueue *q) {
	ASTNodeNode *node;
	ASTNode *astNode;
	if (q->length == 0) return 0;

	int i;
	int goal = q->length;
	q->last = NULL;
	for (i = 0; i < goal; i++) {
		node = q->first;
		q->first = node->prev;
		astNode = node->node;
		node->prev = NULL;
		free(astNode);
		free(node);
	}

	q->length = 0;
	return 1;
}