#include "Hacks.h"
#include "Decrypt.h"
#include "Mem.h"
#include "Offsets.h"
#include "Overlay.h"
#include "imgui.h"

bool Hacks::ReadAndWriteHealth(int value)
{
	uintptr_t Components = Decrypt::DecryptLocalComponents();

	mem::Write<int>(Components + HEALTH, value);

	return true;
}

bool Hacks::Speed(float value)
{
	uintptr_t profileManager = Decrypt::DecryptProfileManager();
	uintptr_t LocalPlayer = Decrypt::DecryptLocalPlayer();
	uintptr_t Pawn = Decrypt::DecryptPawn();

	mem::Write<float>(Pawn + SPEED, value);

	return true;
}

bool Hacks::NoRecoil(float Spread, float Recoil)
{
	uintptr_t localPawn = Decrypt::DecryptPawn();

	mem::Write<DWORD>(Decrypt::DecryptWeapon(localPawn) + 0x60, Decrypt::DecryptSpread(Spread));

	mem::Write<BYTE>(Decrypt::DecryptWeapon(localPawn) + 0x1BC, 0x38);
	mem::Write<DWORD>(Decrypt::DecryptWeapon(localPawn) + 0x274, Decrypt::DecryptRecoilY3(Recoil));
	mem::Write<DWORD>(Decrypt::DecryptWeapon(localPawn) + 0x270, Decrypt::DecryptRecoilY2(Recoil));

	return true;
}

bool Hacks::NoClip(float value)
{
	uintptr_t NoClipAddy = mem::ReadMultiLevelPointer<uintptr_t>(SDK::moduleBase + 0x05C82E28, { 0xA8, 0x10, 0x700 });
	mem::Write<float>(NoClipAddy, value);

	return true;
}

Vector3 Hacks::GetLocalPosition()
{
	uintptr_t LocalPawn = Decrypt::DecryptPawn();
	LocalPawn = mem::Read<uint64_t>(LocalPawn + 0x18);
	Vector3 LocalPosition = mem::Read<Vector3>(LocalPawn + 0x50);

	printf("Local Position: %f, %f, %f\n", LocalPosition.x, LocalPosition.y, LocalPosition.z);

	return LocalPosition;
}

int Hacks::ReadEntityHealth(uintptr_t entity)
{
	uint64_t healthAddr = Decrypt::DecryptEntityPawn(entity); //Basically Decrypt for pawn
	//printf("Entity Pawn: %p\n", healthAddr);
	healthAddr = mem::Read<uint64_t>(healthAddr + 0x18);
	healthAddr = mem::Read<uint64_t>(healthAddr + 0xD8);
	healthAddr = mem::Read<uint64_t>(healthAddr + 0x8);
	int health = mem::Read<int>(healthAddr + 0x170);
	//printf("Health: %i\n", health);

	return health;
}

void Hacks::ReadEntityName(uintptr_t entity)
{
	uintptr_t ReplicationInfo = __ROL8__(mem::Read<uintptr_t>(entity + 0x100), 0xB) + 0xB10B7B8A32930B60;

	uintptr_t NamePointer = mem::Read<uintptr_t>(ReplicationInfo + 0x218);

	char name[24];

	for (int j = 0; j < 24; j++)
	{
		name[j] = mem::Read<char>(NamePointer + j);
		if (name[j] == 0)
			break;
	}

	printf("Name: %s\n", name);
	printf("repInfdo: %p\n", ReplicationInfo);
}

Vector3 Hacks::GetEntityPosition(uintptr_t entity)
{
	uint64_t PosAddr = Decrypt::DecryptEntityPawn(entity);
	PosAddr = mem::Read<uint64_t>(PosAddr + 0x18);
	Vector3 Position = mem::Read<Vector3>(PosAddr + 0x50);

	//printf("PosAddr: %p\n", PosAddr);

	//printf("Entity Position: %f, %f, %f\n", Position.x, Position.y, Position.z);

	return Position;
}

uintptr_t Hacks::GetTeam(uintptr_t entity)
{
	uintptr_t ReplicationInfo = __ROL8__(mem::Read<uintptr_t>(entity + 0x100), 0xB) + 0xB10B7B8A32930B60;
	BYTE team = mem::Read<BYTE>(ReplicationInfo + 0x184);
	team = (_rotl8(team, 3) + 0xA) ^ 0x3D;

	printf("Team: %hhx\n", team);

	return team;
}

bool Hacks::isEnemy(uint64_t entity)
{
	if (entity == NULL)
		return false;

	if (Decrypt::DecryptLocalPlayer() == entity)
		return false;

	return true;
}

void Hacks::EntityListLoop()
{
	for (int i = 0; i < Decrypt::DecryptEntityCount(); i++)
	{
		uintptr_t EntityList = Decrypt::DecryptEntityList();

		uintptr_t entity = mem::Read<uintptr_t>(EntityList + i * 0x8);

		//printf("Current: %p\n", Hacks::GetEntityByID(i));

		if (!Hacks::isEnemy(entity))
			continue;

		printf("Entity List: 0x%p\n", EntityList);

		printf("Entity: %p\n", entity);

		Hacks::ReadEntityName(entity);

		Hacks::GetTeam(entity);

		Hacks::GetEntityPosition(entity);

		Hacks::ReadEntityHealth(entity);
	}
}

Vector3 Hacks::GetEntityBone(uintptr_t entity, int index)
{
	uintptr_t Pawn = Decrypt::DecryptEntityPawn(entity);

	Vector3 entityHead = GetPawnBone(Pawn, index);

	return entityHead;
}

Vector3 Hacks::RainbowESP()
{
	static uint32_t cnt = 0;
	float freq = 0.01f;

	if (cnt++ >= (uint32_t)-1)
		cnt = 0;

	Vector3 ret = { std::sin(freq * cnt + 0) * 0.5f + 0.5f, std::sin(freq * cnt + 2) * 0.5f + 0.5f , std::sin(freq * cnt + 4) * 0.5f + 0.5f };
	return ret;
}

void Hacks::ESP()
{
	Vector3 rainbowVec = RainbowESP();
	D3DCOLOR rainbowColor = D3DCOLOR_RGBA((int)(rainbowVec.x * 255), (int)(rainbowVec.y * 255), (int)(rainbowVec.z * 255), 255);

	for (int i = 0; i < Decrypt::DecryptEntityCount(); i++)
	{
		uintptr_t EntityList = Decrypt::DecryptEntityList();

		uintptr_t entity = mem::Read<uintptr_t>(EntityList + i * 0x8);

		Vector3 entityPosition = Hacks::GetEntityPosition(entity);

		if (!Hacks::isEnemy(entity))
			continue;

		if (Hacks::ReadEntityHealth(entity) <= 0)
			continue;	

		Vector2 screenFeet;
		bool isOnScreen = WorldToScreen(entityPosition, &screenFeet);

		if (isOnScreen == false) //If the entity is on screen
		{
			continue; //continue the code.
		}

		Vector2 screenPos;
		Vector3 entityHead = GetEntityBone(entity, 77);
		Vector3 entityFeet = GetEntityBone(entity, 0);

		Vector2 boxTop2D; //Screen Position of the head.

	    Vector2 screenHead;
		entityHead.z += 0.150f;
		WorldToScreen(entityHead, &screenHead);

		//Calculate Box Dimensions
		float Height = screenFeet.y - screenHead.y;
		float Width = Height / 1.2f;

		int width = 1920;
		int height = 1080;

		//Snapline
		if (config::Snaplines)
		{
			DrawLine(width / 2, height, screenFeet.x, screenFeet.y, Colors::Red);

			if (config::Snaplines && config::Rainbow)
			{
				DrawLine(width / 2, height, screenFeet.x, screenFeet.y, rainbowColor);
			}
		}
		
		//Box 
		if (config::Box)
		{
			DrawOutlinedRect((int)(screenHead.x - (Width / 2)), (int)screenHead.y, (int)Width, (int)Height, Colors::Red);

			if (config::Box && config::Rainbow)
			{
				DrawOutlinedRect((int)(screenHead.x - (Width / 2)), (int)screenHead.y, (int)Width, (int)Height, rainbowColor);
			}
		}

		//Head dot
		if (config::headDot)
		{
			Vector2 headDotPos;
			entityHead.z -= 0.150f;
			WorldToScreen(entityHead, &headDotPos);
			DrawCircle((int)headDotPos.x, (int)headDotPos.y, 4, 20, Colors::Green);

			if (config::headDot && config::Rainbow)
			{
				DrawCircle((int)headDotPos.x, (int)headDotPos.y, 4, 20, rainbowColor);
			}
		}
		
		//Healthbar
		if (config::Healthbar)
		{
			float dX = screenFeet.x - screenHead.x;

			float healthPercentage = (float)(Hacks::ReadEntityHealth(entity)) / 100.f;

			Vector2 topHealth, bottomHealth;
			float healthHeight = Height * healthPercentage;

			bottomHealth.x = screenHead.x + (Width / 2) + 6;
			bottomHealth.y = screenHead.y + Height;

			topHealth.x = screenHead.x + (Width / 2) + 6;
			topHealth.y = screenHead.y + Height - healthHeight;

			DrawLine((int)bottomHealth.x, (int)bottomHealth.y, (int)topHealth.x, (int)topHealth.y, Colors::Green);
		}
	}
}

void Hacks::SetAngles(Vector3 angles, uintptr_t entity)
{
	Vector4 AimAt = CalculateQuaternion(angles);

	uintptr_t viewAngle = mem::Read<uintptr_t>(entity + 0x13D8); //Get to camera angles
	mem::Write<Vector4>(viewAngle + 0xA0, Encrypt::EncryptAimbotAngles(AimAt)); //Quaternion angles that are encrypted
}

void Hacks::FOV(float value)
{
	int Width = GetSystemMetrics(SM_CXSCREEN) / 2;
	int Height = GetSystemMetrics(SM_CYSCREEN) / 2;

	if (config::showFOV)
	{
		DrawCircle(Width, Height, value, 35, Colors::Green);

		Vector3 rainbowVec = RainbowESP();
		D3DCOLOR rainbowColor = D3DCOLOR_RGBA((int)(rainbowVec.x * 255), (int)(rainbowVec.y * 255), (int)(rainbowVec.z * 255), 255);

		if (config::Rainbow)
		{
			DrawCircle(Width, Height, value, 35, rainbowColor);
		}
	}
}

uintptr_t Hacks::GetClosestEntityToCrosshair()
{
	float oldDistance = FLT_MAX;
	float newDistance = 0.f;

	uintptr_t entity = NULL;
	uintptr_t target = NULL;

	Vector2 screenPos;
	Vector2 CrosshairPos = { (float)(GetSystemMetrics(SM_CXSCREEN) / 2), (float)(GetSystemMetrics(SM_CYSCREEN) / 2) };

	float FOV;

	if (config::FOV)
	{
		FOV = config::fovSlider;
	}
	else
	{
		FOV = 9999.f;
	}

	for (int i = 0; i < Decrypt::DecryptEntityCount(); i++)
	{
		uintptr_t entityList = Decrypt::DecryptEntityList();

		entity = mem::Read<uintptr_t>(entityList + i * 0x8);

		if (!entity || Hacks::ReadEntityHealth(entity) <= 0) continue;

		Vector3 entityHead = GetEntityBone(entity, 77);

		if (!Hacks::isEnemy(entity)) continue;

		if (entityHead.x == 0.f && entityHead.y == 0.f && entityHead.z == 0.f) continue;

		if (WorldToScreen(entityHead, &screenPos))
		{
			if (screenPos.x < 0 || screenPos.x > GetSystemMetrics(SM_CXSCREEN) || screenPos.y < 0 || screenPos.y > GetSystemMetrics(SM_CYSCREEN))
				continue;

			newDistance = DistanceVec2(CrosshairPos, screenPos);

			if (newDistance < FOV && newDistance < oldDistance)
			{
				oldDistance = newDistance;
				target = entity;
			}
		}
	}

	return target;
}

void Hacks::HeadAimbot()
{
	Hacks::FOV(config::fovSlider);

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		uintptr_t target = 0;

		target = GetClosestEntityToCrosshair();

		if (target == NULL)
			return;

		Vector3 Angles = CalcAngle(Hacks::GetEntityBone(target, 77), Decrypt::GetViewTranslation());
		Angles.Clamp();

		SetAngles(Angles, Decrypt::DecryptPawn());
	}
}

void Hacks::NeckAimbot()
{
	Hacks::FOV(config::fovSlider);

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		uintptr_t target = 0;

		target = GetClosestEntityToCrosshair();

		if (target == NULL)
			return;

		Vector3 Angles = CalcAngle(Hacks::GetEntityBone(target, 77), Decrypt::GetViewTranslation());
		Angles.Clamp();

		SetAngles(Angles, Decrypt::DecryptPawn());
	}
}

void Hacks::BodyAimbot()
{
	Hacks::FOV(config::fovSlider);

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		uintptr_t target = 0;

		target = GetClosestEntityToCrosshair();

		if (target == NULL)
			return;

		Vector3 Angles = CalcAngle(Hacks::GetEntityBone(target, 77), Decrypt::GetViewTranslation());
		Angles.Clamp();

		SetAngles(Angles, Decrypt::DecryptPawn());
	}
}


