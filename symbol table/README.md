# Mini C++ Compiler

A simple C++ compiler implementation that demonstrates core compiler concepts using double buffering, lexical analysis, and symbol table management.

## 📋 Overview

This project implements a mini compiler that reads source code, performs lexical analysis using regular expressions, manages a symbol table, and validates syntax using defined grammar rules. It uses multithreading with double buffering for efficient I/O and processing.

## ✨ Features

### 1. **Double Buffering**
- Two buffers (`bufferA` and `bufferB`) work alternately
- Loader thread fills one buffer while processor thread processes the other
- Improves efficiency by overlapping I/O and processing operations

### 2. **Multithreading**
- **Loader Thread**: Reads source code from `input.txt` and fills buffers
- **Processor Thread**: Processes buffered data and generates output
- Thread-safe operations using `mutex` for synchronization

### 3. **Lexical Analysis**
- Tokenizes input source code character-by-character
- Uses regular expressions to identify token types:
  - **Keywords**: `int`, `float`, `double`, `print`
  - **Identifiers**: Variable names (e.g., `alpha`, `beta`)
  - **Numbers**: Integers and decimals (e.g., `100`, `20.5`)
  - **Operators**: `=` (assignment)
  - **Punctuation**: `;` (semicolon)

### 4. **Token Stream Generation**
- Converts source code into a sequence of tokens
- Example: `int alpha = 100;` → `[int] [alpha] [=] [100] [;]`
- Displayed in output for verification

### 5. **Symbol Table Management**
- Stores variable declarations with type and value
- **Operations**:
  - ✅ Add new variables with type and initial value
  - ✅ Update variable values during execution
  - ✅ Detect and reject duplicate declarations
  - ✅ Validate variable usage before access

### 6. **Grammar Rules**
Three supported statement patterns:

#### Declaration
```
<type> <identifier> = <number>;
int alpha = 100;
float beta = 20.5;
```

#### Assignment
```
<identifier> = <number>;
alpha = 150;
```

#### Print Statement
```
print <identifier>;
print alpha;
```

## 🚀 Compilation & Execution

### Compile
```bash
g++ -std=c++11 t1.cpp -o t1
```

### Run
```bash
./t1
```

### Output
Results are written to `output.txt`

## 📤 Output Example

```
TOKEN STREAM: [int] [alpha] [=] [100] [;] 
SYMBOL TABLE: Added alpha [int] = 100

TOKEN STREAM: [int] [alpha] [=] [500] [;] 
SYNTAX ERROR: Duplicate declaration of variable 'alpha'

TOKEN STREAM: [float] [beta] [=] [20.5] [;] 
SYMBOL TABLE: Added beta [float] = 20.5

TOKEN STREAM: [alpha] [=] [150] [;] 
UPDATE: alpha changed to 150

TOKEN STREAM: [print] [alpha] [;] 
OUTPUT: alpha is 150

TOKEN STREAM: [print] [beta] [;] 
OUTPUT: beta is 20.5

TOKEN STREAM: [print] [gamma] [;] 
ERROR: Variable 'gamma' is not declared.
```

## 📁 File Structure

```
lab9/
├── t1.cpp          # Main compiler implementation
├── task.md         # Task requirements
├── README.md       # This file
├── input.txt       # Generated input source code
├── output.txt      # Generated compiler output
└── t1              # Compiled executable
```

## 🔧 Architecture

### Token Structure
```cpp
struct Token {
    TokenType type;   // KEYWORD, IDENTIFIER, NUMBER, EQUALS, SEMICOLON
    string value;     // Token value (e.g., "int", "alpha", "100")
};
```

### Variable Structure
```cpp
struct Variable {
    string type;      // Variable type (int, float, double)
    string value;     // Current value
};
```

### Main Components

| Component | Function | Purpose |
|-----------|----------|---------|
| `lexicalAnalyzer()` | Tokenizes input string | Converts source code to token stream |
| `compileLine()` | Processes one line | Validates grammar and updates symbol table |
| `loader()` | Thread function | Reads input.txt and fills buffers |
| `processor()` | Thread function | Processes buffers and generates output |

## 🎯 How It Works

1. **Input Generation**: Creates `input.txt` with source code statements
2. **Loading Phase**: `loader()` thread reads lines and fills `bufferA`/`bufferB` alternately
3. **Processing Phase**: `processor()` thread switches to process the filled buffer
4. **Lexical Analysis**: Each line is tokenized into individual tokens
5. **Syntax Validation**: Tokens are matched against grammar rules
6. **Symbol Table Update**: Valid statements update the symbol table
7. **Output Generation**: All steps logged to `output.txt`

## ✅ Requirements Satisfaction

| Requirement | Status |
|-------------|--------|
| Double buffering | ✅ Fully implemented |
| Multithreading | ✅ Both threads working |
| Regular expressions | ✅ For token identification |
| Token stream generation | ✅ Visible in output |
| Symbol table | ✅ Storage, update, duplicate detection |
| Grammar validation | ✅ Declaration, assignment, print |
| Error handling | ✅ Syntax errors and undeclared variables |

## 🐛 Error Handling

The compiler detects and reports:
- **Duplicate declarations**: Attempting to declare the same variable twice
- **Undeclared variables**: Using a variable before declaration
- **Syntax errors**: Statements that don't match defined grammar rules

## 📝 Example Usage

### Input Code (in `input.txt`)
```cpp
int x = 42;
float y = 3.14;
x = 100;
print x;
print y;
print z;  // Error: z not declared
```

### Expected Behavior
1. ✅ `x` declared as `int` with value `42`
2. ✅ `y` declared as `float` with value `3.14`
3. ✅ `x` updated to `100`
4. ✅ Print `x` outputs: `OUTPUT: x is 100`
5. ✅ Print `y` outputs: `OUTPUT: y is 3.14`
6. ❌ Print `z` outputs: `ERROR: Variable 'z' is not declared.`

## 🔒 Thread Safety

- **Mutex Lock**: Protects buffer access during simultaneous read/write
- **Active Buffer Flag**: Switches between buffers atomically
- **Synchronization**: `join()` ensures all threads complete before exit

## 💡 Key Concepts Demonstrated

- ✅ **Concurrent Programming**: Multithreading with synchronization
- ✅ **Lexical Analysis**: Token recognition using regular expressions
- ✅ **Parser**: Syntax validation using grammar rules
- ✅ **Symbol Table**: Variable scope and lifecycle management
- ✅ **I/O Optimization**: Double buffering for performance
- ✅ **Error Handling**: Validation and error reporting

## 📚 Data Structures Used

- `map<string, Variable>` - Symbol table for O(log n) lookup
- `vector<Token>` - Token stream representation
- `string` - Variable storage and manipulation
- `regex` - Pattern matching for tokenization

---

**Status**: ✅ All requirements satisfied and working correctly!
