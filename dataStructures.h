#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

typedef struct TokenNode TokenNode;
typedef struct TokenQueue TokenQueue;

struct TokenNode {
	Token token;
	TokenNode *prev;
};
struct TokenQueue{
	TokenNode *first;
	TokenNode *last;
	int length;
};

int TokenQueuePush(TokenQueue *q, Token token);
Token TokenQueuePop(TokenQueue *q);
Token TokenQueuePeek(TokenQueue *q);
int TokenQueuePrint(TokenQueue q);
int TokenQueueFree(TokenQueue *q);

#endif