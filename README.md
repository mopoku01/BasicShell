A C++-based custom shell created as part of the COSC 439 Operating Systems course at Towson University.
This project simulates a simplified Unix-like shell with over 20 core commands manually implemented using C++ and POSIX system calls with no external system() usage.

Features

BasicShell supports the following core functionalities:

File and Directory Management
- cd [dir] – Change directory
- pwd – Print working directory
- dir [dir] – List directory contents
- mkdir [dir] – Create a directory
- rm [file/dir] – Delete file or recursively delete directories
- cp [src] [dest] – Copy file
- mv [src] [dest] – Move or rename file
- touch [file] – Create a file or update timestamps

File Content and Permissions
- cat [file] – Display file contents
- echo [text] – Print message to terminal
- grep [pattern] [file] – Search for pattern in file
- wc -l|-w|-c [file] – Count lines, words, or characters
- chmod [perm] [file] – Change file permissions (e.g., 755)
- chown [user] [file] – Change file ownership

Environment and Shell Operations
- clr – Clear the screen
- environ – List environment variables
- pause – Wait for user to press Enter
- help – Display help manual for all commands
- quit – Exit the shell

Highlights

- Fully built using C++ and POSIX APIs
- Manual implementation of command parsing and dispatching
- Recursive logic for directory deletion
- No use of system() or shell wrappers — 100 percent internal logic
- User-friendly shell prompt (osh>) and error feedback
- Version control and collaboration via GitHub

Example Interaction

osh> mkdir demo
osh> cd demo
osh> touch file.txt
osh> echo Hello World > file.txt
osh> cat file.txt
osh> wc -w file.txt
osh> grep Hello file.txt
osh> chmod 600 file.txt
osh> rm file.txt
osh> quit

Collaborators

- Michael Opoku
- Elise Dzimeyor

Project Info

- Course: COSC 439 – Operating Systems
- Semester: Spring 2025
- Institution: Towson University

Lessons Learned

- How real shells work under the hood
- Direct interaction with OS-level APIs
- Managing processes, directories, and permissions manually
- Handling edge cases and recursive logic
- GitHub collaboration and conflict resolution

Repository

https://github.com/mopoku01/BasicShell
