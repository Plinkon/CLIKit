
# 🚀 CLIKit

**CLIKit** is a lightweight and versatile library for creating feature-rich **command-line interface (CLI)** applications in c++. Whether you're building a simple script or a complex tool, CLIKit offers a suite of utilities for text formatting, interactive effects, and user input handling, making CLI development seamless and enjoyable.

---

## 🌟 Features

- 🎨 **Colorized Output**: Make your CLI more vibrant with customizable colored messages.
- 🖋️ **Input Handling**: Supports standard input, masked input (e.g., for passwords), and key-specific input detection.
- ✨ **Interactive Effects**: Includes dynamic typewriter effects, word-by-word animations, and spinners.
- 📊 **Progress Bars**: Generate sleek, customizable progress bars for real-time feedback.
- 🖥️ **Terminal Utilities**: Easily set terminal titles, retrieve terminal dimensions, and more.
- 🛠️ **...and Much More**: Packed with tools to simplify and enhance your CLI applications.

---

## 🚀 Installation

Getting started with CLIKit is easy. Follow these steps:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Plinkon/CLIKit.git
   ``` 

2.  **Include the header file**: Add `CLIKit.h` *(inside "src/")* to your project directory.
    
3.  **Compile your program**: Use any modern C++ compiler
	```bash    
    g++ -std=c++17 your_program_name.cpp -o your_program_name
    ```
    

----------

## 🛠️ Quick Start

Here’s a simple example showcasing how to create a progress bar with CLIKit:

```cpp
#include "CLIKit.h"

int main() {
    for (int i = 0; i <= 100; ++i) {
        std::cout << "\r" << CLIKit::ProgressBar( // create a progress bar
            i, 100, 50, "Progress:", "Done", "=", "-",
            Color::GREEN, Color::GRAY,
            Color::WHITE, Color::LIGHT_GREEN,
            Color::WHITE, Color::LIGHT_BLUE,
            true, true, true
        ) << std::flush;
        sleep(100); // sleep for 100 milliseconds, built-in function
    }
    std::cout << std::endl;
    return 0;
}
```

💡 **Tip**: You can also refer to the `PrintDemo()` function in `CLIKit.h` to see how all the functions are used.

----------

## 📖 Documentation

Dive into the full documentation to explore all available features, utilities, and examples.

👉 [Read the Documentation](./DOCS.md)

----------

## 🤝 Contributing

**CLIKit** welcomes contributions from the community! Here’s how you can help:

1.  **Report Bugs**: Found an issue? Open an [issue](https://github.com/Plinkon/CLIKit/issues).
2.  **Propose Features**: Share your ideas for new features.
3.  **Submit Code**: Fork the repository, make your changes, and submit a pull request.

### Steps to Contribute:

1.  Fork the repository.
2.  Create a new branch for your feature/bug fix:
	```bash  
    git checkout -b feature-or-bugfix-name
    ```
    
3.  Commit your changes and push to your fork:
    
    ```bash
    git push origin feature-or-bugfix-name 
    ```
    
4.  Open a pull request with a clear description of your changes.

----------

## 📜 License

CLIKit is licensed under the **MIT License**. See the [LICENSE](./LICENSE) file for details.

----------

## ❤️ Support

If you find CLIKit helpful, consider giving it a ⭐ on GitHub to show your support. Thank you for helping CLIKit grow!

----------

### ✨ Made with CLIKit

Feel free to share the amazing projects you’ve built with CLIKit! We’d love to showcase your work in the **Showcase**.
