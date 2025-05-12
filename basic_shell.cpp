
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
#include <cstring>
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
void cmd_cp(const vector<string> &args);
void cmd_mv(const vector<string> &args);
void cmd_grep(const vector<string> &args);
void cmd_chown(const vector<string> &args);
void cmd_cat(const vector<string> &args);
void cmd_rm(const vector<string> &args);
void cmd_touch(const vector<string> &args);
void cmd_mkdir(const vector<string> &args);
void cmd_wc(const vector<string> &args);
void cmd_chmod(const vector<string> &args);

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
    else if (command == "cat") cmd_cat(args);
    else if (command == "rm") cmd_rm(args);
    else if (command == "touch") cmd_touch(args);
    else if (command == "mkdir") cmd_mkdir(args);
    else if (command == "wc") cmd_wc(args);
    else if (command == "chmod") cmd_chmod(args);

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
         << "- chmod user file : \n"
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

void cmd_cat(const vector<string> &args){
    const string filename = args[1];
    char ch;
    FILE *file;
    file = fopen(filename.c_str(), "r");
    if(file == nullptr) {
        cout << "Error opening file" << endl;
    } else {
        cout << "'" << filename << "':"<< endl;
        while ((ch = fgetc(file)) != EOF) {
            putchar(ch);
        }
        cout << endl;
    }
    fclose(file);

}

//Command: rm
void cmd_rm(const vector<string> &args) {
    const string target = args[1];
    struct stat st;
    if(stat(target.c_str(), &st) != 0){
        cout << "File/directory '" << target << "' does not exist." << endl;
    }else{
        if (S_ISDIR(st.st_mode) != 0) {
            //remove directory
            DIR* directory = opendir(target.c_str());
            struct dirent *entry;
            while((entry = readdir(directory)) != nullptr){
                if(!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")){
                    continue;
                }
                string path = target + "/" + entry->d_name;
                cmd_rm({"rm", path});
            }
            closedir(directory);
            if (rmdir(target.c_str()) == 0) {
                cout << "Successfully removed directory: " << target << endl;
            } else {
                cout << "Error removing directory." << endl;
            }
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
        cout << "'"<< filename << "' already exists. Updating timestamps" << endl;
    }else{
        ofstream outfile(filename);
        if(!outfile){
            cout << "Error creating file." << endl;
        }
        outfile.close();
        cout << "File '" << filename << "' created." << endl;
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
        cout << "Directory '" << directoryName << "' created successfully." << endl;
    }
}

void cmd_wc (const vector<string> &args) {
    const string option = args[1];
    const string fileName = args[2];
    char fileChars;
    FILE *file;
    int lines = 0, words = 0, chars = 0;

    if (args.size() != 3) { //move this above option and filename declaractions
        cout << "Error: enter three arguments. " << endl;
    } else {
        file = fopen(fileName.c_str(), "r");
        if (file == nullptr) {
            cout << "Error opening file." << endl;
        }
        if (option == "-l") { //count lines
            while ((fileChars=getc(file)) != EOF) {
                if (fileChars == '\n') {
                    lines++;
                }
            }
            cout << "File '" << fileName << "' contains " << lines << " lines" << endl;
        } else if (option == "-w") { //count words
            bool inWord = false;
            while ((fileChars=getc(file)) != EOF) {
                if (isspace(fileChars)) {
                    inWord = false;
                } else if (!inWord) {
                    words++;
                    inWord = true;
                }
            }
            cout << "File '" << fileName << "' contains " << words << " words" << endl;
        } else if (option == "-c") { //count characters
            while ((fileChars=getc(file)) != EOF) {
                if (fileChars != '\n' || fileChars != ' ') {
                    chars++;
                }
            }
            cout << "File '" << fileName << "' contains " << chars << " characters" << endl;
        }
        fclose(file);
    }

}

//chmod <permission> <filename>
void cmd_chmod (const vector<string> &args) {
    if (args.size() != 3) {
        cout << "Error! Incorrect number of arguments." << endl;
        return;
    }
    const string p_input = args[1];
    const string filename = args[2];

    mode_t permissions = strtol(p_input.c_str(), NULL, 8);

    if (chmod(filename.c_str(), permissions) == 0) {
        cout << "Permissions of file '" << filename << "' changed to " << oct << permissions << endl;
    } else {
        cout << "Error changing permissions." << endl;
    }
}