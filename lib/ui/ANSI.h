#pragma once
#include <iostream>
#include <string>

namespace ansi {
    inline void moveCursor(int x, int y) {
        std::cout << "\033[" << y << ";" << x << "H";
    }

    inline void clearScreen() {
        std::cout << "\033[2J";
    }

    inline void clearLine() {
        std::cout << "\033[K";
    }

    inline void hideCursor() {
        std::cout << "\033[?25l";
    }

    inline void showCursor() {
        std::cout << "\033[?25h";
    }

    inline void setColor(int code) {
        std::cout << "\033[" << code << "m";
    }

    inline void resetColor() {
        std::cout << "\033[0m";
    }

    enum Color {
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37
    };
}
