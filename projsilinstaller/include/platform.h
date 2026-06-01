#pragma once

// Platform detection and abstraction layer
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_LINUX 0
    #define PLATFORM_NAME "Windows"
#elif defined(__linux__)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_LINUX 1
    #define PLATFORM_NAME "Linux"
#elif defined(__APPLE__)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_LINUX 0
    #define PLATFORM_NAME "macOS"
#else
    #error "Unsupported platform"
#endif

// Platform-specific includes
#if PLATFORM_WINDOWS
    #include <Windows.h>
    #include <conio.h>
    #define EXPORT __declspec(dllexport)
#else
    #include <unistd.h>
    #include <dlfcn.h>
    #define EXPORT
#endif

// Compatibility macros
#if PLATFORM_WINDOWS
    #define NEWLINE "\r\n"
    #define PATH_SEPARATOR "\\"
#else
    #define NEWLINE "\n"
    #define PATH_SEPARATOR "/"
#endif
