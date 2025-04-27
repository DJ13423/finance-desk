#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace Utils
{
    template <typename... Args>
    void printChoices(Args... args) {
        int index = 1;
        ((std::cout << index++ << ". " << args << std::endl), ...); // what C++ wizardry is this?
    }
    string getCInput();
    string getCInput(string prompt);
    string getCInput(string prompt, bool hideInput);
    int getIntCInput();
    int getIntCInput(string prompt);
    std::vector<std::string> splitString(const std::string& input, char delimiter);

    template <typename... Args>
    void ThrowFatalError(Args... args) {
        system("cls");
        ((std::cout << args << " "), ...) << std::endl;
        std::cin.get();
        exit(1);
    }
};

