#include <iostream>
#include <sys/stat.h>
#include "input.hpp"

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
