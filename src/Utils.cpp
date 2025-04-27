#include "../lib/Utils.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>

using namespace std;

string Utils::getCInput() {
    string input;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the newline character from the input buffer
    getline(cin, input);
    return input;
}

string Utils::getCInput(string prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

string Utils::getCInput(string prompt, bool hideInput = false) {
    cout << prompt;
    string password;
    char ch;
    while ((ch = _getch()) != '\r') { // '\r' = Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b"; // Erase character visually
            }
        }
        else {
            password += ch;
            cout << '*'; // Show asterisk instead of real character
        }
    }
    cout << endl;
    return password;
}

int Utils::getIntCInput() {
    string input;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the newline character from the input buffer
    getline(cin, input);
    return stoi(input);
}

int Utils::getIntCInput(string prompt) {
    cout << prompt;
    string input;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear the newline character from the input buffer
    getline(cin, input);
    return stoi(input);
}

std::vector<std::string> Utils::splitString(const std::string& input, char delimiter) {
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> result;

    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}
