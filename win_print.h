#pragma once

#include <windows.h>
#include <iostream>
#include <string>

using std::string;

void ClearScreen() {
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

void SetCaretPosition(int x, int y)
{
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(console, coord);
}

void SetCaretLastLine()
{
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(console, &s);
    SetCaretPosition(0, s.srWindow.Bottom - s.srWindow.Top);
}

template<typename T>
void Print(const T& text) {
    std::cout << text;
}
template<typename T>
void PrintGreen(const T& text) {
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 10);
    std::cout << text;
    SetConsoleTextAttribute(console, 7);
}
template<typename T>
void PrintRed(const T& text) {
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 12);
    std::cout << text;
    SetConsoleTextAttribute(console, 7);
}
template<typename T>
void PrintBlue(const T& text) {
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 9);
    std::cout << text;
    SetConsoleTextAttribute(console, 7);
}
template<typename T>
void PrintGray(const T& text) {
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 8);
    std::cout << text;
    SetConsoleTextAttribute(console, 7);
}
template<typename T>
void PrintWhite(const T& text) {
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 15);
    std::cout << text;
    SetConsoleTextAttribute(console, 7);
}
template<typename T>
void PrintBlueBack(const T& text) {
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 31);
    std::cout << text;
    SetConsoleTextAttribute(console, 7);
}
template<typename T>
void PrintLightGrayBack(const T& text) {
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 112);
    std::cout << text;
    SetConsoleTextAttribute(console, 7);
}
template<typename T>
void PrintRainbow(const T& text) {
    static const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int k = 1; k < 255; ++k) {
        SetConsoleTextAttribute(console, k);
        std::cout << k << ' ' << text << std::endl;
    }
    SetConsoleTextAttribute(console, 7);
}

void PrintSpaces(int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << ' ';
    }
}
