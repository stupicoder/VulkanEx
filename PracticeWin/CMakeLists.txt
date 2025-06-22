cmake_minimum_required(VERSION 3.10)

project(VulkanPractice)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Set output directories ---
# Set the path for runtime executables (.exe) and libraries (.dll) to the 'binaries' folder.
# ${CMAKE_SOURCE_DIR} points to the project root folder (where CMakeLists.txt is located).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries)

# Find Vulkan SDK
find_package(Vulkan REQUIRED)

# Automatically find all .cpp files in the 'sources' directory and its subdirectories
file(GLOB_RECURSE SOURCE_FILES "sources/*.cpp")

# Create a Windows-specific executable (no console window)
add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

# Add the 'sources' directory to the include path
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Link Vulkan libraries
if (Vulkan_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})
endif()
