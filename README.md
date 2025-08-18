# czy

C to C transpiler with the goal of building a full compiler for the Czy language.
Right now this includes a lexer and a parser. The lexer is responsible for tokenizing the input source code, while the parser processes these tokens to create an abstract syntax tree (AST).

## Project Structure

```
czy
├── src
│   ├── main.c        # Entry point of the application
│   ├── lexer.c       # Implements lexer functionality
│   ├── lexer.h       # Header file for lexer
│   ├── parser.c      # Implements parser functionality
│   ├── parser.h      # Header file for parser
├── Makefile          # Build instructions for compiling the project
└── README.md         # Documentation for the project
```

## Compilation

To compile the project, run the following command in the terminal:

```
make
```

This will generate an executable named `main`.

## Cleaning Up

To remove the compiled object files and the executable, use the command:

```
make clean
```

## Usage

After compiling, you can run the application using:

```
./main
```

Make sure to provide the necessary input as required by the application.