@echo off
:: build.bat
::./build.bat source [--mt] [--bmp] -> to generate the obj files in bin_int and exe file in bin
::./build.bat clear -> to delete bin_int and bin
::./build.bat clear_bin -> to delete bin
::./build.bat clear_bin_int -> to delete bin_int
::./build.bat info -> debugging information
::./build.bat render -> to render the bin/EXE

:: Checking dependencies
where clang 2>NUL 1>&2
if %ERRORLEVEL% neq 0 (
    echo clang is not installed
    exit /b 1
)

clang --version 2>NUL 1>&2
if %ERRORLEVEL% neq 0 (
    echo Add path of clang.exe to environment variable PATH
    exit /b 1
)

:: Building or cleaning test files
if "%1"=="source" (
    setlocal ENABLEDELAYEDEXPANSION

    make -f build.mak clear_bin CODE_DIRS="source" BIN_INT_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%"
    
    set "DEFINES="
    if "%2"=="--mt" set "DEFINES=-DMULTITHREADING"
    if "%2"=="--bmp" set "DEFINES=-DBMP"
    if "%3"=="--mt" set "DEFINES=!DEFINES! -DMULTITHREADING"
    if "%3"=="--bmp" set "DEFINES=!DEFINES! -DBMP"
    
    make -f build.mak all CODE_DIRS="source" BIN_INT_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%" MACROS="!DEFINES!"
    exit /b %errorlevel%
)


if "%1"=="info" (
    make -f build.mak info CODE_DIRS="source" BIN_INT_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%" MACROS=""
    exit /b 0
)

if "%1"=="clear" (
    make -f build.mak clear CODE_DIRS="source" BIN_INT_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%" MACROS=""
    exit /b 0
)

if "%1"=="clear_bin" (
    make -f build.mak clear_bin CODE_DIRS="source" BIN_INT_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%" MACROS=""
    exit /b 0
)

if "%1"=="clear_bin_int" (
    make -f build.mak clear_bin_int CODE_DIRS="source" BIN_INT_DIR="win32" ARCH="%PROCESSOR_ARCHITECTURE%" MACROS=""
    exit /b 0
)

:: Running the executable if requested
if "%1"=="render" (
    if exist .\bin\EXE.exe (
        .\bin\EXE.exe %2
        exit /b 0
    ) else (
        echo EXE.exe not found in bin directory
        exit /b 1
    )
)

:: If no valid command was provided
echo Invalid command. Available commands:
echo   source [--mt] [--bmp]
echo   clear
echo   clear_bin
echo   clear_bin_int
echo   info
echo   render
exit /b 1