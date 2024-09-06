#include <Windows.h>
#include "../include/starwar.h"

int APIENTRY WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd)
{
    Initialize();
    Launch();
    ClearUp();

    return 0;
}