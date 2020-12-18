// Turing machine class

#include "turing.h"

// special symbols .txt files language
const string special = "[](){}|";

bool cancel_task;

Tape::Tape() {
    Tape::symbs.push_back(' ');
    Tape::position = Tape::symbs.begin();
}

Tape::Tape(const string& input) {
    Tape::symbs.push_back(' ');
    for (char c : input)
        Tape::symbs.push_back(c);
    Tape::symbs.push_back(' ');
    Tape::position = Tape::symbs.begin() + 1;
}

void Tape::ShiftLeft() {
    if (Tape::position == Tape::symbs.begin()) {
        Tape::symbs.push_front(' ');
        Tape::position = Tape::symbs.begin();
    } else {
        --Tape::position;
    }
}

void Tape::ShiftRight() {
    if (++Tape::position == Tape::symbs.end()) {
        Tape::symbs.push_back(' ');
        Tape::position = Tape::symbs.end() - 1;
    }
}

char Tape::Get() const {
    return *Tape::position;
}

char Tape::Get(int rel_index) const {
    int i = 0;
    auto iter = Tape::position;
    if (rel_index < 0) { // to the left of head pos
        rel_index = -rel_index;
        for (; iter != Tape::symbs.begin() && i < rel_index; ++i)
            --iter;
        if (i != rel_index)
            return ' ';
        else
            return *iter;
    } else { // to the right
        for (; iter != Tape::symbs.end() - 1 && i < rel_index; ++i)
            ++iter;
        if (i != rel_index)
            return ' ';
        else
            return *iter;
    }
}

void Tape::Set(char c) {
    *position = c;
}

void Tape::PrintTape() const {
    static const HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
    IO::PrintGray("...");
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(c, &s);
    for (int i = -s.dwSize.X / 2 + 3; i < s.dwSize.X / 2 - 3; ++i) {
        char symb = Get(i);
        if (i == 0) {
            IO::PrintBlueBack(symb);
        } else if (std::isalpha(symb)) {
            IO::PrintGreen(symb);
        } else if (std::isdigit(symb)) {
            IO::Print(symb);
        } else if (special.find(symb) != string::npos) {
            IO::PrintBlue(symb);
        } else {
            IO::PrintRed(symb);
        }
    }
    IO::PrintGray("...\n");
}

int Machine::get_state_index(const string& input) {
    auto iter = state_index.find(input);
    if (iter == state_index.end()) {
        state_index[input] = Machine::transitions.size();
        Machine::transitions.emplace_back();
        state_names.push_back(input);
    }
    return state_index[input];
}

void Machine::PrintInfo(const TInfo& info) {
    std::cout << '(' << state_names[info.next] << ", \'" << info.output << "\', " << info.shift << ')';
}

Machine::Machine() {}

Machine::Machine(const vector<string>& list) {
    int i = 0;
    try {
        for (; i < list.size(); ++i) {
            if (list[i].empty() || (list[i].size() >= 2 &&
                list[i][0] == '/' && list[i][1] == '/')) {
            } else if (list[i].find("start") == 0) {
                start_state = get_state_index(list[i].substr(list[i].find_last_of(' ') + 1));
            } else if (list[i].find("end") == 0 || list[i].find("fin") == 0) {
                fin_state = get_state_index(list[i].substr(list[i].find_last_of(' ') + 1));
            } else if (list[i][0] == '(') {
                string input;
                int j = 1;
                while (list[i][j] != ',')
                    input.push_back(list[i][j++]);
                int initial_state = get_state_index(input);
                while (list[i][j++] != '\'');
                char symb = list[i][j++];
                while (list[i][j++] != '(');
                input.clear();
                while (list[i][j] != ',')
                    input.push_back(list[i][j++]);
                int next_state = get_state_index(input);
                while (list[i][j++] != '\'');
                char output = list[i][j++];
                while (list[i][j++] != ',');
                input.clear();
                while (list[i][j] != ')')
                    input.push_back(list[i][j++]);
                int shift = std::stoi(input);
                transitions[initial_state][symb] = TInfo(next_state, output, shift);
            } else {
                throw std::exception();
            }
        }
    } catch (...) {
        std::cout << "Unfortunately, an exception occured while compiling machine.\n";
        std::cout << "Compilation terminated at line " << i << ":\n";
        IO::PrintGray("    ...\n");
        for (int j = max(0, i - 2); j < min(list.size(), i + 3); ++j) {
            if (j == i)
                IO::PrintRed(std::to_string(j) + "    " + list[j] + "\n");
            else
                IO::Print(std::to_string(j) + "    " + list[j] + "\n");
        }
        IO::PrintGray("    ...\n");
        std::cout << "Try correcting machine code.\n";
    }
}

void Machine::Process(const string& input, int(*func)(), bool fast, int limit) {
    // load a new tape
    Tape tape(input);
    int current_state = start_state, step_count = 0;
    auto animate = [&tape, this, func, &fast, &current_state, &limit]() {
        // provide uniform access to visual feedback
        if (!fast) {
            Machine::PrintMachine(current_state, tape);
            if (current_state != fin_state) {
                int ans = (*func)();
                if (ans == 1)
                    limit = 0;
                else if (ans == 2)
                    fast = true;
            }
        }
    };
    animate();
    while (current_state != fin_state && step_count < limit && !cancel_task) {
        // main working cycle
        auto transition = transitions[current_state].find(tape.Get());
        if (transition == transitions[current_state].end()) {
            Machine::PrintMachine(current_state, tape);
            IO::PrintRed("Missing Transition!\n");
            return;
        }
        const auto& info = transition->second;
        if (info.output != tape.Get()) {
            tape.Set(info.output);
            animate();
            if (step_count >= limit || cancel_task) {
                break;
            }
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
        IO::ClearScreen();
        Machine::PrintMachine(current_state, tape, false);
    } else {
        Machine::PrintMachine(current_state, tape, false);
        static const HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO s;
        GetConsoleScreenBufferInfo(c, &s);
        auto saved_coord = s.dwCursorPosition;
        IO::SetCaretLastLine();
        IO::PrintSpaces(s.dwSize.X - 1);
        SetConsoleCursorPosition(c, saved_coord);
    }
    std::cout << step_count << " iters\n";
}

void Machine::PrintMachine(int current_state, const Tape& tape, bool reset_cursor) {
    static const HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(c, &s);
    auto saved_coord = s.dwCursorPosition;
    SetConsoleCursorPosition(c, { 0, 0 });
    IO::Print("\n");
    tape.PrintTape();
    SetConsoleCursorPosition(c, { s.dwSize.X / 2, 2 });
    IO::PrintWhite("^");
    SetConsoleCursorPosition(c, { s.dwSize.X / 2, 3 });
    IO::PrintSpaces(s.dwSize.X / 2 + 1);
    SetConsoleCursorPosition(c, { s.dwSize.X / 2, 3 });
    if (current_state == fin_state) {
        IO::PrintRed(state_names[current_state]);
    } else if (current_state == start_state) {
        IO::PrintGreen(state_names[current_state]);
    } else {
        IO::Print(state_names[current_state]);
    }
    IO::Print('\n');
    int i = 0, line_ind = 4;
    for (const auto&[symb, info] : transitions[current_state]) {
        if (line_ind >= s.srWindow.Bottom - s.srWindow.Top - 2) {
            SetConsoleCursorPosition(c, { s.dwSize.X / 3 + 1, (SHORT)line_ind });
            std::cout << "...";
            break;
        }
        if (i == 0) {
            SetConsoleCursorPosition(c, { s.dwSize.X / 3 + 1, (SHORT)line_ind });
            std::cout << '\'' << symb << '\'';
            IO::PrintBlue(" -> ");
            PrintInfo(info);
        } else {
            IO::PrintSpaces(4);
            std::cout << '\'' << symb << '\'';
            IO::PrintBlue(" -> ");
            PrintInfo(info);
            GetConsoleScreenBufferInfo(c, &s);
            IO::PrintSpaces(s.dwSize.X - s.dwCursorPosition.X + 1);
            ++line_ind;
        }
        i ^= 1;
    }
    GetConsoleScreenBufferInfo(c, &s);
    if (i == 1) {
        IO::PrintSpaces(s.dwSize.X - s.dwCursorPosition.X + 1);
        GetConsoleScreenBufferInfo(c, &s);
    }
    int new_print_pos = (int)s.dwCursorPosition.Y;
    if (new_print_pos < last_print_position) {
        DWORD written, cells = s.dwSize.X * (last_print_position - new_print_pos + 1);
        FillConsoleOutputCharacter(c, ' ', cells, s.dwCursorPosition, &written);
        FillConsoleOutputAttribute(c, s.wAttributes, cells, s.dwCursorPosition, &written);
    }
    last_print_position = new_print_pos;
    if (reset_cursor) {
        SetConsoleCursorPosition(c, saved_coord);
    } else {
        SetConsoleCursorPosition(c, { 0, (SHORT)last_print_position });
    }
}
