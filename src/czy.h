#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    FILE* outputStream;   // Where to send errors (default: stderr)
    bool hadError;        // Global error flag
    bool panicMode;       // For error recovery/synchronization
} CompilerState;

// Initialize the compiler state (call this at startup)
void InitCompiler(CompilerState* state, FILE* errorSteam);

// Main error reporting function
void CompilerError(CompilerState* state, int line, int column, const char* where, const char* message, const char* lineText);

// Macro for convenience
#define ERROR_AT(state, line, col, expr, msg) CompilerError(state, line, col, "Error", msg, expr)
#define WARNING_AT(state, line, col, expr, msg) CompilerError(state, line, col, "Warning", msg, expr)

#endif
