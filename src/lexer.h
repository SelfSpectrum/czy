#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Token Token;
typedef struct TokenNode TokenNode;
typedef struct TokenQueue TokenQueue;

typedef enum TokenType {
	// Data types
	TOK_INT,
	TOK_FLOAT,
	TOK_CHAR,
	TOK_VOID,
	TOK_DOUBLE,
	TOK_LONGDOUBLE,
	TOK_LONG,
	TOK_SHORT,
	TOK_LONGLONG,
	TOK_SIGNED,
	TOK_UNSIGNED,
	TOK_BOOL,			// This is in C, sure, but making sure to add it native in Czy

	// Pointer types
	TOK_INTP,
	TOK_FLOATP,
	TOK_CHARP,
	TOK_VOIDP,
	TOK_DOUBLEP,
	TOK_LONGDOUBLEP,
	TOK_LONGP,
	TOK_SHORTP,
	TOK_LONGLONGP,
	TOK_SIGNEDP,
	TOK_UNSIGNEDP,
	TOK_BOOLP,

	// Literals
	TOK_INTLIT,
	TOK_CHARLIT,
	TOK_FLOATLIT,
	TOK_DOUBLELIT,
	TOK_LONGDOUBLELIT,
	TOK_STRINGLIT,
	TOK_BOOLLIT,

	// Identifiers
	TOK_ID,

	// Keywords
	TOK_RETURN,
	TOK_IF,
	TOK_ELSE,
	TOK_WHILE,
	TOK_FOR,
	TOK_DO,
	TOK_SWITCH,
	TOK_CASE,
	TOK_DEFAULT,
	TOK_BREAK,
	TOK_CONTINUE,
	TOK_TYPEDEF,
	TOK_STRUCT,
	TOK_UNION,
	TOK_ENUM,
	TOK_SIZEOF,

	// Operators
	TOK_ASSIGN,
	TOK_PLUS,
	TOK_MINUS,
	TOK_STAR,
	TOK_SLASH,
	TOK_PERCENT,
	TOK_PLUSPLUS,
	TOK_MINUSMINUS,
	TOK_ARROW,
	TOK_DOT,

	// Comparison operators
	TOK_EQUAL,
	TOK_NOTEQUAL,
	TOK_LESSERTHAN,
	TOK_GREATERTHAN,
	TOK_LESSEROREQUAL,
	TOK_GREATEROREQUAL,

	// Logical operators
	TOK_AND,
	TOK_OR,
	TOK_NOT,

	// Bitwise operators
	TOK_BITAND,
	TOK_BITOR,
	TOK_BITNOT,
	TOK_BITXOR,
	TOK_LSHIFT,
	TOK_RSHIFT,

	// Punctuation
	TOK_OPENPARENTHESIS,
	TOK_CLOSEPARENTHESIS,
	TOK_OPENCURLYBRACES,
	TOK_CLOSECURLYBRACES,
	TOK_OPENBRACKET,
	TOK_CLOSEBRACKET,
	TOK_SEMICOLON,
	TOK_COLON,
	TOK_COMMA,
	TOK_QUESTION,

	// Special
	TOK_EOF,
	TOK_ERROR
} TokenType;

struct Token {
	TokenType type;
	char *value;
	int line;     // For error reporting
	int column;   // For error reporting
};

struct TokenNode {
	Token token;
	TokenNode *prev;
};

struct TokenQueue {
	TokenNode *first;
	TokenNode *last;
	int length;
};

// Function prototypes
TokenType TokenTypeParseString(const char *str);
Token GetNextToken(char **input, int *line, int *column);
bool TokenPrint(Token token);
bool TokenFree(Token *token);
bool TokenExpect(TokenQueue *q, TokenType type);
bool TokenIsDataType(TokenQueue *q);
bool TokenIsKeyword(const char *str);
bool TokenQueuePush(TokenQueue *q, Token token);
Token TokenQueuePop(TokenQueue *q);
Token TokenQueuePeek(TokenQueue *q);
bool TokenQueuePrint(TokenQueue q);
bool TokenQueueFree(TokenQueue *q);

#endif
