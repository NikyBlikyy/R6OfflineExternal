#pragma once
#include "Includes.h"
#include "Defs.h"
#include "Math.h"


namespace Decrypt
{
	std::uintptr_t DecryptProfileManager();
	std::uintptr_t DecryptGameManager();
	std::uintptr_t DecryptRoundManager();
	std::uintptr_t DecryptFovManager();
	std::uintptr_t GetCurrentState();
	std::uintptr_t GetCamera();
	Vector3        GetViewRight();
	Vector3        GetViewUp();
	Vector3        GetViewForward();
	Vector3        GetViewTranslation();
	float	       GetFovX();
	float	       GetFovY();
	auto           DecryptWeapon(uintptr_t localPawn)->uintptr_t;
	auto	       DecryptEntityList()->uintptr_t;
	auto           DecryptEntityCount()->uint32_t;
    uint32_t       DecryptSpread(float value);
    uint32_t       DecryptRecoilY3(float value);
    uint32_t       DecryptRecoilY2(float value);
	Vector4        DecryptAimbotAngles(Vector4 ang);
	std::uintptr_t DecryptLocalPlayer();
	std::uintptr_t DecryptPawn();
	std::uintptr_t DecryptEntityPawn(uintptr_t entity);
	std::uintptr_t DecryptLocalComponents();
}

namespace Encrypt //We need to encrypt some stuff back because the game can only read encrypted values on some things.
{
	Vector4 EncryptAimbotAngles(Vector4 ang);
}

