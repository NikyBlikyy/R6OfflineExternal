#include "Decrypt.h"
#include "mem.h"
#include "Defs.h"

std::uintptr_t Decrypt::DecryptProfileManager()
{
	//Profile: (__ROL8__(__ROL8__(BaseAddress + 0x5EE9B58 ^ 0x5FFFC7A102E8C151i64, 0x24) - 0x5Di64), 0x11) ^ 0x9316BF9A8905FBC3ui64) - 0x43)

	uintptr_t profileManager = ((__ROL8__(mem::Read<uint64_t>(__ROL8__(mem::Read<uint64_t>(SDK::moduleBase + 0x5EE9B58) ^ 0x5FFFC7A102E8C151, 0x24) - 0x5D), 0x11) ^ 0x9316BF9A8905FBC3u) - 0x43);

	printf("Profile Manager: 0x%p\n", profileManager);

	return mem::Read<std::uintptr_t>(profileManager);
}

std::uintptr_t Decrypt::DecryptGameManager()
{
	//GameManagerDecrypt = (__ROL8__(qword_7102990, 0x24) - 111i64) ^ 0x69C7993AD4EEF7BAi64);

	std::uintptr_t GameManagerKey = mem::Read<int64>(SDK::moduleBase + 0x7102990);
	std::uintptr_t decryptKey = GameManagerKey;

	std::uintptr_t v8 = __ROL8__(decryptKey, 0x24) - 0x6F;

	v8 ^= 0x69C7993AD4EEF7BA;

	//printf("Game Manager: 0x%p\n", v8);

	return v8;
}

std::uintptr_t Decrypt::DecryptRoundManager()
{
	uintptr_t roundManager = __ROL8__((mem::Read<uint64_t>(SDK::moduleBase + 0x72B59F0) ^ 0xC338AAB395DBEF6) + 0x7360742F891A8FE1, 28);

	printf("Round Manager %p\n", roundManager);

	return roundManager;
}

std::uintptr_t Decrypt::DecryptFovManager()
{
	//uintptr_t fovManager = (_rotl64(*reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(SDK::moduleBase + 0x6E9FD90) + 0x120) ^ 0x9C0D901FB0C55FEE, 0x35) + 0x15AD9B5573CCF470);
	//return fovManager;
}

std::uintptr_t Decrypt::GetCurrentState()
{
	DWORD CurrentState = mem::Read<DWORD>(Decrypt::DecryptRoundManager() + 0x90);
	CurrentState = ((__ROL4__(CurrentState, 0x14) - 0x5D) ^ 0x4D0F07BE);

	//printf("Current State: %i", CurrentState);

	return CurrentState;
}

std::uintptr_t Decrypt::GetCamera()
{
	auto camera_manager = mem::Read<uintptr_t>(mem::Read<uintptr_t>(Decrypt::DecryptProfileManager()) + 0x180) - 72;
	return camera_manager;
}

Vector3 Decrypt::GetViewRight()
{
	Vector3 right = mem::Read<Vector3>(Decrypt::GetCamera() + 0x1C0 - 0x10);
	return right;
}

Vector3 Decrypt::GetViewUp()
{
	Vector3 up = mem::Read<Vector3>(Decrypt::GetCamera() + 0x1D0 - 0x10);
	return up;
}

Vector3 Decrypt::GetViewForward()
{
	Vector3 forward = mem::Read<Vector3>(Decrypt::GetCamera() + 0x1E0 - 0x10);
	return forward;
}

Vector3 Decrypt::GetViewTranslation()
{
	Vector3 translation = mem::Read<Vector3>(Decrypt::GetCamera() + 0x1F0 - 0x10);
	return translation;
}

float Decrypt::GetFovX()
{
	Vector3 FOVx = mem::Read<Vector3>(Decrypt::GetCamera() + 0x380 - 0x10);
	return FOVx.x;
}

float Decrypt::GetFovY()
{
	Vector3 FOVy = mem::Read<Vector3>(Decrypt::GetCamera() + 0x384 - 0x10);
	return FOVy.x;
}
	
auto Decrypt::DecryptWeapon(uintptr_t localPawn)->uintptr_t
{
	uintptr_t weapon = mem::Read<uintptr_t>(Decrypt::DecryptPawn() + 0x238 + 0x6EC1608A42270296) ^ 0x8A9CBF390CFB678E + 0x680616CBD653B059;

	return weapon;
}

auto Decrypt::DecryptEntityList() -> uintptr_t
{
	uintptr_t list = mem::Read<uint64_t>(Decrypt::DecryptGameManager() + 0x80) ^ 0x6BBF520EADBF6BB9 - 0x31F66D17B5608FAD >> 0xF;

	printf("Entity List: %p\n", list);
	return list;
}

auto Decrypt::DecryptEntityCount() -> uint32_t
{
	auto count_encrypted = mem::Read<uintptr_t>(Decrypt::DecryptGameManager() + 0x160);
	auto cunt = (_rotl64(count_encrypted + 0x2D51260417837813, 44) - 0x2210905C863201E9) & 0x3FFFFFFF;

	uintptr_t count = mem::Read<uint64_t>(Decrypt::DecryptGameManager() + 0x80) ^ 0x6BBF520EADBF6BB9 - 0x31F66D17B5608FAD >> 0xF & 0x3FFFFFFF;

	printf("Entity Count: %d\n\n", count);

	return static_cast<uint32_t>(count);
}

uint32_t Decrypt::DecryptSpread(float value)
{
	uint32_t bytes = *(uint32_t*)&value;
	return __ROL4__(__ROL4__(bytes, 0x11) + 0x36, 0x13);
}

uint32_t Decrypt::DecryptRecoilY3(float value)
{
	uint32_t bytes = *(uint32_t*)&value;
	return (bytes - 0x135547C4) ^ 0xDB3C7B39;
}

uint32_t Decrypt::DecryptRecoilY2(float value)
{
	uint32_t bytes = *(uint32_t*)&value;
	return ((bytes + 0x6) ^ 0x5ABF2EBC) + 0x2CD235AE;
}

Vector4 Decrypt::DecryptAimbotAngles(Vector4 ang)
{
	__m128 angles = *(__m128*)&ang;
	angles.m128_u64[0] ^= 0x7DCA2E95666DA8A;
	angles.m128_u64[1] ^= 0x7DCA2E95666DA8A;
	angles.m128_u64[0] = (angles.m128_u64[0] >> 0x24) | (angles.m128_u64[0] << 0x1C);
	angles.m128_u64[1] = (angles.m128_u64[1] >> 0x24) | (angles.m128_u64[1] << 0x1C);
 
	return *(Vector4*) & angles;
}

Vector4 Encrypt::EncryptAimbotAngles(Vector4 ang)
{
	__m128 angles = *(__m128*) & ang;
	angles.m128_u64[0] = (angles.m128_u64[0] >> 0x1C) | (angles.m128_u64[0] << 0x24);
	angles.m128_u64[1] = (angles.m128_u64[1] >> 0x1C) | (angles.m128_u64[1] << 0x24);
	angles.m128_u64[0] ^= 0x7DCA2E95666DA8A;
	angles.m128_u64[1] ^= 0x7DCA2E95666DA8A;

	return *(Vector4*)&angles;
}

std::uintptr_t Decrypt::DecryptLocalPlayer()
{
	uintptr_t localPlayer = (__ROL8__(mem::Read<uint64_t>(Decrypt::DecryptProfileManager() + 0x80), 0x24) ^ 9) - 0x13;

	return localPlayer;
}

uintptr_t Decrypt::DecryptPawn()
{
	//_ROL8__(__ROL8__(*(_QWORD *)(localPlayer + 0x40), 0x2E) + 0x272A6ADB9DAC59ADi64, 0x23))

	uintptr_t localPawn = (__ROL8__(__ROL8__(Decrypt::DecryptLocalPlayer() + 0x40, 0x2E) + 0x272A6ADB9DAC59AD, 0x23));

	return localPawn;
}

std::uintptr_t Decrypt::DecryptEntityPawn(uintptr_t entity)
{
	uintptr_t enemyPawn = (__ROL8__(__ROL8__(entity + 0x40, 0x2E) + 0x272A6ADB9DAC59AD, 35));

	return enemyPawn;
}

uintptr_t Decrypt::DecryptLocalComponents()
{
	uintptr_t Pawn = Decrypt::DecryptPawn();
	uintptr_t Components = mem::Read<uintptr_t>(Pawn + 0x18);
	uintptr_t ComponentList = mem::Read<uintptr_t>(Components + 0xD8);
	uintptr_t DamageComponent = mem::Read<uintptr_t>(ComponentList + 0x8);

	return DamageComponent;
}
