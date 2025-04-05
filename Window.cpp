#ifndef UNICODE
#define UNICODE
#endif 

#include "Main.h"

const wchar_t CLASS_NAME[] = L"WindowClass";
HWND hwnd;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);

        DestroyWindow(hwnd);
        UnregisterClass(CLASS_NAME, GetModuleHandleW(NULL));
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void InitWindow()
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L" ",
        WS_OVERLAPPEDWINDOW,

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,

        NULL,
        NULL,
        GetModuleHandleW(NULL),
        NULL
    );

    if (!hwnd)
    {
        MessageBox(NULL, L"Cann't Create Window!", L"Engine Error!", MB_OK);
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
}
