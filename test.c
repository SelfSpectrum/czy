#include <stdio.h>
#include <stdlib.h>
// A simple assert macro
#define MY_ASSERT(expr) \
    if (!(expr)) { \
        fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", #expr, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

#define QUEUE_NODE(T) \
    typedef struct T ## _Node { \
        T data; \
        T_Node prev; \
    } T ## _Node;
#define QUEUE(T) \
    typedef struct T ## _Queue { \
        T ## _Node *first; \
        T ## _Node *last; \
        int length; \
    } T ## _Queue;

void main() {
    int x = -1;
    MY_ASSERT(x > 0);
}