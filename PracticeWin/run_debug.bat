@echo off
REM This script automates the build process for the DEBUG configuration and runs the executable.

REM Create a build directory if it doesn't exist
if not exist build (
    mkdir build
)

REM Generate the Visual Studio solution in the 'build' directory
echo "--- Generating Visual Studio solution... ---"
cmake -S . -B build

REM Build the project in Debug mode from the 'build' directory
echo "--- Building the project (Debug)... ---"
cmake --build build --config Debug

REM Check if the build was successful before running
if %errorlevel% neq 0 (
    echo "--- Build failed. ---"
    pause
    exit /b
)

echo "--- Running the application... ---"
call .\binaries\Debug\VulkanPractice.exe

pause
