#include <iostream>
#include <sys/stat.h>
#include "input.hpp"
#include <termios.h>

struct termios orig_termios;
void enableRawMode();
void disableRawMode();

void enableRawMode() {
    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode); // Ensure that normal mode is restored at exit

    struct termios raw = orig_termios; // Make a copy of original settings
    raw.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    raw.c_cc[VMIN] = 1; // Minimum number of characters to read
    raw.c_cc[VTIME] = 0; // No timeout

    // Set the new terminal attributes
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}


bool fileExists(const std::string& filename) {
    struct stat buffer; 
    return (stat(filename.c_str(), &buffer) == 0);
}


int main(int argc, char** argv) {
    char cwd[1024];
    
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd() error");
    }

    enableRawMode();

    Input editor; // Create Input object
    
    if (argc > 1) {
        std::string filename = argv[1];
        
        // Check if the file exists
        if (fileExists(filename)) {
            std::cout << "File exists. Opening file..." << std::endl;
            editor.openFile(filename); // Open existing file
        } else {
            std::cout << "File does not exist. Creating a new one..." << std::endl;
            editor.createNewFile(filename); // Create a new file
            editor.openFile(filename); // Open the new file
        }
    } else {
        std::cerr << "No filename provided. Exiting." << std::endl;
        cout << "provide filename like | ./text_editor hello.txt" << endl;
        return 1;
    }

    while (true) {
        editor.getInput();
    }

    return 0;
}
