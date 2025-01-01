# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("vulkanex")

####################################################################################################
# shaderc 정의
####################################################################################################
set(CMAKE_SHADERC ${CMAKE_ANDROID_NDK}/sources/third_party/shaderc)
add_library(shaderc INTERFACE)

target_include_directories(shaderc INTERFACE
        ${CMAKE_SHADERC}/include)

target_link_libraries(shaderc INTERFACE
        ${CMAKE_SHADERC}/libs/system/${CMAKE_ANDROID_ARCH_ABI}/libshaderc.a)

####################################################################################################
# practicevulkan 정의
####################################################################################################
# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(vulkanex SHARED
        VkRenderer.h
        VkRenderer.cpp
        VkUtil.h
        main.cpp
        AndroidOut.cpp
)

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)
find_package(Vulkan REQUIRED)

target_compile_definitions(vulkanex PRIVATE VK_USE_PLATFORM_ANDROID_KHR)

# Configure libraries CMake uses to link your target library.
target_link_libraries(vulkanex
        # The game activity
        game-activity::game-activity

        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        Vulkan::Vulkan
        shaderc
        log)

####################################################################################################
# shaderctest 정의
####################################################################################################
find_package(googletest REQUIRED CONFIG)
find_package(junit-gtest REQUIRED CONFIG)

add_library(shaderctest SHARED ShadercTest.cpp)

target_link_libraries(shaderctest PRIVATE
        googletest::gtest
        junit-gtest::junit-gtest
        shaderc)