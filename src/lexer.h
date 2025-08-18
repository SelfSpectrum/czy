#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Token Token;
typedef enum TokenType TokenType;
typedef struct TokenNode TokenNode;
typedef struct TokenQueue TokenQueue;

enum TokenType {
    // Data types
    TOK_INT,
    TOK_FLOAT,
    TOK_CHAR,
    TOK_VOID,
    TOK_DOUBLE,
    TOK_SHORT,
    TOK_LONG,
    TOK_SIGNED,
    TOK_UNSIGNED,
    
    // Pointer types
    TOK_INTP,
    TOK_FLOATP,
    TOK_CHARP,
    TOK_VOIDP,
    TOK_DOUBLEP,
    
    // Literals
    TOK_INTLIT,
    TOK_CHARLIT,
    TOK_FLOATLIT,
    TOK_STRINGLIT,
    
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
    TOK_EQ,
    TOK_NE,
    TOK_LT,
    TOK_GT,
    TOK_LE,
    TOK_GE,
    
    // Logical operators
    TOK_AND,
    TOK_OR,
    TOK_NOT,
    
    // Bitwise operators
    TOK_BITAND,
    TOK_BITOR,
    TOK_BITNOT,
    TOK_XOR,
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
};

struct Token {
    TokenType type;
    char *value;  // Renamed from 'id' to be more generic
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
Token GetNextToken(char **input, int *line, int *column);
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