
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <fstream>
#include <limits.h>
#include <fcntl.h>

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
void cmd_cp(const vector<string> &args);
void cmd_mv(const vector<string> &args);
void cmd_grep(const vector<string> &args);
void cmd_chown(const vector<string> &args);

int main() {
    cout << "Welcome to Basic OS Shell (C++)" << endl;
    shellLoop();
    return 0;
}

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

vector<string> tokenize(const string &input) {
    vector<string> tokens;
    istringstream stream(input);
    string token;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void executeCommand(const vector<string> &args) {
    string command = args[0];

    if (command == "cd") cmd_cd(args);
    else if (command == "pwd") cmd_pwd();
    else if (command == "dir") cmd_dir(args);
    else if (command == "clr") cmd_clr();
    else if (command == "echo") cmd_echo(args);
    else if (command == "environ") cmd_environ();
    else if (command == "help") cmd_help();
    else if (command == "pause") cmd_pause();
    else if (command == "quit") cmd_quit();
    else if (command == "cp") cmd_cp(args);
    else if (command == "mv") cmd_mv(args);
    else if (command == "grep") cmd_grep(args);
    else if (command == "chown") cmd_chown(args);
    else cout << "Unsupported command: " << command << endl;
}

void cmd_cd(const vector<string> &args) {
    const char *path = (args.size() > 1) ? args[1].c_str() : getenv("HOME");
    if (chdir(path) != 0) perror("cd failed");
}

void cmd_pwd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) cout << cwd << endl;
    else perror("pwd error");
}

void cmd_dir(const vector<string> &args) {
    const char *path = (args.size() > 1) ? args[1].c_str() : ".";
    DIR *dir = opendir(path);
    if (dir) {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) cout << entry->d_name << "  ";
        cout << endl;
        closedir(dir);
    } else perror("dir error");
}

void cmd_clr() {
    cout << "\033[2J\033[1;1H";
}

void cmd_echo(const vector<string> &args) {
    for (size_t i = 1; i < args.size(); ++i) cout << args[i] << " ";
    cout << endl;
}

void cmd_environ() {
    extern char **environ;
    for (int i = 0; environ[i]; ++i) cout << environ[i] << endl;
}

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
         << "- quit           : Exit the shell\n"
         << "- cp src dest    : Copy file from src to dest\n"
         << "- mv src dest    : Move file from src to dest\n"
         << "- grep pattern file : Search pattern in file\n"
         << "- chown user file : Change ownership of file\n";
}

void cmd_pause() {
    cout << "Press Enter to continue..." << endl;
    cin.ignore();
}

void cmd_quit() {
    cout << "Exiting Basic OS Shell..." << endl;
    exit(0);
}

void cmd_cp(const vector<string> &args) {
    if (args.size() < 3) {
        cerr << "Usage: cp <source> <destination>" << endl;
        return;
    }
    ifstream src(args[1], ios::binary);
    ofstream dst(args[2], ios::binary);
    dst << src.rdbuf();
    cout << "File copied successfully." << endl;
}

void cmd_mv(const vector<string> &args) {
    if (args.size() < 3) {
        cerr << "Usage: mv <source> <destination>" << endl;
        return;
    }
    if (rename(args[1].c_str(), args[2].c_str()) != 0) perror("mv failed");
    else cout << "File moved successfully." << endl;
}

void cmd_grep(const vector<string> &args) {
    if (args.size() < 3) {
        cerr << "Usage: grep <pattern> <file>" << endl;
        return;
    }
    ifstream file(args[2]);
    string line;
    while (getline(file, line)) {
        if (line.find(args[1]) != string::npos) cout << line << endl;
    }
}

void cmd_chown(const vector<string> &args) {
    if (args.size() < 3) {
        cerr << "Usage: chown <user> <file>" << endl;
        return;
    }
    struct passwd *pw = getpwnam(args[1].c_str());
    if (!pw) {
        perror("Invalid user");
        return;
    }
    if (chown(args[2].c_str(), pw->pw_uid, -1) != 0) perror("chown failed");
    else cout << "Ownership changed successfully." << endl;
}
