# LLVM Compiler using Flex and Bison

A C++ compiler framework that uses **Flex** for lexical analysis, **Bison** for parsing, and **LLVM** for code generation.

## Project Structure

```
LLVM_Compiler_c++/
├── build/                # Build artifacts
├── build_test/           # Test suite
├── include/              # Headers
│   ├── lexer
|      ├── lexer.l        # Flex lexer rules
│   ├── parser
|      ├── parser.y       # Bison grammar rules
│   ├── AST.h             # Abstract Syntax Tree interface
├── src/                  # Source files
|   ├── AST.cpp           # Abstract Syntax Tree implementation
│   ├── main.cpp          # Main compiler entry point
├── build.sh              # Build script
├── CMakeLists.txt        # CMake configuration file
├── commands.txt          # Command examples for running the compiler
└── README.md             # Project documentation
```

## Prerequisites

All required tools are already installed:
- ✅ **Flex** - Lexer generator
- ✅ **Bison** - Parser generator  
- ✅ **LLVM** - Code generation backend
- ✅ **CMake** - Build system
- ✅ **g++** - C++ compiler (version 17+)

## Quick Start

### Build the Compiler

```bash
./build.sh
```

This will:
1. Generate `lex.cpp` from `lex.l` using Flex
2. Generate parser tables from `grammar.y` using Bison
3. Compile the main compiler executable

### Run Examples

**From file:**
```bash
echo 'int main() { return 0; }' > test.txt
./build/compiler test.txt
```

**From stdin:**
```bash
echo 'int x = 5;' | ./build/compiler
```

## Architecture

### Lexical Analysis (Flex)
- Converts source code into tokens
- Handles keywords, identifiers, operators, literals
- Tracks line/column for error reporting

### Parsing (Bison)
- Builds Abstract Syntax Tree (AST) from tokens
- Implements standard C-like grammar rules:
  - Declarations and definitions
  - Statements and expressions
  - Control flow structures (if, while, for)

### AST Representation
```
Program
├── Block
│   ├── Declaration (Variable or Function)
│   ├── Statement (If, While, For, Return)
│   └── Expression (Binary, Unary, Literal)
```

### LLVM Backend Integration
- Converts AST to LLVM IR
- Supports optimization passes
- Generates assembly output

## Grammar Highlights

The Bison grammar supports:

```bison
program     → declarations
declaration → functionDefinition | type IDENTIFIER ASSIGN expression SEMICOLON
functionDef → type IDENTIFIER LPAREN parameters RPAREN block
ifStatement → IF LPAREN expression RPAREN block [ELSE block]
whileStmt   → WHILE LPAREN expression RPAREN block
forStmt     → FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN block
```

## Build Options (CMake)

```bash
cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_EXPENSIVE_CHECKS=ON
```

## Testing

Run the test suite:
```bash
./build_test.sh
```

Tests cover:
- Simple variable declarations
- Function definitions
- Control flow structures
- Expression parsing

## Next Steps for Enhancement

1. **Full LLVM IR Generation**: Implement complete AST-to-IR visitor pattern
2. **Type Checking**: Add type system validation
3. **Symbol Table**: Implement scope management
4. **Optimization**: Integrate LLVM passes (inlining, loop unrolling, etc.)
5. **Debug Info**: Add DWARF debugging information
6. **Standard Library**: Support STL and common libraries

## Example: Simple Addition Program

```c
int add(int a, int b) {
    return a + b;
}

void print(int x) {
    printf("%d\n", x);
}

int main() {
    int result = add(5, 3);
    print(result);
    return 0;
}
```

## License

This project is for educational purposes.

## Contributing

See `CONTRIBUTING.md` (to be created).