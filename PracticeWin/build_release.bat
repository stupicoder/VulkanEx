@echo off
REM This script automates the build process for the RELEASE configuration.

REM Create a build directory if it doesn't exist
if not exist build (
    mkdir build
)

REM Change to the build directory
cd build

REM Generate the Visual Studio solution using CMake
echo "--- Generating Visual Studio solution... ---"
cmake ..

REM Build the project in Release mode
echo "--- Building the project (Release)... ---"
cmake --build . --config Release

echo "--- Build complete. The executable is in the 'binaries' folder. ---"
pause
