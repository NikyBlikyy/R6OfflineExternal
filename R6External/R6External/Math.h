#pragma once
#include "Includes.h"
#include <xmmintrin.h>
#include "Mem.h"

#ifndef VECTOR_H
#define VECTOR_H

class Vector2
{
public:
	Vector2() : x(0.f), y(0.f)
	{

	}

	Vector2(float _x, float _y) : x(_x), y(_y)
	{

	}
	~Vector2()
	{

	}

	float x;
	float y;

	Vector2& operator+=(const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	bool equals(Vector2 other)
	{
		return (x == other.x && y == other.y);
	}

};

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float x, float y, float z);

	Vector3 operator+(const Vector3& vector) const;
	Vector3 operator-(const Vector3& vector) const;
	Vector3 operator-() const;
	Vector3 operator*(float number) const;
	Vector3 operator/(float number) const;

	Vector3& operator+=(const Vector3& vector);
	Vector3& operator-=(const Vector3& vector);
	Vector3& operator*=(float number);
	Vector3& operator/=(float number);

	bool operator==(const Vector3& vector) const;
	bool operator!=(const Vector3& vector) const;

	inline float Dot(const Vector3& vector)
	{
		return x * vector.x + y * vector.y + z * vector.z;
	}

	inline float Distance(const Vector3& vector)
	{
		float xCoord = vector.x - x;
		float yCoord = vector.y - y;
		float zCoord = vector.z - z;
		return sqrtf((xCoord * xCoord) + (yCoord * yCoord) + (zCoord * zCoord));
	}

	inline float Length()
	{
		return sqrt((x * x) + (y * y) + (z * z));
	}

	inline void Rotate2D(float angle)
	{
		// use local variables to find transformed components
		float Vx1 = cosf(angle) * x - sinf(angle) * y;
		float Vy1 = sinf(angle) * x + cosf(angle) * y;
		// store results thru the pointer
		x = Vx1;
		y = Vy1;

		return;
	}

	inline bool Normalize()
	{
		if (x != x || y != y || z != z) return false;

		if (x > 180) x -= 360.f;
		if (x < -180) x += 360.f;
		if (y > 180.f) y -= 360.f;
		if (y < -180.f) y += 360.f;

		return x >= -180.f && x <= 180.f && y >= -180.f && y <= 180.f;
	}

	inline void Clamp()
	{
		if (x > 75.f) x = 75.f;
		else if (x < -75.f) x = -75.f;
		if (z < -180) z += 360.0f;
		else if (z > 180) z -= 360.0f;

		y = 0.f;
	}
};

inline bool Vector3::operator==(const Vector3& vector) const
{
	return x == vector.x && y == vector.y && z == vector.z;
}

inline bool Vector3::operator!=(const Vector3& vector) const
{
	return x != vector.x || y != vector.y || z != vector.z;
}

inline Vector3 Vector3::operator+(const Vector3& vector) const
{
	return Vector3(x + vector.x, y + vector.y, z + vector.z);
}

inline Vector3 Vector3::operator-(const Vector3& vector) const
{
	return Vector3(x - vector.x, y - vector.y, z - vector.z);
}

inline Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

inline Vector3 Vector3::operator*(float number) const
{
	return Vector3(x * number, y * number, z * number);
}

inline Vector3 Vector3::operator/(float number) const
{
	return Vector3(x / number, y / number, z / number);
}

inline Vector3::Vector3()
{
}

inline Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

inline Vector3& Vector3::operator+=(const Vector3& vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;

	return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	return *this;
}

inline Vector3& Vector3::operator*=(float number)
{
	x *= number;
	y *= number;
	z *= number;

	return *this;
}

inline Vector3& Vector3::operator/=(float number)
{
	x /= number;
	y /= number;
	z /= number;

	return *this;
}

class Vector4
{
public:
	Vector4() : x(0.f), y(0.f), z(0.f), w(0.f)
	{

	}

	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{

	}
	~Vector4()
	{

	}

	inline Vector4 operator-(const Vector4& vector) const
	{
		return Vector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	}

	float x;
	float y;
	float z;
	float w;
};

inline float DistanceVec2(Vector2 src, Vector2 dst)
{
	float distance;
	distance = sqrtf(powf(src.x - dst.x, 2) + powf(src.y - dst.y, 2));
	return distance;
}

inline float DistanceVec3(Vector3 src, Vector3 dst)
{
	float distance;
	distance = sqrtf(powf(src.x - dst.x, 2) + powf(src.y - dst.y, 2) + powf(src.z - dst.z, 2));
	return distance;
}
#endif

void TransformsCalculation(__int64 pBones, __m128* ResultPosition, __m128* BoneInfo);

Vector4 CalculateQuaternion(Vector3 euler);

Vector3 CalculateEuler(Vector4 quat);

Vector3 CalcAngle(Vector3 enemyPos, Vector3 cameraPos);

Vector3 calculate_angle(Vector3 translation, Vector3 position);

Vector3 calculate_euler(Vector4 quaternion);

Vector3 GetPawnBone(uintptr_t Pawn, __int64 BoneID);

auto m128ToVec3(__m128 input)->Vector3;

bool WorldToScreen(Vector3 position, Vector2* Screen);
