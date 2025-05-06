
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
#include <cstring>
#include <pwd.h>
#include <limits.h>
#include <fstream>
#include <utime.h>

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
void cmd_touch(const vector<string> &args);
void cmd_mkdir(const vector<string> &args);
void cmd_rm(const vector<string> &args);

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
    } else if (command == "touch") {
        cmd_touch(args);
    } else if (command == "mkdir") {
        cmd_mkdir(args);
    } else if (command == "rm") {
        cmd_rm(args);
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


//Command: cp
void cmd_cp(const vector<string> &args) {

}

//Command: mv
void cmd_mv(const vector<string> &args) {

}

//Command: rm
void cmd_rm(const vector<string> &args) {
    //check if file or directory
    //if doesnt exist, error message
    //if file remove file, if directory remove directory
    const string target = args[1];
    struct stat st;
    if(stat(target.c_str(), &st) != 0){
        cout << "File/directory '" << target << "' does not exist." << endl;
    }else{
        if (S_ISDIR(st.st_mode) != 0) {
            //remove directory
            //maybe empty out directory contents to allow use of rmdir()
        } else {
            //remove file
            if(remove(target.c_str()) == 0){
                cout << "Successfully removed file: " << target << endl;
            }else{
                cout << "Error removing file." << endl;
            }
        }
    }
}

//Command: touch
void cmd_touch(const vector<string> &args) {
    //if file exists update timestamp else create file
    const string filename = args[1];
    
    struct stat st;
    if(stat(filename.c_str(), &st) == 0){
    //might need more lines to properly update timestamps
        utime(filename.c_str(), nullptr);
        cout << filename << " already exists. Updating timestamps" << endl;
    }else{
        ofstream outfile(filename);
        if(!outfile){
            cout << "Error creating file." << endl;
        }
        outfile.close();
        cout << "File created." << endl;
    }
}


//Command: mkdir
void cmd_mkdir(const vector<string> &args) {
    const string directoryName = args[1];
    struct stat st;
    if(stat(directoryName.c_str(), &st) == 0){
        cout << "Directory " << directoryName << " already exists." << endl;
    }else{
        mkdir(directoryName.c_str(), 0700);
        cout << "Directory created successfully." << endl;
    }
}