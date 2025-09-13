#include "parser.h"

int main() {
	TokenQueue q1 = { NULL, NULL, 0 };
	TokenQueue q2 = { NULL, NULL, 0 };
	int line = 1;
	int column = 1;
	char *czy = "int main() { return 0; }";
	Token token;
	while (1) {
		token = GetNextToken(&czy, &line, &column);
		if (token.type == TOK_EOF) break;
		TokenQueuePush(&q1, token);
	}
	TokenQueuePrint(q1);
	TokenQueueFree(&q1);
	return 0;
}