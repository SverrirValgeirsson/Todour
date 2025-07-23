@echo off
REM Todour Test Suite Runner for Windows
REM This script builds and runs the complete test suite for the Todour application

setlocal enabledelayedexpansion

set SCRIPT_DIR=%~dp0
set PROJECT_ROOT=%SCRIPT_DIR%\..
set TEST_DIR=%SCRIPT_DIR%
set BUILD_DIR=%TEST_DIR%\build

REM Setup Qt environment if setup script exists
if exist "%SCRIPT_DIR%\setup_paths_local.bat" (
    echo Setting up Qt environment...
    call "%SCRIPT_DIR%\setup_paths_local.bat"
    echo.
)

echo Todour Test Suite Runner
echo ========================
echo Project root: %PROJECT_ROOT%
echo Test directory: %TEST_DIR%
echo Build directory: %BUILD_DIR%
echo.

REM Check for required tools
echo Checking for required tools...
where qmake >nul 2>&1
if errorlevel 1 (
    echo Error: qmake not found. Please install Qt development tools and add to PATH.
    exit /b 1
)

where mingw32-make >nul 2>&1
if errorlevel 1 (
    echo Error: mingw32-make not found. Please install MinGW and add to PATH.
    exit /b 1
)

echo All required tools found.
echo.

REM Clean previous build
echo Cleaning previous build...
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
mkdir "%BUILD_DIR%"

REM Build tests
echo Building test suite...
cd /d "%BUILD_DIR%"

REM Use simplified core tests that work around UI compilation issues
qmake "%TEST_DIR%\tests_simple.pro" CONFIG+=debug
if errorlevel 1 (
    echo Error: qmake failed.
    exit /b 1
)

mingw32-make
if errorlevel 1 (
    echo Error: Failed to build test suite.
    exit /b 1
)

echo Test suite built successfully.
echo.

REM Run tests
echo Running test suite...
echo =====================

REM Copy required Qt DLLs for test execution
if exist "debug" (
    copy /Y "%QT_BIN_PATH%\Qt5Core.dll" debug\ >nul 2>&1
    copy /Y "%QT_BIN_PATH%\Qt5Test.dll" debug\ >nul 2>&1
    copy /Y "%QT_BIN_PATH%\Qt5Gui.dll" debug\ >nul 2>&1
    if not exist "debug\platforms" mkdir debug\platforms
    copy /Y "%QT_BIN_PATH%\..\plugins\platforms\qminimal.dll" debug\platforms\ >nul 2>&1
)

REM Set minimal platform for headless testing
set QT_QPA_PLATFORM=minimal

if exist "debug\todour_core_tests.exe" (
    debug\todour_core_tests.exe
    set TEST_RESULT=!errorlevel!
) else if exist "todour_core_tests.exe" (
    todour_core_tests.exe
    set TEST_RESULT=!errorlevel!
) else (
    echo Error: Test executable not found.
    exit /b 1
)

echo.
echo Test suite completed with exit code: !TEST_RESULT!

if !TEST_RESULT! equ 0 (
    echo ✅ All tests passed!
) else (
    echo ❌ Some tests failed. See output above for details.
)

exit /b !TEST_RESULT!
