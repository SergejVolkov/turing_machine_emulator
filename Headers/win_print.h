// various win console tools, coloring, spacing, etc.

#pragma once

#include <windows.h>
#include <iostream>
#include <string>

using std::string;

namespace IO {
    inline void ClearScreen() { // fill screen with spaces
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        char fill = ' ';
        COORD tl = { 0, 0 };
        CONSOLE_SCREEN_BUFFER_INFO s;
        GetConsoleScreenBufferInfo(console, &s);
        DWORD written, cells = s.dwSize.X * s.dwSize.Y;
        FillConsoleOutputCharacter(console, fill, cells, tl, &written);
        FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
        SetConsoleCursorPosition(console, tl);
    }

    inline void SetCaretPosition(int x, int y) { // set cursor to coords
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        std::cout.flush();
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(console, coord);
    }

    inline void SetCaretLastLine() { // set cursor to the beginning of the last line
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO s;
        GetConsoleScreenBufferInfo(console, &s);
        SetCaretPosition(0, s.srWindow.Bottom - s.srWindow.Top);
    }

    template<typename T>
    inline void Print(const T& text) { // just for the sake of compatibility
        std::cout << text;
    }
    template<typename T>
    inline void PrintGreen(const T& text) { // set color to green, print text, set color back to default
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console, 10);
        std::cout << text;
        SetConsoleTextAttribute(console, 7);
    }
    template<typename T>
    inline void PrintRed(const T& text) { // same with red
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console, 12);
        std::cout << text;
        SetConsoleTextAttribute(console, 7);
    }
    template<typename T>
    inline void PrintBlue(const T& text) { // same with blue
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console, 9);
        std::cout << text;
        SetConsoleTextAttribute(console, 7);
    }
    template<typename T>
    inline void PrintGray(const T& text) { // and stuff
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console, 8);
        std::cout << text;
        SetConsoleTextAttribute(console, 7);
    }
    template<typename T>
    inline void PrintWhite(const T& text) {
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console, 15);
        std::cout << text;
        SetConsoleTextAttribute(console, 7);
    }
    template<typename T>
    inline void PrintBlueBack(const T& text) { // set background to blue, print, revert back to default
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console, 31);
        std::cout << text;
        SetConsoleTextAttribute(console, 7);
    }
    template<typename T>
    inline void PrintLightGrayBack(const T& text) { // same with other color
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console, 112);
        std::cout << text;
        SetConsoleTextAttribute(console, 7);
    }
    template<typename T>
    inline void PrintRainbow(const T& text) { // display all 254 color combinations
        static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        for (int k = 1; k < 255; ++k) {
            SetConsoleTextAttribute(console, k);
            std::cout << k << ' ' << text << std::endl;
        }
        SetConsoleTextAttribute(console, 7);
    }

    inline void PrintSpaces(int n) { // indentation
        for (int i = 0; i < n; ++i) {
            std::cout << ' ';
        }
    }
}
