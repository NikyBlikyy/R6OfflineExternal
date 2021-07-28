#include "SDK.h"
#include "Hacks.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include "Overlay.h"

HWND SDK::hwnd = NULL;
HANDLE SDK::hProc = NULL;
uintptr_t SDK::moduleBase = NULL;
DWORD SDK::procID = NULL;

constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

uintptr_t SDK::GetModuleBaseAddress(const char* modName, DWORD procID) //basically loops through all the modules of processes until it finds the one that matches our string.
{
	uintptr_t modBaseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_stricmp(modEntry.szModule, modName))
				{
					modBaseAddress = (uintptr_t)modEntry.modBaseAddr;
				}

			} while (Module32Next(hSnap, &modEntry));
		}
	}

	CloseHandle(hSnap);
	return modBaseAddress;
}

bool SDK::Initialize()
{
	SDK::hwnd = FindWindowA(NULL, SDK::windowName);
	if (SDK::hwnd == NULL)
	{
		MessageBoxA(NULL, "Rainbow Six Siege not open!", "Error", MB_OK);
		return false;
	}

	GetWindowThreadProcessId(SDK::hwnd, &SDK::procID);
	if (SDK::procID == NULL)
	{
		MessageBoxA(NULL, "Unable to obtain process ID!", "Error", MB_OK);
		return false;
	}

	SDK::hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, SDK::procID);
	if (SDK::hProc == NULL || SDK::hProc == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "Unable to open a handle!", "Error", MB_OK);
		return false;
	}

	SDK::moduleBase = SDK::GetModuleBaseAddress(SDK::moduleName, SDK::procID);
	if (SDK::moduleBase == NULL)
	{
		MessageBoxA(NULL, "Unable to get module base address!", "Error", MB_OK);
		return false;
	}

	// 0x7ff7116d0000 <- Module we want
	/*printf("Process ID: %d\n", SDK::procID);
	printf("Handle: 0x%p\n", SDK::hProc);
	printf("Module: 0x%p\n\n", SDK::moduleBase);*/

	
	AllocConsole();
	FILE* fIn;
	FILE* fOut;
	freopen_s(&fIn, "conin$", "r", stdin);
	freopen_s(&fOut, "conout$", "w", stdout);
	freopen_s(&fOut, "conout$", "w", stderr);

	Decrypt::DecryptProfileManager();
	Decrypt::DecryptLocalPlayer();
	Decrypt::DecryptEntityList();
	Decrypt::DecryptEntityCount();

	return true;
}

static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;

bool initImgui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

	return true;
}

bool init = false;

bool show = false;

void SDK::Menu()
{
	if (!init)
	{
		initImgui();
		init = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (show)
	{
		SetWindowLongA(hWnd, GWL_EXSTYLE, (WS_EX_TRANSPARENT | WS_EX_TOPMOST));
	}
	else
	{
		SetWindowLongA(hWnd, GWL_EXSTYLE, (WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST));
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		show = !show;
	}

	int currentState = Decrypt::GetCurrentState();

	if (show)
	{
		ImGui::SetNextWindowSize(ImVec2(280.f, 200.f));
		ImGui::Begin("R6 by - Nik", nullptr, windowFlags | ImGuiWindowFlags_AlwaysAutoResize);
		if (ImGui::BeginTabBar("TabBar", ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip)) 
		{
			if (ImGui::BeginTabItem("Aimbot"))
			{
				ImGui::Checkbox("Aimbot", &config::Aimbot);
				if (config::Aimbot)
				{
					ImGui::Checkbox("FOV", &config::FOV);
					
					if (config::FOV)
					{
						ImGui::SliderFloat("FOV", &config::fovSlider, 1.0f, 300.f);
					}

			     	ImGui::Checkbox("Show FOV", &config::showFOV);
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("ESP"))
			{
				ImGui::Checkbox("ESP", &config::ESP);
				if (config::ESP)
				{
					ImGui::Checkbox("Box", &config::Box);
					ImGui::Checkbox("Head Dot", &config::headDot);
					ImGui::Checkbox("Snaplines", &config::Snaplines);
					ImGui::Checkbox("Healthbar", &config::Healthbar);
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc"))
			{
				ImGui::Checkbox("NsNr", &config::NoRecoil);
				if (config::NoRecoil)
				{
					ImGui::SliderFloat(("Recoil"), &config::NoRecoilSlider, 0.001f, 1.f); //If its a flat 0 it will glitch out.
					ImGui::SliderFloat(("Spread"), &config::NoSpreadSlider, 0.0f, 1.f);
				}
				ImGui::Checkbox("Speed", &config::Speed);
				if (config::Speed)
				{
					ImGui::SliderFloat(("Speed"), &config::SpeedSlider, 3.5, 15.f);
				}
				ImGui::Checkbox("Noclip", &config::NoClip);
				ImGui::Checkbox("Infinite Health", &config::InfiniteHealth);
				ImGui::Checkbox("LGBTQ Mode", &config::Rainbow);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());


	//if (currentState == 3) //Attacking phase in the game, we don't want to write to addresses when we're not actually playing.
	//{
		if (config::InfiniteHealth)
		{
			Hacks::ReadAndWriteHealth(99999);
		}

		if (config::NoClip)
		{
			Hacks::NoClip(-1); //Noclip is -1
		}

		if (config::ESP)
		{
			Hacks::ESP();
		}

		if (config::Aimbot)
		{
			Hacks::HeadAimbot();
		}

		/*if (!config::InfiniteHealth)
		{
			Hacks::ReadAndWriteHealth(100);
		}*/

		if (!config::NoClip)
		{
			Hacks::NoClip(0.3f); //Noclip is -1
		}

		if (!config::NoRecoil)

		{
			Hacks::NoRecoil(0.5f, 0.5f);
		}

		if (!config::Speed)
		{
			Hacks::Speed(3.5f);
		}

		if (config::NoRecoil)
		{
			Hacks::NoRecoil(config::NoSpreadSlider, config::NoRecoilSlider);
		}

		if (config::Speed)
		{
			Hacks::Speed(config::SpeedSlider);
		}
//	}
}