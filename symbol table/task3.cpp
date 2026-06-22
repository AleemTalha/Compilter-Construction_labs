#include <iostream>
#include <string>
#include <regex>
#include <limits>

using namespace std;

void checkEmail(string email) {
    const regex emailPattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    
    if (regex_match(email, emailPattern)) {
        cout << "Valid Email" << endl;
    } else {
        cout << "Invalid Email" << endl;
    }
}

void checkMobile(string mobile) {
    const regex mobilePattern("(\\+92|0092|0)3[0-9]{9}");

    if (regex_match(mobile, mobilePattern)) {
        cout << "Valid Mobile Number" << endl;
    } else {
        cout << "Invalid Mobile Number" << endl;
    }
}

int main() {
    string input;
    int choice;

    while (true) {
        cout << "\n--- Task 3: Regex Validator ---" << endl;
        cout << "1. Validate Email" << endl;
        cout << "2. Validate Mobile Number" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter 1,2,3 only." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 3) break;

        if (choice == 1) {
            while (true) {
                cout << "Enter Email (or type back): ";
                cin >> input;

                if (input == "back") break;

                checkEmail(input);
            }
        }
        else if (choice == 2) {
            while (true) {
                cout << "Enter Mobile (or type back): ";
                cin >> input;

                if (input == "back") break;

                checkMobile(input);
            }
        }
        else {
            cout << "Invalid Choice!" << endl;
        }
    }

    return 0;
}