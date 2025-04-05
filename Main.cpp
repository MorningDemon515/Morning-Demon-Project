#ifndef UNICODE
#define UNICODE
#endif 

#include "Main.h"

extern HWND hwnd;
bool run = true;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InitWindow();

void InitGraphics(int windowed);
void DrawScene();
void CleanGraphics();

int main()
{
    InitWindow();

    InitGraphics(3);

    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    while (run)
    {
        if (PeekMessage(&msg,0,0,0,PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                run = false;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            DrawScene();
        }
    }

    CleanGraphics();
    return 0;
}

void ErrorMessage(const char* message)
{
    MessageBoxA(hwnd, message, "Engine Error!", MB_OK);
}
