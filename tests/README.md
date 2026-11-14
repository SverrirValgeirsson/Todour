# Todour Test Suite Documentation

## Overview

This comprehensive test suite for the Todour application provides automated testing for core functionality. The test suite is built using the Qt Test framework and focuses on unit testing the core `todotxt` class.

## Test Structure

### Current Test Implementation

The test suite currently consists of:

1. **Core Unit Tests** (`test_todotxt.cpp`, `test_todotxt.h`)
   - Comprehensive testing of the `todotxt` class
   - 49 individual test methods covering all major functionality
   - Focus on core todo.txt operations, file handling, and advanced features

2. **Simplified Test Runner** (`test_main_simple.cpp`)
   - Lightweight test execution without UI dependencies
   - Isolated from GUI components for reliable CI/CD integration

### Test Categories

#### TestTodoTxt
Tests the core `todotxt` class functionality with 49 comprehensive tests:

**Basic Operations:**
- File parsing and writing
- Todo manipulation (add, edit, remove, complete)
- Priority and date handling
- Project and context extraction

**Advanced Features:**
- **Relative Time Support**: `t:+4d`, `due:+1w`, `rec:+1m` formats
- **Repeat Functionality**: Daily, weekly, monthly, yearly, and business day repeats
- **Threshold Dates**: Future task hiding and filtering
- **Mixed Date Formats**: Relative and absolute dates in the same task

**File Operations:**
- Undo/redo functionality
- Archive operations
- File watching and refresh
- URL extraction

**Edge Cases:**
- Corrupted files and malformed data
- Special characters and Unicode
- Large datasets (1000+ tasks)
- Empty files and boundary conditions

### Recent Additions

The test suite was recently enhanced with 9 new tests covering:
- Relative threshold dates (`t:+4d`)
- Relative due dates (`due:+1w`)
- Daily, weekly, monthly, yearly repeat patterns
- Business day repeats
- Mixed relative and absolute date formats

## Running the Tests

### Prerequisites

- Qt 5.15.2 or later with development tools
- A compatible C++ compiler (MinGW or MSVC on Windows, GCC on Linux, or Clang on macOS)
- Make, MinGW32-make, or NMake as appropriate for the compiler

### Quick Start

#### Windows
```cmd
cd tests
run_tests.bat
```

#### Linux/macOS
```bash
cd tests
qmake tests_simple.pro CONFIG+=release
make
QT_QPA_PLATFORM=offscreen ./todour_core_tests
```

Some macOS SDKs no longer include the AGL framework required by the Qt 5.15.2
qmake defaults. With those SDKs, use
`qmake tests_simple.pro CONFIG+=release -after 'QMAKE_LIBS_OPENGL = -framework OpenGL'`
instead.

### Quick Start (Automated)

On Windows, use the provided batch files:

1. **Set up Qt paths (first time only):**
   ```cmd
   setup_paths_local.bat
   ```

2. **Run all tests:**
   ```cmd
   run_tests.bat
   ```

The automated scripts handle building and running the simplified test suite.

### Manual Build and Run

1. **Build the test suite:**
   ```cmd
   cd tests
   qmake tests_simple.pro CONFIG+=debug
   mingw32-make
   ```

2. **Run the tests:**
   ```cmd
   .\debug\todour_core_tests.exe
   ```

### Configuration

Each test uses its own temporary directory and settings, so it won't interfere with your actual Todour data or depend on earlier tests.

## Test Data

Test data is generated dynamically within the test methods for maximum reliability and isolation. The tests create temporary files and clean up after themselves.


GitHub Actions builds the application and runs this suite on Linux, Windows,
and macOS using the offscreen Qt platform.

## License

This test suite is part of the Todour project and follows the same license terms.
