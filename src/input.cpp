#include "input.hpp"

Input::Input() {
    cursorPosition = 0;
    currentCommand = "";
    originalBuffer.clear();
    editorBuffer.clear();
    currentMode = COMMAND; // Start in command mode
}

Input::~Input() {
    cout << "Bye bye" << endl;
}

/*
- read the input, how do you read ?
- use system call read
*/
char Input::getInput() {
    char ch;
    std::cout << "> "; // Prompt for user input

    while (true) {
        ch = std::cin.get(); // Read one character

        // Handle mode switching
        if (currentMode == COMMAND && ch == 'i') {
            toggleMode();
            processInput(ch);
        } else {
            cout << "Press i to get into insert mode" << endl;
        }

        if (currentMode == COMMAND && ch == 's') {
            processInput(ch);
            quitFile();
        }

        if (currentMode == COMMAND && ch == 'q') {
            quitFile();
        }

        updateDisplay(); // Update display after each character
    }

    return 0; // Optionally return 0 if needed
}

void Input::updateDisplay() {
    std::cout << "\r\033[K"; // Clear the current line
    std::cout << "Current Mode: " << (currentMode == INSERT ? "Insert" : "Command") << std::endl;
    std::cout << "Current buffer content:" << std::endl;

    // Display the entire buffer
    for (const auto& bufferLine : editorBuffer) {
        std::cout << bufferLine << std::endl;
    }

    // Show the prompt again based on the mode
    std::cout << (currentMode == COMMAND ? "> " : "  "); // Indent in Insert Mode
}

void Input::processInput(char input) {
    switch(input) {
        case 'i':
            writeInFile();
            break;
        case 's':
            saveFile();
            break;
        case 'q':
            quitFile();
            break;
        case 'm':
            toggleMode();
            break;
        default:
            std::cout << "Unknown Command. Available commands: 's' to save, 'q' to quit, 'm' to toggle mode." << std::endl;
            break;
    }
}

void Input::writeInFile() {
    std::cout << "Entering write mode. Type your text (press 'Esc' to switch to command mode):" << std::endl;

    currentMode = INSERT; // Switch to Insert Mode

    // Main input loop for Insert Mode
    while (currentMode == INSERT) {
        char ch = std::cin.get(); // Read one character

        // Check for escape key to switch back to Command Mode
        if (ch == 27) { // ASCII for Escape key
            toggleMode();  // Switch to command mode
            break;         // Exit insert mode
        }

        // Handle line breaks (Enter key)
        if (ch == '\n') {
            editorBuffer.push_back(""); // Start a new line in the buffer
            std::cout << std::endl;     // Move cursor to new line in terminal
        }
        // Handle backspace (ASCII value 8 or 127)
        else if (ch == 8 || ch == 127) { 
            if (!editorBuffer.empty()) {
                // Remove the last character from the last line in the buffer
                if (!editorBuffer.back().empty()) {
                    editorBuffer.back().pop_back();
                } else {
                    // If the last line is empty, remove the line itself
                    editorBuffer.pop_back();
                }
                // Move cursor back and clear current line
                std::cout << "\r" << std::string(80, ' ') << "\r"; // Clear line
                std::cout << (editorBuffer.empty() ? "" : editorBuffer.back()); // Display updated line
            }
        } else {
            // Add the character to the last line in the buffer
            if (!editorBuffer.empty()) {
                editorBuffer.back() += ch;
            } else {
                editorBuffer.push_back(std::string(1, ch)); // Add first character
            }

            // Display the character in real-time (like in Vim)
            std::cout << ch;
        }

        updateDisplay(); // Update the display after each character if needed
    }
}


void Input::toggleMode() {
    if (currentMode == INSERT) {
        currentMode = COMMAND; // Switch to Command Mode
        std::cout << "\nSwitched to Command Mode." << std::endl;
    } else {
        currentMode = INSERT; // Switch to Insert Mode
        std::cout << "\nSwitched to Insert Mode." << std::endl;
    }
}

string Input::getFileName() {
    string filename;
    cout << "Enter a file name" << endl;
    getline(cin, filename);
    return filename;
}

/*
Save File logic
- get file name from user
- I need to store whatever is present in the editor buffer
*/
void Input::saveFile() {
    std::cout << "Saving the file..." << std::endl;

    if (currentFileName.empty()) {
        std::cerr << "Error: No file currently opened to save." << std::endl;
        return;
    }

    std::ofstream outFile(currentFileName);  // Use the stored filename
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }

    for (const std::string& line : editorBuffer) {
        outFile << line << std::endl;  // Write each line from the buffer to the file
    }

    outFile.close();
    originalBuffer = editorBuffer;
    std::cout << "File saved successfully." << std::endl;
}

void Input::createNewFile(const std::string& filename) {
    std::ofstream newFile(filename);  // Create a new file

    isItaNewFile = true;

    if (!newFile.is_open()) {
        std::cerr << "Error: Could not create file." << std::endl;
        return;
    }

    newFile.close();
    currentFileName = filename;  // Store the newly created file's name
    std::cout << "New file created: " << filename << std::endl;
}


void Input::openFile(const std::string& filename) {
    std::cout << "Opening a file..." << std::endl;
    
    currentFileName = filename;
    // Open the file for reading
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file for reading." << std::endl;
        quitFile();
        return;
    }

    // Clear the current buffer
    editorBuffer.clear();

    // Read the file line by line and store it in editorBuffer
    std::string line;
    while (getline(inFile, line)) {
        editorBuffer.push_back(line);
    }

    // Set original buffer to the content we just read
    originalBuffer = editorBuffer;

    // Close the file after reading
    inFile.close();

    std::cout << "File opened successfully." << std::endl;
}

void Input::quitFile() {
    cout << "Quitting the editor..." << endl;

    // Check for unsaved changes
    bool hasUnsavedChanges = checkForUnsavedChanges();
    if (hasUnsavedChanges) {
        std::cout << "You have unsaved changes. Save before quitting? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'n' && isItaNewFile) {
            std::cout << "This is a new file and has unsaved changes. Deleting it..." << std::endl;
            std::remove(currentFileName.c_str()); // Delete the new file
        } else if (choice == 'y') {
            saveFile();
        }
    }

    cout << "Exiting the editor." << endl;
    exit(0);
}

bool Input::checkForUnsavedChanges() {
    // Compare the current buffer with the original buffer
    if (editorBuffer != originalBuffer) {
        return true;
    }
    return false;
}

