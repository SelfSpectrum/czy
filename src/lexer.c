#include "lexer.h"

TokenType TokenTypeParseString(const char *str) {
	if (strcmp(str, "int") == 0)		return TOK_INT;
	if (strcmp(str, "float") == 0)		return TOK_FLOAT;
	if (strcmp(str, "char") == 0)		return TOK_CHAR;
	if (strcmp(str, "void") == 0)		return TOK_VOID;
	if (strcmp(str, "double") == 0)		return TOK_DOUBLE;
	if (strcmp(str, "short") == 0)		return TOK_SHORT;
	if (strcmp(str, "long") == 0)		return TOK_LONG;
	if (strcmp(str, "signed") == 0)		return TOK_SIGNED;
	if (strcmp(str, "unsigned") == 0)	return TOK_UNSIGNED;
	if (strcmp(str, "return") == 0)		return TOK_RETURN;
	if (strcmp(str, "if") == 0)		return TOK_IF;
	if (strcmp(str, "else") == 0)		return TOK_ELSE;
	if (strcmp(str, "while") == 0)		return TOK_WHILE;
	if (strcmp(str, "for") == 0)		return TOK_FOR;
	if (strcmp(str, "do") == 0)		return TOK_DO;
	if (strcmp(str, "switch") == 0)		return TOK_SWITCH;
	if (strcmp(str, "case") == 0)		return TOK_CASE;
	if (strcmp(str, "default") == 0)	return TOK_DEFAULT;
	if (strcmp(str, "break") == 0)		return TOK_BREAK;
	if (strcmp(str, "continue") == 0)	return TOK_CONTINUE;
	if (strcmp(str, "typedef") == 0)	return TOK_TYPEDEF;
	if (strcmp(str, "struct") == 0)		return TOK_STRUCT;
	if (strcmp(str, "union") == 0)		return TOK_UNION;
	if (strcmp(str, "enum") == 0)		return TOK_ENUM;
	if (strcmp(str, "sizeof") == 0)		return TOK_SIZEOF;

						return TOK_ID;
}
Token GetNextToken(char **input, int *line, int *column) {
	// Ignore whitespace and track line/column numbers
	while (isspace(**input)) {
		if (**input == '\n') {
			(*line)++;
			*column = 0;
		}
		else (*column)++;
		(*input)++;
	}
	// End of input
	if (**input == '\0') return (Token) { TOK_EOF, NULL, *line, *column };
	// Handle singleline comments
	if (**input == '/' && (*input)[1] == '/') {
		// Ignore until end of line, then return recursively
		while (**input != '\n' && **input != '\0') (*input)++;
		return GetNextToken(input, line, column);
	}
	// Handle multiline comments
	if (**input == '/' && (*input)[1] == '*') {
		(*input) += 2;
		while (!(**input == '*' && (*input)[1] == '/') && **input != '\0') {
			if (**input == '\n') {
				(*line)++;
				*column = 1;
			}
			else {
				(*column)++;
			}
			(*input)++;
		}
		if (**input == '\0') {
			fprintf(stderr, "Error: Unclosed block comment at line %d, column %d\n", *line, *column);
			return (Token) { .type = TOK_ERROR, .value = NULL, .line = *line, .column = *column };
		}
		(*input) += 2;
		return GetNextToken(input, line, column);
	}

	// Handle multicharacter operators
	if (**input == '=' && (*input)[1] == '=') {
		(*input) += 2;
		return (Token) { TOK_EQUAL, "==", *line, *column - 1 };
	}
	if (**input == '!' && (*input)[1] == '=') {
		(*input) += 2;
		return (Token) { TOK_NOTEQUAL, "!=", *line, *column - 1 };
	}
	if (**input == '<' && (*input)[1] == '=') {
		(*input) += 2;
		return (Token) { TOK_LESSEROREQUAL, "<=", *line, *column - 1 };
	}
	if (**input == '>' && (*input)[1] == '=') {
		(*input) += 2;
		return (Token) { TOK_GREATEROREQUAL, ">=", *line, *column - 1 };
	}
	if (**input == '&' && (*input)[1] == '&') {
		(*input) += 2;
		return (Token) { TOK_AND, "&&", *line, *column - 1 };
	}
	if (**input == '|' && (*input)[1] == '|') {
		(*input) += 2;
		return (Token) { TOK_OR, "||", *line, *column - 1 };
	}
	if (**input == '+' && (*input)[1] == '+') {
		(*input) += 2;
		return (Token) { TOK_PLUSPLUS, "++", *line, *column - 1 };
	}
	if (**input == '-' && (*input)[1] == '-') {
		(*input) += 2;
		return (Token) { TOK_MINUSMINUS, "--", *line, *column - 1 };
	}
	if (**input == '-' && (*input)[1] == '>') {
		(*input) += 2;
		return (Token) { TOK_ARROW, "->", *line, *column - 1 };
	}
	if (**input == '<' && (*input)[1] == '<') {
		(*input) += 2;
		return (Token) { TOK_LSHIFT, "<<", *line, *column - 1 };
	}
	if (**input == '>' && (*input)[1] == '>') {
		(*input) += 2;
		return (Token) { TOK_RSHIFT, ">>", *line, *column - 1 };
	}

	// Handle singlecharacter tokens
	switch (**input) {
		case '(': (*input)++; (*column)++; return (Token) { TOK_OPENPARENTHESIS, "(", *line, *column - 1 };
		case ')': (*input)++; (*column)++; return (Token) { TOK_CLOSEPARENTHESIS, ")", *line, *column - 1 };
		case '{': (*input)++; (*column)++; return (Token) { TOK_OPENCURLYBRACES, "{", *line, *column - 1 };
		case '}': (*input)++; (*column)++; return (Token) { TOK_CLOSECURLYBRACES, "}", *line, *column - 1 };
		case '[': (*input)++; (*column)++; return (Token) { TOK_OPENBRAKET, "[", *line, *column - 1 };
		case ']': (*input)++; (*column)++; return (Token) { TOK_CLOSEBRAKET, "]", *line, *column - 1 };
		case ';': (*input)++; (*column)++; return (Token) { TOK_SEMICOLON, ";", *line, *column - 1 };
		case ',': (*input)++; (*column)++; return (Token) { TOK_COMMA, ",", *line, *column - 1 };
		case ':': (*input)++; (*column)++; return (Token) { TOK_COLON, ":", *line, *column - 1 };
		case '?': (*input)++; (*column)++; return (Token) { TOK_QUESTION, "?", *line, *column - 1 };
		case '!': (*input)++; (*column)++; return (Token) { TOK_NOT, "!", *line, *column - 1 };
		case '&': (*input)++; (*column)++; return (Token) { TOK_BITAND, "&", *line, *column - 1 };
		case '|': (*input)++; (*column)++; return (Token) { TOK_BITOR, "|", *line, *column - 1 };
		case '^': (*input)++; (*column)++; return (Token) { TOK_BITXOR, "^", *line, *column - 1 };
		case '~': (*input)++; (*column)++; return (Token) { TOK_BITNOT, "~", *line, *column - 1 };
		case '=': (*input)++; (*column)++; return (Token) { TOK_ASSIGN, "=", *line, *column - 1 };
		case '+': (*input)++; (*column)++; return (Token) { TOK_PLUS, "+", *line, *column - 1 };
		case '-': (*input)++; (*column)++; return (Token) { TOK_MINUS, "-", *line, *column - 1 };
		case '*': (*input)++; (*column)++; return (Token) { TOK_STAR, "*", *line, *column - 1 };
		case '/': (*input)++; (*column)++; return (Token) { TOK_SLASH, "/", *line, *column - 1 };
		case '%': (*input)++; (*column)++; return (Token) { TOK_PERCENT, "%", *line, *column - 1 };
		case '<': (*input)++; (*column)++; return (Token) { TOK_LESSERTHAN, "<", *line, *column - 1 };
		case '>': (*input)++; (*column)++; return (Token) { TOK_GREATERTHAN, ">", *line, *column - 1 };
		case '.': (*input)++; (*column)++; return (Token) { TOK_DOT, ".", *line, *column - 1 };
	}
	
	// Handle multicharacter tokens, identifiers and keywords,
	if (isalpha(**input)) {
		char *start = *input;
		int startColumn = *column;
		while (isalnum(**input) || **input == '_') {
			(*input)++;
			(*column)++;
		}
		size_t len = *input - start;
		char *value = strndup(start, len);

		if (TokenIsKeyword(value)) return (Token) { TokenTypeParseString(value), value, *line, startColumn };

		return (Token) { TOK_ID, value, *line, startColumn };
	}

	// Handle numeric literals
	if (isdigit(**input)) {
		char *start = *input;
		int startColumn = *column;
		bool floatingPoint = false;
		bool hexadecimal = false;
		bool floatSuffix = false;
		bool longSuffix = false;

		// Check for hexadecimal literals
		if (**input == '0' && ((*input)[1] == 'x' || (*input)[1] == 'X')) {
			// Hexadecimal part
			hexadecimal = true;
			(*input) += 2;
			(*column) += 2;
			while (isxdigit(**input)) {
				(*input)++;
				(*column)++;
			}
		}
		else {
			// Integer part
			while (isdigit(**input)) {
				(*input)++;
				(*column)++;
			}
		}

		// Check for floating point
		if (**input == '.') {
			floatingPoint = true;
			(*input)++;
			(*column)++;
			while (isdigit(**input)) {
				(*input)++;
				(*column)++;
			}
		}

		// Check for exponent part
		if (hexadecimal && (**input == 'p' || **input == 'P')) {
			floatingPoint = true;
			(*input)++;
			(*column)++;
			if (**input == '+' || **input == '-') {
				(*input)++;
				(*column)++;
			}
			if (!isdigit(**input)) {
				fprintf(stderr, "Invalid hexadecimal floating-point literal at line %d, column %d\n", *line, *column);
				exit(1);
			}
			while (isdigit(**input)) {
				(*input)++;
				(*column)++;
			}
		}
		else if (!hexadecimal && ( **input == 'e' || **input == 'E')) {
			floatingPoint = true;
			(*input)++;
			(*column)++;
			if (**input == '+' || **input == '-') {
				(*input)++;
				(*column)++;
			}
			if (!isdigit(**input)) {
				fprintf(stderr, "Invalid floating-point literal at line %d, column %d\n", *line, *column);
				exit(1);
			}
			while (isdigit(**input)) {
				(*input)++;
				(*column)++;
			}
		}

		// Check for suffix part
		if (**input == 'f' || **input == 'F') {
			floatSuffix = true;
			(*input)++;
			(*column)++;
		}
		else if (**input == 'l' || **input == 'L') {
			longSuffix = true;
			(*input)++;
			(*column)++;
		}

		size_t length = *input - start;
		char *value = strndup(start, length);
		TokenType type;
		if (floatingPoint) {
			if (floatSuffix) type = TOK_FLOATLIT;
			else if (longSuffix) type = TOK_LONGDOUBLELIT;
			else type = TOK_DOUBLELIT;
		}
		else type = TOK_INTLIT;
		
		return (Token) { type, value, *line, startColumn };
	}

	fprintf(stderr, "Unknown character: %c\n", **input);
	exit(1);
}
bool TokenPrint(Token token) {
	if (token.id == NULL) return false;
	static const char *type[] = {   "TOK_INT",
					"TOK_FLOAT",
					"TOK_CHAR",
					"TOK_VOID",
					"TOK_DOUBLE",
					"TOK_SHORT",
					"TOK_LONG",
					"TOK_SIGNED",
					"TOK_UNSIGNED",
					"TOK_INTP",
					"TOK_CHARP",
					"TOK_FLOATP",
					"TOK_DOUBLELITP",
					"TOK_LONGDOUBLELITP",
					"TOK_VOIDP",
					"TOK_DOUBLEP",
					"TOK_INTLIT",
					"TOK_CHARLIT",
					"TOK_FLOATLIT",
					"TOK_DOUBLELIT",
					"TOK_LONGDOUBLELIT",
					"TOK_STRINGLIT",
					"TOK_ID",
					"TOK_RETURN",
					"TOK_IF",
					"TOK_ELSE",
					"TOK_WHILE",
					"TOK_FOR",
					"TOK_DO",
					"TOK_SWITCH",
					"TOK_CASE",
					"TOK_DEFAULT",
					"TOK_BREAK",
					"TOK_CONTINUE",
					"TOK_TYPEDEF",
					"TOK_STRUCT",
					"TOK_UNION",
					"TOK_ENUM",
					"TOK_SIZEOF",
					"TOK_ASSIGN",
					"TOK_PLUS",
					"TOK_MINUS",
					"TOK_STAR",
					"TOK_SLASH",
					"TOK_PERCENT",
					"TOK_PLUSPLUS",
					"TOK_MINUSMINUS",
					"TOK_ARROW",
					"TOK_DOT",
					"TOK_EQUAL",
					"TOK_NOTEQUAL",
					"TOK_LESSERTHAN",
					"TOK_GREATERTHAN",
					"TOK_LESSEROREQUAL",
					"TOK_GREATEROREQUAL",
					"TOK_AND",
					"TOK_OR",
					"TOK_NOT",
					"TOK_BITAND",
					"TOK_BITOR",
					"TOK_BITNOT",
					"TOK_BITXOR",
					"TOK_LSHIFT",
					"TOK_RSHIFT",
					"TOK_OPENPARENTHESIS",
					"TOK_CLOSEPARENTHESIS",
					"TOK_OPENCURLYBRACES",
					"TOK_CLOSECURLYBRACES",
					"TOK_OPENBRACKET",
					"TOK_CLOSEBRACKET",
					"TOK_SEMICOLON",
					"TOK_COLON",
					"TOK_COMMA",
					"TOK_QUESTION",
					"TOK_EOF",
					"TOK_ERROR" };
	printf("[%s, \"%s\"] ", type[(int) token.type] ,token.id);
	return true;
}
bool TokenFree(Token *token) {
	switch (token->type) {
		case TOK_INT:
		case TOK_RETURN:
		case TOK_ID:
		case TOK_INTLIT:
			free(token->id);
			return true;
		default: return false;
	}
}
bool TokenExpect(TokenQueue *q, TokenType type) {
	return TokenQueuePeek(q).type == type;
}
bool TokenIsDataType(TokenQueue *q) {
	switch (q->first->token.type) {
		case TOK_INT:
		case TOK_CHAR:
		case TOK_FLOAT:
		case TOK_DOUBLE:
		case TOK_LONGDOUBLE:
		case TOK_VOID:
		case TOK_SHORT:
		case TOK_LONG:
		case TOK_SIGNED:
		case TOK_UNSIGNED:
			return true;
		default: return false;
	}
}
bool TokenIsKeyword(const char *str) {
	static const char *keywords[] = {"int",
					 "float",
					 "char",
					 "void",
					 "double",
					 "short",
					 "long",
					 "signed",
					 "unsigned",
					 "return",
					 "if",
					 "else",
					 "while",
					 "for",
					 "do",
					 "switch",
					 "case",
					 "default",
					 "break",
					 "continue",
					 "typedef",
					 "struct",
					 "union",
					 "enum",
					 "sizeof",
					 NULL};
	for (const char **kw = keywords; *kw; kw++) {
		if (strcmp(str, *kw) == 0) return true;
	}
	return false;
}

bool TokenQueuePush(TokenQueue *q, Token token) {
	TokenNode *node = (TokenNode *) malloc(sizeof(TokenNode));
	if (node == NULL) return false;

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

	return true;
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
bool TokenQueuePrint(TokenQueue q) {
	int i;
	TokenNode *node;
	if (q.length == 0) return false;

	for (i = 0, node = q.first; i < q.length; i++, node = node->prev) {
		TokenPrint(node->token);
	}
	node = NULL;
	return true;
}
bool TokenQueueFree(TokenQueue *q) {
	TokenNode *node;
	Token token;
	if (q->length == 0) return false;

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
	return false;
}