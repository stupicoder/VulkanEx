#define UNICODE
#include <windows.h>
#include <iostream>
#include <chrono>
#include <string>

#ifdef _DEBUG
void CreateConsole()
{
    if (AllocConsole())
    {
        FILE* pCout;
        freopen_s(&pCout, "CONOUT$", "w", stdout);
        SetConsoleTitle(L"Debug Console");
    }
}
#endif

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    CreateConsole();
#endif

    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Vulkan Window",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // FPS counter variables
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    double elapsedTime = 0.0;

    // Main game loop
    bool isRunning = true;
    while (isRunning)
    {
        MSG msg = { };
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                isRunning = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // --- FPS Calculation ---
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> delta = currentTime - lastTime;
        elapsedTime += delta.count();
        frameCount++;

        if (elapsedTime >= 1000.0)
        {
            double fps = frameCount / (elapsedTime / 1000.0);
            double ms = elapsedTime / frameCount;
            
            std::wstring title = L"Vulkan Window | FPS: " + std::to_wstring((int)fps) + L" | ms: " + std::to_wstring(ms);
            SetWindowText(hwnd, title.c_str());

            frameCount = 0;
            elapsedTime = 0.0;
        }
        lastTime = currentTime;

        // --- Game logic would go here ---
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
