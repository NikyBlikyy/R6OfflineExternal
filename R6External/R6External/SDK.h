#pragma once
#include "Includes.h"

namespace SDK
{
	//Variables and names
	extern HWND           hwnd;
	extern DWORD          procID;
	extern HANDLE         hProc;
	extern uintptr_t      moduleBase;
	static const char*    windowName    = "Rainbow Six";
	static const char*    moduleName    = "RainbowSix.exe";
	
	//Functions
	uintptr_t GetModuleBaseAddress(const char* modName, DWORD procID);

	bool Initialize();

	void Menu();
}

namespace config
{
	inline bool InfiniteHealth = false;
	inline bool Speed = false;
	inline bool NoRecoil = false;
	inline bool NoClip = false;
	inline bool ESP = false;
	inline bool Aimbot = false;
	inline bool ESPConfig = false;
	inline bool AimbotConfig = false;
	inline bool MiscConfig = false;
	inline bool Box = false;
	inline bool headDot = false;
	inline bool Rainbow = false; 
	inline bool Snaplines = false;
	inline bool Healthbar = false;
	inline bool FOV = false;
	inline bool showFOV = false;

	inline float NoRecoilSlider = NULL;
	inline float NoSpreadSlider = NULL;
	inline float SpeedSlider = NULL;
	inline float fovSlider = NULL;
}