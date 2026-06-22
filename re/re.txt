#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>

using namespace std;

const int MAX_TOKENS = 100;
const int MAX_SYMBOLS = 100;
const int MAX_LINE_LEN = 256;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    EQUALS,
    SEMICOLON,
    UNKNOWN
};

struct Token {
    TokenType type;
    char value[50];
};

struct Variable {
    char name[50];
    char type[20];
    char value[50];
};

Variable symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

int findVariable(const char* name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void addVariable(const char* name, const char* type, const char* value, ofstream& out) {
    if (findVariable(name) != -1) {
        out << "SYNTAX ERROR: Duplicate declaration of variable '" << name << "'\n";
    } else if (symbolCount < MAX_SYMBOLS) {
        strcpy(symbolTable[symbolCount].name, name);
        strcpy(symbolTable[symbolCount].type, type);
        strcpy(symbolTable[symbolCount].value, value);
        symbolCount++;
        out << "SYMBOL TABLE: Added " << name << " [" << type << "] = " << value << "\n";
    } else {
        out << "ERROR: Symbol table is full!\n";
    }
}

int lexicalAnalyzer(const char* line, Token tokens[]) {
    int tokenCount = 0;
    int len = strlen(line);
    char word[50];
    int wordIdx = 0;

    for (int i = 0; i <= len; i++) {
        char c = line[i];

        if (isspace(c) || c == '=' || c == ';' || c == '\0') {
            if (wordIdx > 0) {
                word[wordIdx] = '\0';
                
                if (strcmp(word, "int") == 0 || strcmp(word, "float") == 0 || 
                    strcmp(word, "double") == 0 || strcmp(word, "print") == 0) {
                    tokens[tokenCount].type = KEYWORD;
                } else {
                    bool isNum = true;
                    int dots = 0;
                    for (int j = 0; j < wordIdx; j++) {
                        if (word[j] == '.') dots++;
                        else if (!isdigit(word[j])) isNum = false;
                    }
                    if (isNum && dots <= 1) {
                        tokens[tokenCount].type = NUMBER;
                    } else {
                        tokens[tokenCount].type = IDENTIFIER;
                    }
                }
                strcpy(tokens[tokenCount].value, word);
                tokenCount++;
                wordIdx = 0;
            }
            if (c == '=') {
                tokens[tokenCount].type = EQUALS;
                strcpy(tokens[tokenCount].value, "=");
                tokenCount++;
            } else if (c == ';') {
                tokens[tokenCount].type = SEMICOLON;
                strcpy(tokens[tokenCount].value, ";");
                tokenCount++;
            }
        } else {
            if (wordIdx < 49) {
                word[wordIdx++] = c;
            }
        }
    }
    return tokenCount;
}

void compileLine(const char* line, ofstream& out) {
    if (strlen(line) == 0) return;

    Token tokens[MAX_TOKENS];
    int tokenCount = lexicalAnalyzer(line, tokens);

    if (tokenCount == 0) return;

    out << "TOKEN STREAM: ";
    for (int i = 0; i < tokenCount; i++) {
        out << "[" << tokens[i].value << "] ";
    }
    out << "\n";

    if (tokenCount >= 5 && tokens[0].type == KEYWORD && tokens[1].type == IDENTIFIER &&
        tokens[2].type == EQUALS && tokens[3].type == NUMBER && tokens[4].type == SEMICOLON) {
        
        addVariable(tokens[1].value, tokens[0].value, tokens[3].value, out);
    }
    else if (tokenCount >= 4 && tokens[0].type == IDENTIFIER && tokens[1].type == EQUALS &&
             tokens[2].type == NUMBER && tokens[3].type == SEMICOLON) {
             
        int idx = findVariable(tokens[0].value);
        if (idx != -1) {
            strcpy(symbolTable[idx].value, tokens[2].value);
            out << "UPDATE: " << tokens[0].value << " changed to " << tokens[2].value << "\n";
        } else {
            out << "ERROR: Cannot assign to undeclared variable '" << tokens[0].value << "'\n";
        }
    }
    else if (tokenCount >= 3 && tokens[0].type == KEYWORD && strcmp(tokens[0].value, "print") == 0 &&
             tokens[1].type == IDENTIFIER && tokens[2].type == SEMICOLON) {
             
        int idx = findVariable(tokens[1].value);
        if (idx != -1) {
            out << "OUTPUT: " << tokens[1].value << " is " << symbolTable[idx].value << "\n";
        } else {
            out << "ERROR: Variable '" << tokens[1].value << "' is not declared.\n";
        }
    } 
    else {
        out << "SYNTAX ERROR: Invalid statement syntax\n";
    }
}

int main() {
    ifstream checkFile("input.txt");
    if (!checkFile.is_open()) {
        ofstream setup("input.txt");
        setup << "int alpha = 100;\n";
        setup << "int alpha = 500;\n";
        setup << "float beta = 20.5;\n";
        setup << "alpha = 150;\n";
        setup << "print alpha;\n";
        setup << "print beta;\n";
        setup << "print gamma;\n";
        setup.close();
    } else {
        checkFile.close();
    }

    ifstream inFile("input.txt");
    ofstream outFile("output.txt");

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error opening files!" << endl;
        return 1;
    }

    char line[MAX_LINE_LEN];
    while (inFile.getline(line, MAX_LINE_LEN)) {
        compileLine(line, outFile);
    }

    cout << "1. Our Code has been complied successfully!\n2. Please check your 'output.txt' in your project dir" << endl;

    inFile.close();
    outFile.close();

    return 0;
}
