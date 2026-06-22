#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

bool isValidIdentifier(char str[]) {
    int len = strlen(str);
    if (len == 0) return false;

    for (int i = 0; i < len; i++) {
        if (isspace(str[i])) return false;
    }

    if (!isalpha(str[0]) && str[0] != '_') return false;

    for (int i = 1; i < len; i++) {
        if (!isalnum(str[i]) && str[i] != '_') return false;
    }

    return true;
}

int main() {
    char input[100];

    cout << "--- Identifier Validator ---" << endl;
    cout << "Enter a string to check (or 'exit' to quit):" << endl;

    while (true) {
        cout << "> ";
        cin.getline(input, 100);

        if (strcmp(input, "exit") == 0) break;

        if (isValidIdentifier(input))
            cout << "Valid Identifier" << endl;
        else
            cout << "Invalid Identifier" << endl;
    }

    return 0;
}