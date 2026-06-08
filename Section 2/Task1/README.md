# FileWrapper Module

## 1. Architectural Overview

This module implements a simple RAII-based file handling wrapper in C++.

The FileWrapper class encapsulates a FILE* handle and ensures:
- Safe opening of files in the constructor
- Automatic closing of files in the destructor (RAII)
- Reading full file content using buffered reads (fgets)
- Writing content using fputs

The goal is to prevent resource leaks and ensure files are always properly closed even in case of errors or exceptions.

---

## 2. Assumptions and Trade-offs

- Uses C-style FILE* instead of C++ streams for simplicity and direct control
- Assumes text-based file input/output only
- No advanced buffering or performance optimizations
- Trade-off: simplicity and predictability over abstraction and flexibility

---

## 3. Verification Steps

1. Build project:
make
2. Run program:
./RAII.exe
Enter input.txt (asks for which file you want to transfer is content into output.txt)
3. Verify expected behavior:
- FileWrapper: input file is correctly read and written to output file
- ThreadPool: multiple tasks execute concurrently across worker threads
- No crashes or resource leaks observed

---

## 4. Build Instructions

Build:
make
Clean: 
make clean
Compiler flags:
-std=c++20 -Wall -Wextra -Werror -O2