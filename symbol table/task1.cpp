#include <iostream>
#include <cstring>

using namespace std;

struct Symbol {
    char name[50];
    char type[20];
    int location;
    char scope[50];
};

Symbol table[100];
int symbolCount = 0;

bool exists(char n[]) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(table[i].name, n) == 0) return true;
    }
    return false;
}

bool isDataType(char t[]) {
    if (strcmp(t, "int") == 0 || strcmp(t, "float") == 0 || 
        strcmp(t, "char") == 0 || strcmp(t, "double") == 0) {
        return true;
    }
    return false;
}

void add(char n[], char t[], const char* s, int &mem) {
    if (!exists(n) && isDataType(t)) {
        strcpy(table[symbolCount].name, n);
        strcpy(table[symbolCount].type, t);
        strcpy(table[symbolCount].scope, s);
        table[symbolCount].location = mem;
        mem += 4;
        symbolCount++;
    }
}

void show() {
    cout << "\n--- SYMBOL TABLE ---" << endl;
    cout << "Name\tType\tLocation\tScope" << endl;
    for (int i = 0; i < symbolCount; i++) {
        cout << table[i].name << "\t" 
             << table[i].type << "\t" 
             << table[i].location << "\t\t" 
             << table[i].scope << endl;
    }
}

int main() {
    int mem = 1000;
    char line[100];
    cout << "Enter your code (type 'END' to finish):" << endl;
    
    while (true) {
        cin.getline(line, 100);
        if (strcmp(line, "END") == 0) break;
        if (line[0] == '/' && line[1] == '/') continue;

        char type[20], name[50];
        if (sscanf(line, "%s %s", type, name) == 2) {
            int len = strlen(name);
            if (name[len - 1] == ';') name[len - 1] = '\0';
            if (isDataType(type)) {
                add(name, type, "main", mem);
            }
        }
    }

    show();
    return 0;
}