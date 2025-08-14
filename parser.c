#include "parser.h"

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