Assembler Project

Overview

This project is an assembler designed for a specific assembly language. It includes the implementation of essential components such as macro handling, symbol tables, and multiple passes to translate assembly code into machine code.


Folder Structure

Root Directory: Contains project metadata and main source files.

assembler:

.c and .h files: Core source code and headers for the assembler.

Debug: Contains debug builds and related metadata.

Release: Contains release builds and associated files.

output: Example outputs demonstrating the assembler's functionality.

.settings: Eclipse-specific project settings.

test: Placeholder or example files for assembler tests.

Read me.pdf: Provides additional documentation or instructions.


Key Files

Source Code:

assembler.c & assembler.h: Main assembler logic.

macro.c & macro.h: Macro processing.

symbolTable.c & symbolTable.h: Symbol table management.

decoding.c & decoding.h: Instruction decoding.

firstPass.c, secondPass.c: Handles the assembler's two-pass process.

utils.c & utils.h: Utility functions.

Debug/Release:

Build files (makefile, *.o, *.d): For debugging and deployment.
Output Examples:

Files showcasing assembler outputs with and without entries or externals (*.ob, *.ent, *.ext, *.am).

Usage

Build:
Use the makefile in the Debug or Release directory to build the project.

Run:
Execute the assembler binary on the test files provided.

Test:
Use the provided examples in the output folder to validate functionality.

Dependencies
Compiler: Ensure GCC or a compatible compiler is installed.
