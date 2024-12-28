#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <limits>
#include <optional>
#include <sstream>
#include <type_traits>
#include <functional>
#include <random>
#include <ctime>

#ifdef _WIN32
    #define NOMINMAX           // <-- IMPORTANT to avoid macro conflicts
    #include <conio.h>         // Windows-only
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>        // For STDIN_FILENO
    #include <sys/ioctl.h>
#endif

enum class Key
{
    Unknown,
    Char,       // e.g. letters, digits, etc.
    Space,
    Enter,
    Backspace,
    UpArrow,
    DownArrow,
    LeftArrow,
    RightArrow,
    Escape,
};

struct KeyResult
{
    Key key;
    char ch; // For Key::Char, store the ASCII character
};

struct Color {
    // rainbow
    static inline constexpr const char* RED = "\033[31m";
    static inline constexpr const char* ORANGE = "\033[38;5;208m";
    static inline constexpr const char* YELLOW = "\033[33m";
    static inline constexpr const char* GREEN = "\033[32m";
    static inline constexpr const char* BLUE = "\033[34m";
    static inline constexpr const char* PURPLE = "\033[35m";
    static inline constexpr const char* CYAN = "\033[36m";

    // normal
    static inline constexpr const char* WHITE = "\033[37m";
    static inline constexpr const char* GRAY = "\033[90m";
    static inline constexpr const char* BLACK = "\033[30m";

    // light
    static inline constexpr const char* LIGHT_RED = "\033[91m";
    static inline constexpr const char* LIGHT_ORANGE = "\033[38;5;214m";
    static inline constexpr const char* LIGHT_YELLOW = "\033[93m";
    static inline constexpr const char* LIGHT_GREEN = "\033[92m";
    static inline constexpr const char* LIGHT_BLUE = "\033[94m";
    static inline constexpr const char* LIGHT_PURPLE = "\033[95m";
    static inline constexpr const char* LIGHT_CYAN = "\033[96m";

    // reset
    static inline constexpr const char* RESET = "\033[0m";

    // custom colors...
    // example: static inline constexpr const char* CUSTOM_COLOR = "\033[0m";
};

namespace CLIKit {

    inline void sleep(static int Miliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(Miliseconds));
    }

    inline std::tm LocalTimeNow() {
        using namespace std::chrono;
        std::time_t now_c = system_clock::to_time_t(system_clock::now());
        std::tm local_tm;
#ifdef _WIN32
        localtime_s(&local_tm, &now_c);
#else
        local_tm = *std::localtime(&now_c);
#endif
        return local_tm;
    }

    inline std::string GetTimestamp(
        bool addYear = true,
        bool addMonth = true,
        bool addDay = true,
        bool addHour = true,
        bool addMinute = true,
        bool addSecond = true,
        bool is24HourFormat = true,
        const std::string& colorYear = "",
        const std::string& colorMonth = "",
        const std::string& colorDay = "",
        const std::string& colorHour = "",
        const std::string& colorMinute = "",
        const std::string& colorSecond = "",
        const std::string& colorAMPM = ""
    )
    {
        const std::string& colorReset = Color::RESET;

        std::tm local_tm = LocalTimeNow();

        // Extract components
        int year = 1900 + local_tm.tm_year;
        int month = local_tm.tm_mon + 1;
        int day = local_tm.tm_mday;
        int hour = local_tm.tm_hour;
        int minute = local_tm.tm_min;
        int second = local_tm.tm_sec;

        // Convert to 12hr if requested
        bool isPM = false;
        if (!is24HourFormat) {
            if (hour == 0) {
                hour = 12;         // midnight => 12 AM
            }
            else if (hour == 12) {
                isPM = true;       // 12 PM
            }
            else if (hour > 12) {
                hour -= 12;
                isPM = true;
            }
        }

        std::ostringstream oss;
        bool first = true;  // helps us decide whether to insert spaces or separators

        // Year
        if (addYear) {
            if (!first) oss << " "; // or add a dash, etc.
            oss << colorYear << year << colorReset;
            first = false;
        }
        // Month
        if (addMonth) {
            if (!first) oss << "-";
            oss << colorMonth;
            if (month < 10) oss << "0";
            oss << month << colorReset;
            first = false;
        }
        // Day
        if (addDay) {
            if (!first) oss << "-";
            oss << colorDay;
            if (day < 10) oss << "0";
            oss << day << colorReset;
            first = false;
        }

        // Hour
        if (addHour) {
            if (!first) oss << " ";
            oss << colorHour;
            if (hour < 10) oss << "0";
            oss << hour << colorReset;
            first = false;
        }
        // Minute
        if (addMinute) {
            oss << ":";
            oss << colorMinute;
            if (minute < 10) oss << "0";
            oss << minute << colorReset;
        }
        // Second
        if (addSecond) {
            oss << ":";
            oss << colorSecond;
            if (second < 10) oss << "0";
            oss << second << colorReset;
        }
        // AM/PM if in 12hr mode
        if (!is24HourFormat && (addHour || addMinute || addSecond)) {
            oss << colorAMPM;
            oss << (isPM ? " PM" : " AM");
            oss << colorReset;
        }

        return oss.str();
    }

    template<typename T>
    std::optional<T> GetInput() {
        if constexpr (std::is_same_v<T, std::string>) {
            // For std::string, read the entire line:
            std::string line;
            if (!std::getline(std::cin, line)) {
                return std::nullopt;
            }
            return line;
        }
        else {
            // For non-string types, read a single token:
            T value;
            if (std::cin >> value) {
                // Discard leftover up to newline
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            else {
                // Clear and discard so subsequent I/O isn’t broken:
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return std::nullopt;
            }
        }
    }

    static void WaitForInput(const std::string& Message) {
        std::cout << Message;

#ifdef _WIN32
        _getch(); // or getch()
#else
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    }

    template<typename T>
    std::optional<T> GetMaskedInput(const std::string& maskCharacter = "*") {
        std::string input;

#ifdef _WIN32
        while (true) {
            int ch = _getch();
            // ENTER
            if (ch == 13) {
                std::cout << std::endl;
                break;
            }
            // BACKSPACE or DEL
            else if (ch == 8 || ch == 127) {
                if (!input.empty()) {
                    input.pop_back();
                    std::cout << "\b \b";
                }
            }
            // Normal printable character
            else if (ch >= 32 && ch <= 126) {
                input.push_back(static_cast<char>(ch));
                std::cout << maskCharacter;
            }
            // else ignore arrow keys, function keys, etc.
        }
#else
        // Linux/Unix approach
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        while (true) {
            int ch = getchar();
            // ENTER
            if (ch == '\n' || ch == '\r') {
                std::cout << std::endl;
                break;
            }
            // BACKSPACE or DEL
            else if (ch == 8 || ch == 127) {
                if (!input.empty()) {
                    input.pop_back();
                    std::cout << "\b \b";
                }
            }
            // Normal printable
            else if (ch >= 32 && ch <= 126) {
                input.push_back(static_cast<char>(ch));
                std::cout << maskCharacter;
            }
            // else ignore
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

        // If the user wants a string, return the entire line.
        if constexpr (std::is_same_v<T, std::string>) {
            return input;
        }
        else {
            // Otherwise, parse as a single token inside the collected string.
            std::istringstream iss(input);
            T value;
            if (iss >> value) {
                return value;
            }
            return std::nullopt;
        }
    }

    inline KeyResult PollKey()
    {
#ifdef _WIN32

        int ch1 = _getch();  // read one char
        // Check for arrow keys or special keys:
        if (ch1 == 224 || ch1 == 0) {
            // This indicates a special key was pressed. Need another read:
            int ch2 = _getch();
            switch (ch2) {
            case 72: return { Key::UpArrow,    0 }; // up
            case 80: return { Key::DownArrow,  0 }; // down
            case 75: return { Key::LeftArrow,  0 }; // left
            case 77: return { Key::RightArrow, 0 }; // right
            default: return { Key::Unknown,    0 };
            }
        }
        else {
            // Normal keys:
            if (ch1 == 13)  return { Key::Enter,     0 };
            if (ch1 == 27)  return { Key::Escape,    0 };
            if (ch1 == 8)   return { Key::Backspace, 0 };
            if (ch1 == 32)  return { Key::Space,     0 };
            // Otherwise treat as Key::Char
            return { Key::Char, static_cast<char>(ch1) };
        }

#else // Linux / macOS / etc.

        // Put terminal in raw mode so we can read key-by-key:
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);  // no canonical, no echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        // Read chars
        int ch1 = getchar();

        // If ch1 == 27, could be ESC or an arrow key (escape sequence)
        if (ch1 == 27) {
            // Peek next chars without blocking if they aren't available?
            // For simplicity, do a blocking read. If we get '[' -> arrow key sequence
            int ch2 = getchar();
            if (ch2 == '[') {
                int ch3 = getchar(); // final char of escape sequence
                switch (ch3) {
                case 'A': // up
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return { Key::UpArrow, 0 };
                case 'B': // down
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return { Key::DownArrow, 0 };
                case 'D': // left
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return { Key::LeftArrow, 0 };
                case 'C': // right
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return { Key::RightArrow, 0 };
                default:
                    // Some other escape sequence
                    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                    return { Key::Unknown, 0 };
                }
            }
            // If ch2 != '[', it's just ESC pressed
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return { Key::Escape, 0 };
        }
        else {
            // Normal key:
            if (ch1 == '\n' || ch1 == '\r') {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return { Key::Enter, 0 };
            }
            else if (ch1 == 127 || ch1 == 8) {
                // backspace
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return { Key::Backspace, 0 };
            }
            else if (ch1 == ' ') {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return { Key::Space, 0 };
            }
            // Otherwise treat as normal char
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            return { Key::Char, static_cast<char>(ch1) };
        }

#endif
    }

    inline bool KeyboardEvent(const std::string& desiredKey)
    {
        KeyResult kr = PollKey();

        // Compare result with desiredKey
        if (desiredKey.size() == 1 && kr.key == Key::Char) {
            // e.g. "a", "b", "c"
            return (kr.ch == desiredKey[0]);
        }
        // Check for special words
        if (desiredKey == "space" && kr.key == Key::Space)     return true;
        if (desiredKey == "enter" && kr.key == Key::Enter)     return true;
        if (desiredKey == "backspace" && kr.key == Key::Backspace) return true;
        if (desiredKey == "escape" && kr.key == Key::Escape)    return true;
        if (desiredKey == "up_arrow" && kr.key == Key::UpArrow)   return true;
        if (desiredKey == "down_arrow" && kr.key == Key::DownArrow) return true;
        if (desiredKey == "left_arrow" && kr.key == Key::LeftArrow) return true;
        if (desiredKey == "right_arrow" && kr.key == Key::RightArrow)return true;

        return false;
    }

    static void SetConsoleTitle(const std::string& title) {
#ifdef _WIN32
        SetConsoleTitleA(title.c_str());
#else
        std::cout << "\033]0;" << title << "\007" << std::flush;
#endif
    }

    inline int GetTerminalWidth()
    {
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }
        return 80; // fallback
#else
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
            return w.ws_col;
        }
        return 80; // fallback
#endif
    }

    inline void PrintCentered(const std::string& text)
    {
        int width = GetTerminalWidth();
        if (width <= 0) {
            std::cout << text << std::endl;
            return;
        }
        // Calculate how many spaces
        int textLen = static_cast<int>(text.size());
        if (textLen >= width) {
            // If text is longer than terminal width, just print as-is
            std::cout << text << std::endl;
            return;
        }
        int leftPadding = (width - textLen) / 2;
        for (int i = 0; i < leftPadding; i++) {
            std::cout << " ";
        }
        std::cout << text << std::endl;
    }

    inline void WordByWordEffect(const std::string& text, unsigned delayMs = 400)
    {
        std::istringstream iss(text);
        std::string word;
        while (iss >> word) {
            std::cout << word << " " << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
        // std::cout << std::endl; maybe
    }

    inline void TypewriterEffect(const std::string& Text,
        int MinDelayMilliseconds,
        int MaxDelayMilliseconds) {

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(MinDelayMilliseconds, MaxDelayMilliseconds);

        for (char c : Text) {
            int delay = dist(gen);
            std::cout << c << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
    }

    inline void PrintSpinner(int SpinDurationMs, int SpinSpeedMs) {
        const char* spinChars = "|/-\\";
        int spinIndex = 0;
        auto start = std::chrono::steady_clock::now();

        while (true) {
            // Print spinning char and flush.
            std::cout << "\r" << spinChars[spinIndex++] << std::flush;
            if (spinIndex == 4) {
                spinIndex = 0;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(SpinSpeedMs));
            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= SpinDurationMs) {
                break;
            }
        }

        // Clear the spinner character.
        std::cout << "\r " << std::endl;
    }

    std::string ProgressBar(int CurrentPercentage,
        int MaxPercentage,
        int BarWidth,
        const std::string& PrefixText,
        const std::string& SuffixText,
        const std::string& FillChar,
        const std::string& UnfilledChar,
        const std::string& FillColor,
        const std::string& UnfilledColor,
        const std::string& TextColor,
        const std::string& PrefixColor,
        const std::string& SuffixColor,
        const std::string& BracketColor,
        bool ShowPercentage,
        bool ShowBrackets,
        bool ResetColorOnCompletion)
    {
        // Clamp current progress
        if (CurrentPercentage < 0) {
            CurrentPercentage = 0;
        }
        if (CurrentPercentage > MaxPercentage) {
            CurrentPercentage = MaxPercentage;
        }

        // Calculate progress fraction, avoid divide-by-zero
        double progress = (MaxPercentage != 0)
            ? static_cast<double>(CurrentPercentage) / MaxPercentage
            : 0.0;
        int filledWidth = static_cast<int>(progress * BarWidth);
        int remainingWidth = BarWidth - filledWidth;

        std::string result;

        // Prefix
        if (!PrefixText.empty()) {
            result.append(PrefixColor);
            result.append(PrefixText);
        }

        // Optional brackets
        if (ShowBrackets) {
            result.append(BracketColor);
            result.push_back('[');
        }

        // Filled portion
        result.append(FillColor);
        for (int i = 0; i < filledWidth; i++) {
            result.append(FillChar);
        }

        // Unfilled portion
        result.append(UnfilledColor);
        for (int i = 0; i < remainingWidth; i++) {
            result.append(UnfilledChar);
        }

        // Close brackets
        if (ShowBrackets) {
            result.append(BracketColor);
            result.push_back(']');
        }

        // Show percentage
        if (ShowPercentage) {
            int percentageValue = static_cast<int>(progress * 100);
            result.append(TextColor);
            result.append(" ");
            result.append(std::to_string(percentageValue));
            result.push_back('%');
        }

        // Suffix
        if (!SuffixText.empty()) {
            result.append(" ");
            result.append(SuffixColor);
            result.append(SuffixText);
        }

        if (ResetColorOnCompletion) {
            result.append(Color::RESET);
        }

        return result;
    }

    std::string Spacing(int NumberOfSpaces) {
        std::string spacing;
        for (int i = 0; i < NumberOfSpaces; i++) {
            spacing.append("\n");
        }
        return spacing;
    }

    inline void PrintWarning(const std::string& msg) {
        // Often yellow for warnings
        std::cout << Color::LIGHT_YELLOW << "[WARNING] " << msg << Color::RESET << std::endl;
    }

    inline void PrintError(const std::string& msg) {
        // Often red for errors
        std::cout << Color::LIGHT_RED << "[ERROR] " << msg << Color::RESET << std::endl;
    }

    inline void PrintSuccess(const std::string& msg) {
        // Often green for success
        std::cout << Color::LIGHT_GREEN << "[SUCCESS] " << msg << Color::RESET << std::endl;
    }

    inline void PrintInfo(const std::string& msg) {
        // Often cyan for info
        std::cout << Color::LIGHT_CYAN << "[INFO] " << msg << Color::RESET << std::endl;
    }

    inline void RenderASCIIArt(const std::string& ascii, bool center = false)
    {
        std::istringstream iss(ascii);
        std::string line;
        int width = GetTerminalWidth();

        while (std::getline(iss, line)) {
            if (center) {
                // Center each line
                int leftPadding = (width - static_cast<int>(line.size())) / 2;
                if (leftPadding < 0) leftPadding = 0; // if line is longer than width
                std::cout << std::string(leftPadding, ' ');
            }
            std::cout << line << "\n";
        }
    }

    inline void ShowTooltip(const std::string& message,
        int boxWidth = 40,
        bool centerInTerminal = false,
        const std::string& color = "\033[93m")
    {
        const std::string& reset = Color::RESET;
        // If requested, we'll center the entire box in the terminal horizontally
        int termWidth = GetTerminalWidth();
        if (boxWidth <= 0) boxWidth = 1;
        if (termWidth < boxWidth) {
            // If terminal is smaller than our box, we'll just align left
            centerInTerminal = false;
        }

        // Create top/bottom border
        std::string border = "+" + std::string(boxWidth - 2, '-') + "+";

        // Decide left padding for the entire box
        int leftPad = 0;
        if (centerInTerminal) {
            leftPad = (termWidth - boxWidth) / 2;
            if (leftPad < 0) leftPad = 0;
        }

        // Print top border
        if (leftPad > 0) std::cout << std::string(leftPad, ' ');
        std::cout << color << border << reset << "\n";

        // Break the message into lines that fit within boxWidth-2
        // so we can handle multi-line text or text that is too wide.
        const int contentWidth = boxWidth - 2;
        std::istringstream iss(message);
        std::string word, currentLine;

        auto flushLine = [&](int pad) {
            // Print current line, centered horizontally within the box
            // if there's leftover space.
            int spaceRemaining = contentWidth - static_cast<int>(currentLine.size());
            if (spaceRemaining < 0) spaceRemaining = 0;
            // Build line with spaces
            std::string lineContent = currentLine + std::string(spaceRemaining, ' ');
            // Print the line
            if (pad > 0) std::cout << std::string(pad, ' ');
            std::cout << color << "|" << reset << lineContent << color << "|" << reset << "\n";
            currentLine.clear();
            };

        while (iss >> word) {
            // If adding this word would exceed contentWidth, flush current line first
            if ((currentLine.size() + 1 + word.size()) > (size_t)contentWidth) {
                flushLine(leftPad);
                currentLine = word;
            }
            else {
                if (!currentLine.empty()) currentLine += " ";
                currentLine += word;
            }
        }
        // Flush the last line if any
        if (!currentLine.empty()) {
            flushLine(leftPad);
        }

        // Print bottom border
        if (leftPad > 0) std::cout << std::string(leftPad, ' ');
        std::cout << color << border << reset << "\n";
    }

    inline void PrintDemo() {
        // Set console title
        SetConsoleTitle("CLIKit Demo");

        // Clear the screen (optional, platform-dependent)
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        // Render ASCII Art
        std::string asciiArt = R"(
  ____ _     ___ _  _____ _____ 
 / ___| |   |_ _| |/ /_ _|_   _|
| |   | |    | || ' / | |  | |  
| |___| |___ | || . \ | |  | |  
  \____|_____|___|_|\_\___| |_| :))";
        std::cout << Color::CYAN;
        RenderASCIIArt(asciiArt, true);
        std::cout << Color::RESET << "\n";

        // Print Info Message
        PrintInfo("Welcome to the CLIKit PrintDemo!");

        WaitForInput("Please press any key to start the demo..");

        // Print Timestamp
        std::cout << "\n\nCurrent Timestamp: "
            << GetTimestamp(true, true, true, true, true, true, true,
                Color::LIGHT_GREEN, Color::LIGHT_BLUE, Color::LIGHT_YELLOW,
                Color::LIGHT_PURPLE, Color::LIGHT_CYAN, Color::LIGHT_RED, "")
            << "\n\n";

        sleep(1000);

        // Demonstrate Colored Text
        std::cout << "Demonstrating Colors:\n";
        std::cout << Color::RED << "This is red text.\n";
        std::cout << Color::ORANGE << "This is orange text.\n";
        std::cout << Color::YELLOW << "This is yellow text.\n";
        std::cout << Color::GREEN << "This is green text.\n";
        std::cout << Color::BLUE << "This is blue text.\n";
        std::cout << Color::PURPLE << "This is purple text.\n";
        std::cout << Color::CYAN << "This is cyan text.\n";
        std::cout << Color::RESET << "\n";

        sleep(1000);

        // Demonstrate Typewriter Effect
        PrintInfo("Typewriter Effect:");
        TypewriterEffect("This text appears one character at a time...", 50, 150);
        std::cout << "\n\n";

        sleep(1000);

        // Demonstrate Word by Word Effect
        PrintInfo("Word by Word Effect:");
        WordByWordEffect("This text appears one word at a time...", 500);
        std::cout << "\n\n";

        sleep(1000);

        // Demonstrate Spinner
        PrintInfo("Loading with Spinner:");
        PrintSpinner(2000, 400); // Spin for 2 seconds with 100ms speed
        PrintSuccess("Loading complete!\n");

        sleep(1000);

        // Demonstrate Progress Bar
        PrintInfo("Progress Bar Demonstration:");
        const int total = 100;
        for (int i = 0; i <= total; ++i) {
            std::cout << "\r" << ProgressBar(
                i,
                total,
                50,
                "Progress:",
                "Processing",
                "=",
                "-",
                Color::GREEN,
                Color::GRAY,
                Color::WHITE,
                Color::LIGHT_GREEN,
                Color::WHITE,
                Color::LIGHT_BLUE,
                true,
                true,
                true
            ) << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::cout << "\n\n";

        sleep(1000);

        // Demonstrate Input Functions
        PrintInfo("Input Demonstration:");
        std::cout << "Please enter your name: ";
        auto name = GetInput<std::string>();
        if (name) {
            PrintSuccess("Hello, " + *name + "!");
        }
        else {
            PrintError("Failed to read your name.");
        }

        std::cout << "Please enter your age: ";
        auto age = GetInput<int>();
        if (age) {
            PrintSuccess("You are " + std::to_string(*age) + " years old.");
        }
        else {
            PrintError("Failed to read your age.");
        }

        // Demonstrate Masked Input (e.g., Password)
        std::cout << "Please enter your password: ";
        auto password = GetMaskedInput<std::string>();
        if (password) {
            PrintSuccess("Password received (hidden).");
        }
        else {
            PrintError("Failed to read your password.");
        }
        std::cout << "\n";

        // Demonstrate Keyboard Event
        PrintInfo("Keyboard Event Demonstration:");
        std::cout << "Press the 's' key to see a success message, or 'e' for an error message.\n";
        bool eventHandled = false;
        while (!eventHandled) {
            KeyResult kr = PollKey();
            if (kr.key == Key::Char) {
                if (kr.ch == 's' || kr.ch == 'S') {
                    PrintSuccess("You pressed 's'!");
                    eventHandled = true;
                }
                else if (kr.ch == 'e' || kr.ch == 'E') {
                    PrintError("You pressed 'e'!");
                    eventHandled = true;
                }
                else {
                    PrintWarning("Invalid key pressed. Press 's' or 'e'.");
                }
            }
        }
        std::cout << "\n";

        // Demonstrate ShowTooltip
        PrintInfo("Tooltip Demonstration:");
        ShowTooltip("This is a tooltip message to provide additional information.", 50, true, Color::LIGHT_CYAN);
        std::cout << "\n";

        // Demonstrate PrintCentered
        PrintInfo("Centered Text Demonstration:");
        PrintCentered("This text is centered in the terminal.");
        std::cout << "\n";

        // Demonstrate PrintWarning, PrintError, PrintSuccess, PrintInfo
        PrintWarning("This is a warning message.");
        PrintError("This is an error message.");
        PrintSuccess("This is a success message.");
        PrintInfo("This is an informational message.");
        std::cout << "\n";

        // Final Message
        PrintInfo("CLIKit PrintDemo completed successfully!");

        // Wait for user to press any key before exiting
        WaitForInput("Press any key to exit...");
    }
}