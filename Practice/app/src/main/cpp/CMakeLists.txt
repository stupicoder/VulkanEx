# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("practice")

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(practice SHARED
        main.cpp
        AndroidOut.cpp
        VkRenderer.h
        VkRenderer.cpp
        VkUtil.h
)

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)
find_package(Vulkan REQUIRED)

target_compile_definitions(practice PRIVATE
        VK_USE_PLATFORM_ANDROID_KHR)

# Configure libraries CMake uses to link your target library.
target_link_libraries(practice
        # The game activity
        game-activity::game-activity

        android
        log
        Vulkan::Vulkan
)