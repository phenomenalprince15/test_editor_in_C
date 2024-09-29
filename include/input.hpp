#pragma once

#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>
#include <unistd.h>
#include <fcntl.h> 
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

const char ESCAPE_KEY = 27;
const char BACKSPACE_KEY = 127;

using namespace std;

class Input {

    public:
        // constructor and Destructor
        Input();
        ~Input();

        // Method to capture user Input
        char getInput();

        void openFile(const std::string& filename);
        std::string getFileName();
        void writeInFile();
        void createNewFile(const std::string& filename);
        enum Mode { INSERT, COMMAND };

    private:
        void handleCharacterInput(char input);
        void handleArrowKeys(char input);
        void handleCommandInput(const std::string& command); // save, insert, delete, quit

        // Different operations
        void processInput(char input);
        void saveFile();
        void quitFile();
        bool checkForUnsavedChanges();
        void toggleMode();
        void updateDisplay();
        void moveCursorUp();
        void moveCursorDown();
        void moveCursorRight();
        void moveCursorLeft();

        int rowPosition;
        int columnPosition;
        int cursorPosition;
        std::string currentCommand;
        Mode currentMode;

        std::vector<std::string> editorBuffer;
        std::vector<std::string> originalBuffer;
        string currentFileName;
        bool isItaNewFile = false;
        bool isTextMode;
};

#endif