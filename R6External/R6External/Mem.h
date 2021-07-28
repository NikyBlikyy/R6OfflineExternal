#pragma once
#include "Includes.h"
#include "SDK.h"

namespace mem
{
	//Read & Write Memory.
	template<typename T>
	T Read(uintptr_t address)
	{
		T buffer;
		ReadProcessMemory(SDK::hProc, (LPVOID)address, &buffer, sizeof(buffer), NULL);
		return buffer;
	}

	template<typename T>
	T ReadMultiLevelPointer(uintptr_t address, std::vector<unsigned int> Offsets)
	{
		for (unsigned int i = 0; i < Offsets.size(); i++)
		{
			ReadProcessMemory(SDK::hProc, (LPVOID*)address, &address, sizeof(address), nullptr);
			address += Offsets[i];
		}
		return address;
	}

	template<typename T>
	void Write(uintptr_t address, T buffer)
	{
		WriteProcessMemory(SDK::hProc, (LPVOID)address, &buffer, sizeof(T), NULL);
	}
}


