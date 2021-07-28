#pragma once
#include "Includes.h"
#include "Math.h"

namespace Hacks
{
	bool         ReadAndWriteHealth(int value);
	bool         Speed(float value);
	bool         NoRecoil(float Spread, float Recoil);
	bool         NoClip(float value);
	Vector3      GetLocalPosition();
	int          ReadEntityHealth(uintptr_t entity);
	void         ReadEntityName(uintptr_t entity);
	Vector3      GetEntityPosition(uintptr_t entity);
	uintptr_t    GetTeam(uintptr_t entity);
	bool         isEnemy(uint64_t entity);
	void         EntityListLoop();
	Vector3      GetEntityBone(uintptr_t entity, int index);
	Vector3      RainbowESP();
	void         ESP();
	uintptr_t    GetClosestEntityToCrosshair();
	void         SetAngles(Vector3 angles, uintptr_t entity);
	void         FOV(float value);
	void         HeadAimbot();
	void         NeckAimbot();
	void         BodyAimbot();
}
