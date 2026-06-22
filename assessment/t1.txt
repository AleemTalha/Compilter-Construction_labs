#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <thread>
#include <mutex>
#include <sstream>
#include <chrono>

using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    SEMICOLON,
    EQUALS,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;
};

struct Variable {
    string type;
    string value;
};
map<string, Variable> symbolTable;
vector<Token> tokenStream;


string bufferA, bufferB;
int activeBuffer = 1;
bool loadingDone = false;
mutex mtx;

vector<Token> lexicalAnalyzer(string line) {
    vector<Token> tokens;
    if (line.empty()) return tokens;

    regex keyword_regex("^(int|float|double|print)$");
    regex identifier_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    regex number_regex("^[0-9]+(\\.[0-9]+)?$");

    string word = "";
    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];
        
        if (c == ' ' || c == '=' || c == ';') {
            if (!word.empty()) {
                tokens.push_back({UNKNOWN, word});
                word = "";
            }
            if (c == '=') tokens.push_back({EQUALS, "="});
            else if (c == ';') tokens.push_back({SEMICOLON, ";"});
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        tokens.push_back({UNKNOWN, word});
    }

    // Classify tokens
    for (auto& token : tokens) {
        if (regex_match(token.value, keyword_regex)) {
            token.type = KEYWORD;
        } else if (regex_match(token.value, number_regex)) {
            token.type = NUMBER;
        } else if (regex_match(token.value, identifier_regex)) {
            token.type = IDENTIFIER;
        }
    }
    return tokens;
}

void compileLine(string line, ofstream& out) {
    if (line.empty()) return;

    vector<Token> tokens = lexicalAnalyzer(line);
    
    if (tokens.empty()) return;

    out << "TOKEN STREAM: ";
    for (const auto& t : tokens) {
        out << "[" << t.value << "] ";
    }
    out << endl;

    if (tokens.size() >= 5 && tokens[0].type == KEYWORD && tokens[1].type == IDENTIFIER && 
        tokens[2].type == EQUALS && tokens[3].type == NUMBER && tokens[4].type == SEMICOLON) {
        
        string type = tokens[0].value;
        string name = tokens[1].value;
        string val = tokens[3].value;

        if (symbolTable.count(name)) {
            out << "SYNTAX ERROR: Duplicate declaration of variable '" << name << "'" << endl;
        } else {
            symbolTable[name] = {type, val};
            out << "SYMBOL TABLE: Added " << name << " [" << type << "] = " << val << endl;
        }
    }
    else if (tokens.size() >= 3 && tokens[0].type == KEYWORD && tokens[0].value == "print" && 
             tokens[1].type == IDENTIFIER && tokens[2].type == SEMICOLON) {
        
        string name = tokens[1].value;
        if (symbolTable.count(name)) {
            out << "OUTPUT: " << name << " is " << symbolTable[name].value << endl;
        } else {
            out << "ERROR: Variable '" << name << "' is not declared." << endl;
        }
    }
    else if (tokens.size() >= 4 && tokens[0].type == IDENTIFIER && tokens[1].type == EQUALS && 
             tokens[2].type == NUMBER && tokens[3].type == SEMICOLON) {
        
        string name = tokens[0].value;
        string val = tokens[2].value;
        if (symbolTable.count(name)) {
            symbolTable[name].value = val;
            out << "UPDATE: " << name << " changed to " << val << endl;
        } else {
            out << "ERROR: Cannot assign to undeclared variable '" << name << "'" << endl;
        }
    }
    else {
        out << "SYNTAX ERROR: Invalid statement syntax" << endl;
    }
}


void loader() {
    ifstream inFile("input.txt");
    string line;
    while (getline(inFile, line)) {
        mtx.lock();
        if (activeBuffer == 1) bufferA += line + "\n";
        else bufferB += line + "\n";
        mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    loadingDone = true;
    inFile.close();
}

void processor() {
    ofstream outFile("output.txt");
    if (!outFile.is_open()) return;

    while (!loadingDone || !bufferA.empty() || !bufferB.empty()) {
        string dataToProcess = "";

        mtx.lock();
        if (activeBuffer == 1 && !bufferA.empty()) {
            dataToProcess = bufferA;
            bufferA.clear();
            activeBuffer = 2; 
        } else if (activeBuffer == 2 && !bufferB.empty()) {
            dataToProcess = bufferB;
            bufferB.clear();
            activeBuffer = 1;
        }
        mtx.unlock();

        if (!dataToProcess.empty()) {
            stringstream ss(dataToProcess);
            string line;
            while (getline(ss, line)) {
                compileLine(line, outFile);
            }
            outFile.flush();
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    outFile.close();
}


int main() {
    // ofstream setup("input.txt");
    // setup << "int alpha = 100;" << endl;
    // setup << "int alpha = 500;" << endl;
    // setup << "float beta = 20.5;" << endl;
    // setup << "alpha = 150;" << endl;
    // setup << "print alpha;" << endl;
    // setup << "print beta;" << endl;
    // setup << "print gamma;" << endl;
    // setup.close();

    thread t1(loader);
    thread t2(processor);

    t1.join();
    t2.join();

    cout << "1. Our Code has been complied successfully!\n2. Please check your 'output.txt' in your project dir " << endl;

    return 0;
}