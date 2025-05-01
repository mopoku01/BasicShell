
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <pwd.h>
#include <limits.h>

using namespace std;

// Function declarations
void shellLoop();
vector<string> tokenize(const string &input);
void executeCommand(const vector<string> &args);
void cmd_cd(const vector<string> &args);
void cmd_pwd();
void cmd_dir(const vector<string> &args);
void cmd_clr();
void cmd_echo(const vector<string> &args);
void cmd_environ();
void cmd_help();
void cmd_pause();
void cmd_quit();

int main() {
    cout << "Welcome to Basic OS Shell (C++)" << endl;
    shellLoop();
    return 0;
}

// Main loop
void shellLoop() {
    string input;
    while (true) {
        cout << "osh> ";
        getline(cin, input);
        vector<string> args = tokenize(input);
        if (!args.empty()) {
            executeCommand(args);
        }
    }
}

// Tokenize input string into command + args
vector<string> tokenize(const string &input) {
    vector<string> tokens;
    istringstream stream(input);
    string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Command execution handler
void executeCommand(const vector<string> &args) {
    string command = args[0];

    if (command == "cd") {
        cmd_cd(args);
    } else if (command == "pwd") {
        cmd_pwd();
    } else if (command == "dir") {
        cmd_dir(args);
    } else if (command == "clr") {
        cmd_clr();
    } else if (command == "echo") {
        cmd_echo(args);
    } else if (command == "environ") {
        cmd_environ();
    } else if (command == "help") {
        cmd_help();
    } else if (command == "pause") {
        cmd_pause();
    } else if (command == "quit") {
        cmd_quit();
    } else {
        cout << "Unsupported command: " << command << endl;
    }
}

// Command: cd
void cmd_cd(const vector<string> &args) {
    const char *path = (args.size() > 1) ? args[1].c_str() : getenv("HOME");
    if (chdir(path) != 0) {
        perror("cd failed");
    }
}

// Command: pwd
void cmd_pwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        cout << cwd << endl;
    } else {
        perror("pwd error");
    }
}

// Command: dir
void cmd_dir(const vector<string> &args) {
    const char *path = (args.size() > 1) ? args[1].c_str() : ".";
    DIR *dir = opendir(path);
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            cout << entry->d_name << "  ";
        }
        cout << endl;
        closedir(dir);
    } else {
        perror("dir error");
    }
}

// Command: clr
void cmd_clr() {
    cout << "\033[2J\033[1;1H";  // ANSI escape to clear screen
}

// Command: echo
void cmd_echo(const vector<string> &args) {
    for (size_t i = 1; i < args.size(); ++i) {
        cout << args[i] << " ";
    }
    cout << endl;
}

// Command: environ
void cmd_environ() {
    extern char **environ;
    for (int i = 0; environ[i]; ++i) {
        cout << environ[i] << endl;
    }
}

// Command: help
void cmd_help() {
    cout << "Basic OS Shell Help:\n"
         << "- cd [dir]       : Change directory\n"
         << "- pwd            : Print working directory\n"
         << "- dir [dir]      : List directory contents\n"
         << "- clr            : Clear the screen\n"
         << "- echo [text]    : Print text\n"
         << "- environ        : List environment variables\n"
         << "- help           : Show help\n"
         << "- pause          : Wait for user to press Enter\n"
         << "- quit           : Exit the shell\n";
}

// Command: pause
void cmd_pause() {
    cout << "Press Enter to continue..." << endl;
    cin.ignore();
}

// Command: quit
void cmd_quit() {
    cout << "Exiting Basic OS Shell..." << endl;
    exit(0);
}
