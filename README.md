# text_editor

cmake -G "Ninja" ..
ninja

```
text_editor_cpp/
│
├── src/                # Source files
│   ├── main.cpp        # Main entry point of the program
│   ├── Terminal.cpp    # Terminal control class implementation
│   ├── Input.cpp       # Input handling class implementation
│   ├── Screen.cpp      # Screen rendering class implementation
│   ├── TextBuffer.cpp  # Text buffer management class implementation
│   └── FileIO.cpp      # File input/output class implementation
│
├── include/            # Header files
│   ├── Terminal.hpp    # Terminal control class definition
│   ├── Input.hpp       # Input handling class definition
│   ├── Screen.hpp      # Screen rendering class definition
│   ├── TextBuffer.hpp  # Text buffer management class definition
│   └── FileIO.hpp      # File input/output class definition
│
├── Makefile            # Build script to compile the project
│
├── README.md           # Project documentation and instructions
│
└── LICENSE             # License information (if applicable)
```