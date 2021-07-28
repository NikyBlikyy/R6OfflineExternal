#include <iostream>
#include <thread>
#include "Includes.h"
#include "SDK.h"
#include "mem.h"
#include "Decrypt.h"
#include "Hacks.h"
#include "Overlay.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

HWND hWnd = NULL;
IDirect3D9* pD3D = NULL;
IDirect3DDevice9* pDevice = NULL;
ID3DXFont* pFont1 = NULL;
ID3DXLine* pLine = NULL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
   if (SDK::Initialize()) 
   {
      SetupWindow();
      OverlayThread();
   }
}
