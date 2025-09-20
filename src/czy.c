#include "czy.h"

void InitCompiler(CompilerState* state, FILE* errorSteam) {
    state->outputStream = errorSteam;
    state->hadError = false;
    state->panicMode = false;
}

void CompilerError(CompilerState* state, int line, int column, const char* where, const char* message, const char* lineText) {
    // Set the global error flag
    state->hadError = true;

    // Don't report cascading errors while in panic mode
    if (state->panicMode) return;

    fprintf(state->outputStream, "\033[1;31m%s\033[0m at line %d, column %d:\n", where, line, column);
    fprintf(state->outputStream, "  %s\n", message);

    if (lineText) {
        // Print the problematic line
        fprintf(state->outputStream, "  \033[1;37m%.*s\033[0m\n", (int)strlen(lineText), lineText);

        // Print the caret pointing to the exact column
        fprintf(state->outputStream, "  \033[1;32m"); // Green color
        for (int i = 0; i < column; i++) {
            if (i < (int)strlen(lineText) && lineText[i] == '\t') {
                fprintf(state->outputStream, "    "); // Tabs are 4 spaces
            } else {
                fprintf(state->outputStream, " ");
            }
        }
        fprintf(state->outputStream, "^\033[0m\n");
    }

    fprintf(state->outputStream, "\n");

    // Optional: Enter panic mode for error recovery
    state->panicMode = true;
}

// Call this when you've recovered from an error (e.g., after synchronizing)
void ExitPanicMode(CompilerState* state) {
    state->panicMode = false;
}
