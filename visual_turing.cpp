#include <conio.h>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <future>

#include "turing.h"
#include "manual.h"

struct Command {
    vector<string> arguments;

    Command() : arguments({ "pass" }) {}
    Command(string& command_text) {
        if (command_text.empty()) {
            arguments.push_back("pass");
            return;
        }
        command_text += " ";
        size_t pos = 0;
        while ((pos = command_text.find(" ")) != string::npos) {
            arguments.push_back(command_text.substr(0, pos));
            command_text.erase(0, pos + 1);
            while (!command_text.empty() && command_text[0] == ' ')
                command_text.erase(0, 1);
        }
    }

    const string& Name() const {
        return arguments[0];
    }

    const string& Get(int index) const {
        return arguments[index];
    }

    int Count() const {
        return arguments.size() - 1;
    }
};

void EditMachine(const string& name) {
    std::filesystem::path cwd = std::filesystem::current_path() / "Machines" / (name + ".txt");
    std::ifstream fin("C:\\Program Files (x86)\\Notepad++\\notepad++.exe");
    if (fin.good()) {
        system(&("\"\"C:\\Program Files (x86)\\Notepad++\\notepad++.exe\" \"" + cwd.string() + "\"\"")[0]);
    } else {
        system(&("\"\"C:\\WINDOWS\\system32\\notepad.exe\" \"" + cwd.string() + "\"\"")[0]);
    }
}

int sleep_time;

void RunMachine(Machine& turing, const string& input, const string& mode, int limit = 1E+8) {
    int(*func)() = []() {
        return 0;
    };
    cancel_task = false;
    if (mode.find("-s") == 0 || mode.find("--seq") == 0) {
        IO::ClearScreen();
        IO::SetCaretLastLine();
        IO::PrintLightGrayBack("Press Esc to stop machine");
        sleep_time = 500;
        if (mode.find("-s=") == 0) {
            IO::Print("Wrong options!\n");
            throw std::exception();
        }
        if (mode.find("=") == 5)
            sleep_time = std::stoi(mode.substr(6));
        func = []() {
            Sleep(sleep_time);
            return 0;
        };
        auto future_process = std::async([&turing, &input, func, limit]() { turing.Process(input == "\"\"" ? "" : input, func, false, limit); });
        while (future_process.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                cancel_task = true;
            }
        }
    } else if (mode == "-m" || mode == "--man") {
        IO::ClearScreen();
        IO::SetCaretLastLine();
        IO::PrintLightGrayBack("Press Enter to continue, s to stop machine, e to exit manual mode");
        func = []() {
            char c = _getch();
            if (c == 's')
                return 1;
            else if (c == 'e')
                return 2;
            return 0;
        };
        turing.Process(input == "\"\"" ? "" : input, func, false, limit);
    } else if (mode == "-f" || mode == "--fast") {
        turing.Process(input == "\"\"" ? "" : input, func, true, limit);
    } else {
        IO::Print("Wrong options!\n");
        throw std::exception();
    }
}

Machine LoadMachine(const string& name) {
    std::ifstream fin("Machines/" + name + ".txt");
    if (!fin.good()) {
        IO::PrintRed("Machine " + name + " does not exist!\n");
        throw std::exception();
    }
    string line;
    vector<string> list;
    while (std::getline(fin, line)) {
        list.push_back(line);
    }
    return Machine(list);
}

int main() {
    //IO::PrintRainbow("wubba lubba dub dub");
    IO::Print("TURING TERMINAL 1.1.0\nSergejVolkov 2020\n\n");
    string command_text;
    while (true) {
        IO::PrintGreen("turing@Machine");
        IO::Print(":");
        IO::PrintBlue("~");
        IO::Print("/bin");
        IO::PrintGray("$ ");
        std::getline(std::cin, command_text);
        Command command(command_text);
        if (command.Name() == "pass" || command.Name() == "p") {
        } else if (command.Name() == "list" || command.Name() == "ls") {
            for (const auto & entry : std::filesystem::directory_iterator("Machines/")) {
                string name = entry.path().string();
                if (name.find(".txt") != string::npos)
                    std::cout << name.substr(9, name.size() - 13) << '\n';
            }
        } else if (command.Name() == "define" || command.Name() == "d") {
            if (command.Count() == 1) {
                EditMachine(command.Get(1));
            } else {
                IO::PrintRed(command.Name() + ": wrong arguments\n");
            }
        } else if (command.Name() == "rename" || command.Name() == "rn") {
            if (command.Count() == 2) {
                std::ifstream fin(command.Get(1) + ".txt");
                if (fin.good()) {
                    fin.close();
                    std::rename(&(command.Get(1) + ".txt")[0], &(command.Get(2) + ".txt")[0]);
                } else {
                    IO::PrintRed("Machine " + command.Get(1) + " does not exist!\n");
                }
            } else {
                IO::PrintRed(command.Name() + ": wrong arguments\n");
            }
        } else if (command.Name() == "run" || command.Name() == "r") {
            try {
                Machine turing = LoadMachine(command.Get(1));
                if (command.Count() == 2) {
                    RunMachine(turing, command.Get(2), "-f");
                } else if (command.Count() == 3) {
                    if (command.Get(3).find("--lim=") == 0) {
                        RunMachine(turing, command.Get(2), "-f", std::stoi(command.Get(3).substr(6)));
                    } else {
                        RunMachine(turing, command.Get(2), command.Get(3));
                    }
                } else if (command.Count() == 4) {
                    if (command.Get(3).find("--lim=") == 0) {
                        RunMachine(turing, command.Get(2), command.Get(4), std::stoi(command.Get(3).substr(6)));
                    } else {
                        RunMachine(turing, command.Get(2), command.Get(3), std::stoi(command.Get(4).substr(6)));
                    }
                } else {
                    IO::PrintRed(command.Name() + ": wrong arguments\n");
                }
            } catch (...) {
                IO::PrintRed(command.Name() + ": something went wrong, please try again\n");
            }
        } else if (command.Name() == "man" || command.Name() == "m") {
            if (command.Count() == 0) {
                std::cout << "What manual page do you want?\n"
                    "For example, try 'man man'.\n\n";
                IO::PrintBlue("Available commands:\n");
                for (const auto& line : man_list_commands)
                    std::cout << line << '\n';
            } else if (command.Count() == 1) {
                auto iter = man_commands.find(command.Get(1));
                if (iter == man_commands.end()) {
                    std::cout << "No manual entry for " << command.Get(1) << '\n';
                } else {
                    Sleep(100);
                    IO::ClearScreen();
                    std::cout << iter->second;
                    IO::SetCaretLastLine();
                    IO::PrintLightGrayBack("Press any key to exit manual");
                    _getch();
                    IO::ClearScreen();
                }
            } else {
                IO::PrintRed(command.Name() + ": wrong arguments\n");
            }
        } else if (command.Name() == "exit" || command.Name() == "e") {
            return 0;
        } else {
            IO::PrintRed(command.Name() + ": command not found\n");
        }
    }
    return 0;
}
