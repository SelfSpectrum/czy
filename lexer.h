#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Token Token;
typedef enum TokenType TokenType;
typedef enum NodeType NodeType;
typedef struct TokenNode TokenNode;
typedef struct TokenQueue TokenQueue;

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
struct TokenNode {
	Token token;
	TokenNode *prev;
};
struct TokenQueue{
	TokenNode *first;
	TokenNode *last;
	int length;
};

Token GetNextToken(char **input);
int TokenPrint(Token token);
int TokenFree(Token *token);
int TokenExpect(TokenQueue *q, TokenType type);
int TokenIsType(TokenQueue *q);
int TokenQueuePush(TokenQueue *q, Token token);
Token TokenQueuePop(TokenQueue *q);
Token TokenQueuePeek(TokenQueue *q);
int TokenQueuePrint(TokenQueue q);
int TokenQueueFree(TokenQueue *q);

#endif