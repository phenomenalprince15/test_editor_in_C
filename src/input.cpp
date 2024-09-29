#include "input.hpp"

Input::Input() {
    cursorPosition = 0;
    currentCommand = "";
    originalBuffer.clear();
    editorBuffer.clear();
}

Input::~Input() {
    cout << "Bye bye" << endl;
}

/*
- read the input, how do you read ?
- use system call read
*/
char Input::getInput() {
    char buffer[1024];
    std::cout << "> "; // Prompt for user input
    std::cin.getline(buffer, sizeof(buffer)); // Read a line from standard input

    for (size_t i = 0; i < strlen(buffer); ++i) {
        processInput(buffer[i]); // Process each character read
    }

    return 0;
}

void Input::processInput(char input) {
    switch(input) {
        case 'w':
            writeInFile();
            break;
        case 's':
            saveFile();
            break;
        // case 'o': {
        //     std::string filename = getFileName();
        //     openFile(filename);
        //     break;
        // }
        case 'q':
            quitFile();
            break;
        default:
            std::cout << "Unknown Command. Available commands: 's' to save, 'o' to open, 'q' to quit." << std::endl;
            break;
    }
}

string Input::getFileName() {
    string filename;
    cout << "Enter a file name" << endl;
    getline(cin, filename);
    return filename;
}

void Input::writeInFile() {
    std::cout << "Entering write mode. Type your text (press 's' to save):" << std::endl;
    std::cout << "Entering write mode. Type your text (press 'q' to quit):" << std::endl;

    std::string line;
    while (true) {
        std::getline(std::cin, line); // Read a whole line of input
        
        // Check for save command
        if (line == "s") {
            saveFile(); // Save the file if 's' is entered
            break; // Exit writing mode after saving
        }

        if (line == "q") {
            quitFile(); 
            break;
        }

        // Add the line to the editor buffer
        editorBuffer.push_back(line); 
        std::cout << "Line added. Type more text or 's' to save or 'q to quit:" << std::endl;
    }
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


