#pragma once

#include "win_print.h"
#include <cctype>
#include <unordered_map>
#include <deque>
#include <vector>

using std::vector;
using std::unordered_map;
using std::deque;

struct TInfo {
    int next = -1;
    char output = ' ';
    int shift = 0;

    TInfo() {}
    TInfo(int next, char output, int shift = 0) :
        next(next), output(output), shift(shift) {}
};

std::ostream& operator << (std::ostream& out, const TInfo& info)
{
    out << '(' << info.next << ", \'" << info.output << "\', " << info.shift << ')';
    return out;
}

using Transition = unordered_map<char, TInfo>;

const string special = "[](){}|";

class Tape {
private:
    deque<char> symbs;
    deque<char>::iterator position;

public:
    Tape() {
        symbs.push_back(' ');
        position = symbs.begin();
    }
    Tape(const string& input) {
        symbs.push_back(' ');
        for (char c : input)
            symbs.push_back(c);
        symbs.push_back(' ');
        position = symbs.begin() + 1;
    }

    void ShiftLeft() {
        if (position == symbs.begin()) {
            symbs.push_front(' ');
            position = symbs.begin();
        } else {
            --position;
        }
    }

    void ShiftRight() {
        if (++position == symbs.end()) {
            symbs.push_back(' ');
            position = symbs.end() - 1;
        }
    }

    char Get() const {
        return *position;
    }

    char Get(int rel_index) const {
        int i = 0;
        auto iter = position;
        if (rel_index < 0) {
            rel_index = -rel_index;
            for (; iter != symbs.begin() && i < rel_index; ++i)
                --iter;
            if (i != rel_index)
                return ' ';
            else
                return *iter;
        } else {
            for (; iter != symbs.end() - 1 && i < rel_index; ++i)
                ++iter;
            if (i != rel_index)
                return ' ';
            else
                return *iter;
        }
    }

    void Set(char c) {
        *position = c;
    }

    void PrintTape() const {
        HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
        PrintGray("...");
        CONSOLE_SCREEN_BUFFER_INFO s;
        GetConsoleScreenBufferInfo(c, &s);
        for (int i = -s.dwSize.X / 2 + 3; i < s.dwSize.X / 2 - 3; ++i) {
            char symb = Get(i);
            if (i == 0) {
                PrintBlueBack(symb);
            } else if (std::isalpha(symb)) {
                PrintGreen(symb);
            } else if (std::isdigit(symb)) {
                Print(symb);
            } else if (special.find(symb) != string::npos) {
                PrintBlue(symb);
            } else {
                PrintRed(symb);
            }
        }
        PrintGray("...\n");
    }
};

class Machine {
private:
    vector<Transition> transitions;
    int start_state = 0, fin_state = 0;

public:
    Machine() : transitions(1) {}
    Machine(const vector<string>& list) : transitions(1) {
        int i = 0;
        try {
            for (; i < list.size(); ++i) {
                if (list[i].empty() || (list[i].size() >= 2 &&
                    list[i][0] == '/' && list[i][1] == '/')) {
                } else if (list[i].find("start") == 0) {
                    start_state = std::stoi(list[i].substr(5));
                } else if (list[i].find("end") == 0 || list[i].find("fin") == 0) {
                    fin_state = std::stoi(list[i].substr(3));
                } else if (list[i][0] == '(') {
                    string input;
                    int j = 1;
                    while (list[i][j] != ',')
                        input.push_back(list[i][j++]);
                    int initial_state = std::stoi(input);
                    while (list[i][j++] != '\'');
                    char symb = list[i][j++];
                    while (list[i][j++] != '(');
                    input.clear();
                    while (list[i][j] != ',')
                        input.push_back(list[i][j++]);
                    int next_state = std::stoi(input);
                    while (list[i][j++] != '\'');
                    char output = list[i][j++];
                    while (list[i][j++] != ',');
                    input.clear();
                    while (list[i][j] != ')')
                        input.push_back(list[i][j++]);
                    int shift = std::stoi(input);
                    while (transitions.size() <= max(initial_state, next_state))
                        transitions.emplace_back();
                    transitions[initial_state][symb] = TInfo(next_state, output, shift);
                } else {
                    throw std::exception();
                }
            }
        } catch (...) {
            std::cout << "Unfortunately, an exception occured while compiling machine.\n";
            std::cout << "Compilation terminated at line " << i << ":\n";
            PrintGray("    ...\n");
            for (int j = max(0, i - 2); j < min(list.size(), i + 3); ++j) {
                if (j == i)
                    PrintRed(std::to_string(j) + "    " + list[j] + "\n");
                else
                    Print(std::to_string(j) + "    " + list[j] + "\n");
            }
            PrintGray("    ...\n");
            std::cout << "Try correcting machine code.\n";
        }
    }

    void Process(const string& input, void(*func)(), bool fast, int limit) const {
        Tape tape(input);
        int current_state = start_state, step_count = 0;
        auto animate = [&tape, this, func, fast, &current_state]() {
            if (!fast) {
                PrintMachine(current_state, tape);
                (*func)();
            }
        };
        animate();
        while (current_state != fin_state && step_count < limit) {
            auto transition = transitions[current_state].find(tape.Get());
            if (transition == transitions[current_state].end()) {
                PrintMachine(current_state, tape);
                PrintRed("Missing Transition!\n");
                return;
            }
            const auto& info = transition->second;
            if (info.output != tape.Get()) {
                tape.Set(info.output);
                animate();
            }
            if (info.shift == -1)
                tape.ShiftLeft();
            else if (info.shift == 1)
                tape.ShiftRight();
            current_state = info.next;
            ++step_count;
            animate();
        }
        if (fast) {
            PrintMachine(current_state, tape);
        }
        std::cout << step_count << " iters\n";
    }

    void PrintMachine(int current_state, const Tape& tape) const {
        ClearScreen();
        Print("\n");
        tape.PrintTape();
        HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO s;
        GetConsoleScreenBufferInfo(c, &s);
        PrintSpaces(s.dwSize.X / 2);
        PrintWhite("^\n");
        PrintSpaces(s.dwSize.X / 2);
        if (current_state == fin_state) {
            PrintRed(current_state);
        } else if (current_state == start_state) {
            PrintGreen(current_state);
        } else {
            Print(current_state);
        }
        Print('\n');
        int i = 0;
        for (const auto&[symb, info] : transitions[current_state]) {
            if (i == 0) {
                PrintSpaces(s.dwSize.X / 3);
                std::cout << '\'' << symb << '\'';
                PrintBlue(" -> ");
                std::cout << info;
            } else {
                PrintSpaces(4);
                std::cout << '\'' << symb << '\'';
                PrintBlue(" -> ");
                std::cout << info << '\n';
            }
            i ^= 1;
        }
        Print('\n');
    }
};