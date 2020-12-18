// Turing machine class

#pragma once

#include <cctype>
#include <unordered_map>
#include <deque>
#include <vector>

#include "win_print.h"

using std::vector;
using std::unordered_map;
using std::deque;

struct TInfo { // represents single transition
    // next state id
    int next = -1;

    // output symbol
    char output = ' ';

    // head shift
    int shift = 0;

    TInfo() {}
    TInfo(int next, char output, int shift = 0) :
        next(next), output(output), shift(shift) {}
};

// list of transitions from one state
using Transition = unordered_map<char, TInfo>;

// helps to interrupt task by user input
extern bool cancel_task;

// implementation of infinite tape of symbols
class Tape {
private:
    // tape
    deque<char> symbs;

    // head position
    deque<char>::iterator position;

public:
    // create an empty tape
    Tape();
    // create tape with some input
    Tape(const string& input);

    // shift head one position left
    void ShiftLeft();
    // shift head one step to the right
    void ShiftRight();
    // read symbol from cell
    char Get() const;
    // read symbol from cell placed relatively to the current position
    char Get(int rel_index) const;
    // write symbol down into cell
    void Set(char c);
    // send tape content to the standard output
    void PrintTape() const;
};

// implementation of Turing machine
class Machine {
private:
    vector<Transition> transitions;
    unordered_map<string, int> state_index;
    vector<string> state_names;
    int start_state = 0, fin_state = 0, last_print_position = 0;

    int get_state_index(const string& input);
    void PrintInfo(const TInfo& info);

public:
    Machine();
    Machine(const vector<string>& list);

    // start machine
    void Process(const string& input, int(*func)(), bool fast, int limit);
    // terminal formatting and decoration, nothing interesting here
    void PrintMachine(int current_state, const Tape& tape, bool reset_cursor = true);
};