#pragma once

#include <string>

#include <iostream>

class ThrowFatalError {
public:
    template <typename... Args>
    ThrowFatalError(Args... args);
};

template <typename... Args>
ThrowFatalError::ThrowFatalError(Args... args) {
	system("cls");
	((std::cout << args << " "), ...) << std::endl;
	std::cin.get();
	exit(1);
}