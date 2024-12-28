# CLIKit Documentation

## Introduction

The `CLIKit` library is a versatile toolkit designed to enhance the functionality and user experience of command-line interface (CLI) applications. It offers features for terminal manipulation, colorized text output, user input handling, and a variety of utility functions for formatting and interactive elements.

----------

## Table of Contents

1.  [Dependencies](#dependencies)
2.  [Structures and Enumerations](#structures-and-enumerations)
3.  [Namespace and Functions](#namespace-and-functions)
4.  [Usage Examples](#usage-examples)
5.  [Notes](#notes)
6.  [License](#license)

----------

## Dependencies

This library requires the following standard and platform-specific headers:

-   **Standard headers:**
    -   `<iostream>`
    -   `<string>`
    -   `<chrono>`
    -   `<thread>`
    -   `<vector>`
    -   `<limits>`
    -   `<optional>`
    -   `<sstream>`
    -   `<type_traits>`
    -   `<functional>`
    -   `<random>`
    -   `<ctime>`
-   **Platform-specific headers:**
    -   **Windows (`_WIN32`):**
        -   `<conio.h>`
        -   `<windows.h>`
    -   **Non-Windows:**
        -   `<termios.h>`
        -   `<unistd.h>`
        -   `<sys/ioctl.h>`

----------

## Structures and Enumerations

### `enum class Key`

Defines key types for input handling:

-   **`Unknown`**: Undefined key.
-   **`Char`**: Printable characters (letters, digits, etc.).
-   **`Space`**: Space key.
-   **`Enter`**: Enter key.
-   **`Backspace`**: Backspace key.
-   **`UpArrow`**: Up arrow key.
-   **`DownArrow`**: Down arrow key.
-   **`LeftArrow`**: Left arrow key.
-   **`RightArrow`**: Right arrow key.
-   **`Escape`**: Escape key.

### `struct KeyResult`

Represents the result of a key press:

-   **`Key key`**: Type of key pressed.
-   **`char ch`**: ASCII character for `Key::Char`.

### `struct Color`

Contains static inline constants for ANSI color codes:

-   **Standard colors:** `RED`, `ORANGE`, `YELLOW`, `GREEN`, `BLUE`, `PURPLE`, `CYAN`, `WHITE`, `GRAY`, `BLACK`.
-   **Light colors:** `LIGHT_RED`, `LIGHT_ORANGE`, `LIGHT_YELLOW`, etc.
-   **`RESET`**: Resets the text to default color.

----------

## Namespace and Functions

### Namespace `CLIKit`

All library functions are encapsulated within the `CLIKit` namespace.

#### Utility Functions

1.  **`void sleep(int milliseconds)`** Pauses execution for the specified number of milliseconds.
    
2.  **`std::tm LocalTimeNow()`** Retrieves the current local time as a `std::tm` object.
    
3.  **`std::string GetTimestamp(...)`** Generates a formatted timestamp with customizable components and optional colorization.
    

#### Input Functions

4.  **`template<typename T> std::optional<T> GetInput()`** Reads user input of type `T`.
    
5.  **`template<typename T> std::optional<T> GetMaskedInput(...)`** Reads user input with characters masked (e.g., for passwords).
    
6.  **`void WaitForInput(const std::string& message)`** Displays a message and waits for the user to press any key.
    

#### Output Functions

7.  **`void PrintWarning(const std::string& msg)`** Prints a warning message in yellow.
    
8.  **`void PrintError(const std::string& msg)`** Prints an error message in red.
    
9.  **`void PrintSuccess(const std::string& msg)`** Prints a success message in green.
    
10.  **`void PrintInfo(const std::string& msg)`** Prints an informational message in cyan.
    
11.  **`void RenderASCIIArt(const std::string& ascii, bool center = false)`** Displays ASCII art, optionally centered in the terminal.
    
12.  **`void PrintCentered(const std::string& text)`** Centers the given text in the terminal.
    

#### Interactive Effects

13.  **`void TypewriterEffect(...)`** Simulates text appearing character by character, like a typewriter.
    
14.  **`void WordByWordEffect(...)`** Displays text one word at a time.
    
15.  **`void PrintSpinner(...)`** Displays a spinning indicator for a specified duration.
    
16.  **`std::string ProgressBar(...)`** Generates a customizable progress bar string.
    

#### Terminal Functions

17.  **`void SetConsoleTitle(const std::string& title)`** Sets the title of the terminal window.
    
18.  **`int GetTerminalWidth()`** Retrieves the terminal’s width in characters.
    
19.  **`void ShowTooltip(...)`** Displays a tooltip box with customizable content and alignment.
    
20.  **`bool KeyboardEvent(const std::string& desiredKey)`** Detects and matches specific key events.
    

----------

## Usage Examples

### Example: Printing Colored Text

```cpp
#include "CLIKit.h"

int main() {
    CLIKit::PrintInfo("Hello, World in Cyan!");
    CLIKit::PrintWarning("This is a warning message!");
    CLIKit::PrintError("An error occurred.");
    CLIKit::PrintSuccess("Operation completed successfully.");
    return 0;
}
```

### Example: Progress Bar

```cpp
#include "CLIKit.h"

int main() {
    for (int i = 0; i <= 100; ++i) {
        std::cout << "\r" << CLIKit::ProgressBar(
            i, 100, 50, "Progress:", "Done", "=", "-",
            CLIKit::Color::GREEN, CLIKit::Color::GRAY,
            CLIKit::Color::WHITE, CLIKit::Color::LIGHT_GREEN,
            CLIKit::Color::WHITE, CLIKit::Color::LIGHT_BLUE,
            true, true, true
        ) << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << std::endl;
    return 0;
}
```

### Example: Input Handling

```cpp
#include "CLIKit.h"

int main() {
    CLIKit::PrintInfo("Enter your name:");
    auto name = CLIKit::GetInput<std::string>();
    if (name) {
        CLIKit::PrintSuccess("Hello, " + *name + "!");
    } else {
        CLIKit::PrintError("Failed to read name.");
    }
    return 0;
}
```

----------

## Notes
-   Ensure ANSI color support is enabled in your terminal.

----------

## License

This project uses a MIT liscence, for more information refer to the LISCENCE file.

----------
