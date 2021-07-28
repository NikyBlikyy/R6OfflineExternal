#include "Math.h"
#include "Decrypt.h"

Vector3 CalculateEuler(Vector4 quat)
{
	auto y_p2 = quat.y * quat.y;

	auto x = std::atan2(2.f * (quat.w * quat.z + quat.x * quat.y), (1.f - 2.f * (y_p2 + quat.z * quat.z))) * 57.2957795131f;
	auto y = std::atan2(2.f * (quat.w * quat.x + quat.y * quat.z), (1.f - 2.f * (quat.x * quat.x + y_p2))) * 57.2957795131f;

	return Vector3(x, y, 0.f);
}

Vector3 CalcAngle(Vector3 enemyPos, Vector3 cameraPos)
{
	Vector3 dir = enemyPos - cameraPos;
	float x = asinf(dir.z / dir.Length()) * 57.2957795131f;

	float z = atanf(dir.y / dir.x) * 57.2957795131f;

	if (dir.x >= 0.f) z += 180.f;
	if (x > 179.99f) x -= 360.f;
	else if (x < -179.99f) x += 360.f;

	return Vector3(x, 0.f, z + 90.f);
}

Vector3 calculate_angle(Vector3 translation, Vector3 position)
{
	auto delta = translation - position;

	auto y = -(std::asin(delta.z / fabsf(DistanceVec3(translation, position)))) * 57.2957795131f;
	auto x = std::atan2(delta.y, delta.x) * 57.2957795131f;

	return Vector3(x + 90.f, y, 0.f);
}

Vector3 calculate_euler(Vector4 quaternion)
{
	return Vector3(std::atan2(2.f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y), (1.f - 2.f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z))) * 57.2957795131f,
		std::atan2(2.f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z), (1.f - 2.f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y))) * 57.2957795131f,
		0.f);
}

Vector4 CalculateQuaternion(Vector3 euler)
{
	Vector4 result{};

	auto yaw = (euler.z * 0.01745329251f) * 0.5f;
	auto sy = std::sin(yaw);
	auto cy = std::cos(yaw);

	auto roll = (euler.x * 0.01745329251f) * 0.5f;
	auto sr = std::sin(roll);
	auto cr = std::cos(roll);

	constexpr auto sp = 0.f;
	constexpr auto cp = 1.f;

	result.x = cy * sr * cp - sy * cr * sp;
	result.y = cy * cr * sp + sy * sr * cp;
	result.z = sy * cr * cp - cy * sr * sp;
	result.w = cy * cr * cp + sy * sr * sp;

	return result;
}

void TransformsCalculation(__int64 pBones, __m128* ResultPosition, __m128* BoneInfo)
{
	__m128 v5; // xmm2
	__m128 v6; // xmm3
	__m128 v7; // xmm0
	__m128 v8; // xmm4
	__m128 v9; // xmm1

	__m128 v10 = { 0.500f, 0.500f, 0.500f, 0.500f };
	__m128 v11 = { 2.000f, 2.000f, 2.000f, 0.000f };

	__m128 v12 = mem::Read<__m128>(pBones);
	__m128 v13 = mem::Read<__m128>(pBones + 0x10);

	v5 = v13;

	v6 = _mm_mul_ps(*(__m128*)BoneInfo, v5);
	v6.m128_f32[0] = v6.m128_f32[0]
		+ (float)(_mm_cvtss_f32(_mm_shuffle_ps(v6, v6, 0x55)) + _mm_cvtss_f32(_mm_shuffle_ps(v6, v6, 0xAA)));
	v7 = _mm_shuffle_ps(v13, v5, 0xFF);
	v8 = _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(v5, v5, 0xD2), *(__m128*)BoneInfo),
		_mm_mul_ps(_mm_shuffle_ps(*(__m128*)BoneInfo, *(__m128*)BoneInfo, 0xD2), v5));
	v9 = _mm_shuffle_ps(v12, v12, 0x93);
	v9.m128_f32[0] = 0.0;
	*(__m128*)ResultPosition = _mm_add_ps(
		_mm_shuffle_ps(v9, v9, 0x39),
		_mm_mul_ps(
			_mm_add_ps(
				_mm_add_ps(
					_mm_mul_ps(_mm_shuffle_ps(v8, v8, 0xD2), v7),
					_mm_mul_ps(_mm_shuffle_ps(v6, v6, 0), v5)),
				_mm_mul_ps(
					_mm_sub_ps(_mm_mul_ps(v7, v7), (__m128)v10),
					*(__m128*)BoneInfo)),
			(__m128)v11));
}
inline void SubtractVector(float* Product, float* Vector1, float* Vector2)
{
	Product[0] = Vector1[0] - Vector2[0];
	Product[1] = Vector1[1] - Vector2[1];
	Product[2] = Vector1[2] - Vector2[2];
}

void AddVector(float* Product, float* Vector1, float* Vector2)
{
	Product[0] = Vector1[0] + Vector2[0];
	Product[1] = Vector1[1] + Vector2[1];
	Product[2] = Vector1[2] + Vector2[2];
}

void DivideVector(float* Product, float* Vector1, float Amount)
{
	Product[0] = Vector1[0] / Amount;
	Product[1] = Vector1[1] / Amount;
	Product[2] = Vector1[2] / Amount;
}

inline void MultiplyVector(float* Vector, float Amount)
{
	for (int i = 0; i < 3; i++)
	{
		Vector[i] = Vector[i] * Amount;
	}
}

float VectorDot(float* VectorStart, float* VectorVariable)
{
	return VectorStart[0] * VectorVariable[0] + VectorStart[1] * VectorVariable[1] + VectorStart[2] * VectorVariable[2];
}

auto m128ToVec3(__m128 input) -> Vector3 
{
	Vector3 to_return;

	to_return.x = input.m128_f32[0];
	to_return.y = input.m128_f32[1];
	to_return.z = input.m128_f32[2];

	return to_return;
}

Vector3 GetPawnBone(uintptr_t Pawn, __int64 BoneID)
{
	__m128 Output;
	__int64 pBonesChain1 = mem::Read<__int64>(Pawn + 0xC60);
	pBonesChain1 ^= 0x27C7B7F69E023E21;
	pBonesChain1 = _rotl64(pBonesChain1, 0x8);
	__int64 pBonesChain2 = mem::Read<__int64>(pBonesChain1);
	__int64 pBones = mem::Read<__int64>(pBonesChain2 + 0x240);
	__int64 pBonesData = mem::Read<__int64>(pBones + 0x58);
	__m128 BoneInfo = mem::Read<__m128>((0x20 * BoneID) + pBonesData);
	TransformsCalculation(pBones, &Output, &BoneInfo);
	return Vector3(Output.m128_f32[0], Output.m128_f32[1], Output.m128_f32[2]);
}

bool WorldToScreen(Vector3 position, Vector2* Screen)
{
	Vector3 temp = position - Decrypt::GetViewTranslation();
	float x = temp.Dot(Decrypt::GetViewRight());
	float y = temp.Dot(Decrypt::GetViewUp());
	float z = temp.Dot(Decrypt::GetViewForward() * -1.f);

	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	Screen->x = (width / 2.f) * (1.f + x / -Decrypt::GetFovX() / z);
	Screen->y = (height / 2.f) * (1.f - y / -Decrypt::GetFovY() / z);

	return z >= 0.1f ? true : false;
}
