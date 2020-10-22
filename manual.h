#pragma once

const vector<string> man_list_commands = {
    "man: an interface to command manual pages",
    "list: show all existing machines",
    "define: create or edit machine",
    "rename: rename machine",
    "run: compile and execute machine",
    "pass: do nothing",
    "exit: stop application"
};

const unordered_map<string, string> man_commands = {
    {"define", "DEFINE                                              IO Commands\n\n"
          "NAME\n       define - create or edit machine\n\n"
          "SYNTAX\n       {d | define} NAME\n\n"
          "DESCRIPTION\n       Open machine listing file in Notepad/Notepad++ if installed.\n\n"},
    {"run", "RUN                                               Execution Commands\n\n"
          "NAME\n       run - compile and execute machine\n\n"
          "SYNTAX\n       {r | run} NAME INPUT [OPTIONS]\n\n"
          "DESCRIPTION\n       Load machine from file and process input. Note that \"\" is processed as void input! If Turing machine doesn't stop, it is killed after exceeding iterations limit.\n\n"
          "       -f, --fast\n              no animation, only output (the default)\n\n"
          "       -s, --seq[=TIME]\n              animated steps, time interval is TIME ms (default 500)\n\n"
          "       -m, --man\n              awaiting manual input after each step\n\n"
          "       --lim=LIMIT\n              set iterations limit (default 1E+8)\n\n"},
    {"pass", "PASS                                              Useless Commands\n\n"
          "NAME\n       pass - do nothing\n\n"
          "SYNTAX\n       {p | pass}\n\n"
          "DESCRIPTION\n       Do literally nothing.\n\n" },
    {"exit", "EXIT                                           Application Commands\n\n"
          "NAME\n       exit - stop application\n\n"
          "SYNTAX\n       {e | exit}\n\n"
          "DESCRIPTION\n       Exit app.\n\n"},
    {"man", "MAN                                           Application Commands\n\n"
          "NAME\n       man - an interface to command manual pages\n\n"
          "SYNTAX\n       {m | man} [COMMAND]\n\n"
          "DESCRIPTION\n       List all available commands. If an argument is provided, display specific command manual page.\n\n"},
    {"rename", "RENAME                                           IO Commands\n\n"
          "NAME\n       rename - rename machine\n\n"
          "SYNTAX\n       {rn | rename} OLD_NAME NEW_NAME\n\n"
          "DESCRIPTION\n       Rename machine file.\n\n"},
    {"list", "LIST                                           IO Commands\n\n"
          "NAME\n       list - show all existing machines\n\n"
          "SYNTAX\n       {ls | list}\n\n"
          "DESCRIPTION\n       List all machine names.\n\n"} };