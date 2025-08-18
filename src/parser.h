#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct ASTNode ASTNode;
typedef enum NodeType NodeType;
typedef struct ASTNodeNode ASTNodeNode;
typedef struct ASTQueue ASTQueue;

enum NodeType{
	AST_EXPRESSION = 0,
	AST_SCOPE,
	AST_RETURN,
	AST_FUNCTION,
	AST_INTLIT,
	AST_CHARLIT,
	AST_FLOATLIT,
	AST_STRINGLIT,
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
		// AST_FUNCTION
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
		// AST_STRINGLIT; I'm gonna
		char *stringLit;
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

ASTNode *ASTParseScope(TokenQueue *q1, TokenQueue *q2);
ASTNode *ASTParseExpression(TokenQueue *q1, TokenQueue *q2);
ASTNode *ASTParseFunction(TokenQueue *q1, TokenQueue *q2);
ASTNode *ASTParseValue(TokenQueue *q1, TokenQueue *q2);
ASTNode *ASTParseReturn(TokenQueue *q1, TokenQueue *q2);
int ASTNodeFree(ASTNode **node);
void ASTVisualize(ASTNode *node);
int ASTQueuePush(ASTQueue *q, ASTNode *node);
ASTNode *ASTQueuePop(ASTQueue *q);
ASTNode *ASTQueuePeek(ASTQueue *q);
int ASTQueueFree(ASTQueue *q);

#endif