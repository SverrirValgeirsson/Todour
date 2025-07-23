@echo off
REM Local environment setup for Todour development
REM Customize these paths for your specific Qt installation

REM Qt Installation paths - Adjust these to match your installation
set QTVERSION=5.15.2
set QTDIR=C:\Qt
set QT_BIN_PATH=%QTDIR%\%QTVERSION%\mingw81_64\bin
set MINGW_PATH=%QTDIR%\Tools\mingw810_64\bin

REM Alternative common Qt installation paths (uncomment if needed)
REM set QT_BIN_PATH=C:\Qt\Qt5.15.2\5.15.2\mingw81_64\bin
REM set MINGW_PATH=C:\Qt\Qt5.15.2\Tools\mingw810_64\bin

REM Add Qt and MinGW to PATH
set PATH=%MINGW_PATH%;%QT_BIN_PATH%;%PATH%

echo Qt Development Environment Setup
echo =================================
echo Qt Version: %QTVERSION%
echo Qt Directory: %QTDIR%
echo Qt Bin Path: %QT_BIN_PATH%
echo MinGW Path: %MINGW_PATH%
echo.

REM Verify tools are available
echo Checking for required tools...
where qmake >nul 2>&1
if errorlevel 1 (
    echo WARNING: qmake not found. Please check Qt installation path.
    echo Current QT_BIN_PATH: %QT_BIN_PATH%
) else (
    echo ✓ qmake found
)

where mingw32-make >nul 2>&1
if errorlevel 1 (
    echo WARNING: mingw32-make not found. Please check MinGW installation path.
    echo Current MINGW_PATH: %MINGW_PATH%
) else (
    echo ✓ mingw32-make found
)

echo.
echo Environment setup complete.
echo You can now run: run_tests.bat