# Belisima Scripting Language

## Overview

Belisima is a simple scripting language implemented in C++. It was developed as an experiment in language design and implementation. The language is dynamically-typed and supports imperative programming paradigms.

Key features include:
*   Basic data types: Numbers (arbitrary precision via GMP), Strings, Booleans, Arrays.
*   Control flow: `if`/`else` statements.
*   Functions and a basic concept of Classes (though the extent of OOP features may be limited).
*   Variable handling through environments and symbols.
*   Lexing, parsing, and evaluation of script files.
*   Error handling via a "Panic" mechanism.

## Build Instructions

The project is designed to be built using Visual Studio.

1.  **Open the Solution:**
    *   Open Visual Studio.
    *   Select "Open a project or solution."
    *   Navigate to the project directory and open `Belisima.sln`.

2.  **Build the Project:**
    *   Once the solution is loaded, select a build configuration (e.g., "Debug" or "Release").
    *   From the main menu, choose "Build" > "Build Solution" (or press `Ctrl+Shift+B`).
    *   The executable, `Belisima.exe`, will be created in the respective configuration directory (e.g., `Debug/` or `Release/`).

3.  **GMP Dependency:**
    *   The project relies on the GNU Multiple Precision Arithmetic Library (GMP) for handling large numbers.
    *   The necessary GMP header files (`gmp.h`) and library files (`gmp.lib`, `gmp.dll`) appear to be pre-compiled and are included in the `gmp-dynamic` directory.
    *   The Visual Studio project file (`Belisima.vcxproj`) is configured to look for includes in `gmp-dynamic/` and link against `gmp-dynamic/gmp.lib` (specifically noted in the Debug configuration). Users should ensure this path is correctly referenced if they move the project or encounter build issues related to GMP.

## Running the Interpreter

The compiled `Belisima.exe` can be used to execute Belisima script files.

1.  **Compile the Project:** Ensure you have successfully built the project as described above.
2.  **Run from Command Line:**
    *   Open a command prompt or terminal.
    *   Navigate to the directory containing `Belisima.exe` (e.g., `Debug/` or `Release/`).
    *   To execute a script, provide the script file path as a command-line argument. For example:
        ```bash
        Belisima.exe ..\..\test1.txt
        ```
        (Assuming `test1.txt` is in the project root, and you are running from the `Debug` or `Release` subfolder).
        Or, if `Belisima.exe` is in your PATH or you are in the same directory as the executable and script:
        ```bash
        Belisima.exe test1.txt
        ```
    *   The `Main.cpp` file shows that the interpreter initializes a `Lexer` with the provided filename.

## Dependencies

*   **GNU Multiple Precision Arithmetic Library (GMP):**
    *   Required for arbitrary-precision number support.
    *   The necessary files (headers, `.lib`, `.dll`) are included in the `gmp-dynamic` directory. The project is set up to use these directly.

## Testing (Briefly)

The project includes a few example script files that can be used for basic testing:

*   `test1.txt`
*   `test2.txt`
*   `ArrayTest.txt`

These files demonstrate some of the language's syntax and features.

A more formal and comprehensive testing framework would be a valuable area for future improvement to ensure the interpreter's stability and correctness.
---
*This README was enhanced by an AI assistant.*
