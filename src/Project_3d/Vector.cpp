
#include <cmath>
#include "Vector.h"

using namespace dxmath;
dxmath::Vector3::Vector3()
{
	x = y = z = 0.0f;
}

dxmath::Vector3::Vector3(const Vector3& src)
{
	x = src.x;
	y = src.y;
	z = src.z;
}

dxmath::Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3 dxmath::Vector3::operator+(Vector3 const& destVector)
{
	return Vector3(x + destVector.x, y + destVector.y, z + destVector.z);
}

Vector3 dxmath::Vector3::operator-(Vector3 const& destVector)
{
	return Vector3(x - destVector.x, y - destVector.y, z - destVector.z);
}



Vector3 dxmath::Vector3::operator*(float const& scala)
{
	return  Vector3(scala * x , scala * y, scala * z);
}

Vector3 dxmath::Vector3::operator/(float const& scala)
{
	const float invert = 1.0f / scala;
	return  Vector3( x * invert,  y * invert,  scala * invert);
}

Vector3 dxmath::Vector3::operator+(float const& scala)
{
	return  Vector3(scala + x, scala + y, scala + z);
}

bool dxmath::Vector3::operator==(Vector3 const& destVector)
{
	if (fabsf(x - destVector.x) < EPSILON)
	{
		if (fabsf(y - destVector.y) < EPSILON)
		{
			if (fabsf(z - destVector.z) < EPSILON)
			{
				return true;
			}
		}
	}
	return false;
}

float dxmath::Vector3::LengthSquared()
{
	return ( x * x + y * y + z * z);
}

float dxmath::Vector3::Length()
{
	return sqrtf(LengthSquared());
}

Vector3 dxmath::Vector3::Normal()
{
	float invertLength = 1.0f / Length();
	return Vector3( x * invertLength, y * invertLength, z * invertLength);
}

float dxmath::Vector3::Dot(Vector3 const& destVector)
{
	return x * destVector.x + y * destVector.y + z * destVector.z;
}

Vector3 dxmath::Vector3::Cross(Vector3 const& destVector)
{
	return Vector3( (y * destVector.z - z* destVector.y) , (z * destVector.x - x * destVector.z), (x * destVector.y - y * destVector.x) );
}

float dxmath::Vector3::Angle(Vector3& destVector)
{
	Vector3 src = Normal();
	Vector3 dest = destVector.Normal();
	float dot = src.Dot(dest);
	float result = acosf(dot) * ( 180 / PI);
	return result;
}

dxmath::Vector4::Vector4()
{
	x = y = z = w = 0.0f;
}

dxmath::Vector4::Vector4(const Vector4& src)
{
	x = src.x;
	y = src.y;
	z = src.z;
	w = src.w;
}

dxmath::Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

dxmath::Vector2::Vector2()
{
	x = y = {};
}

dxmath::Vector2::Vector2(const Vector2& src)
{
	x = src.x;
	y = src.y;
}

dxmath::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}
