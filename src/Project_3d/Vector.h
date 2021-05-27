#pragma once
 
namespace dxmath
{
	constexpr float EPSILON = 0.001f;
	constexpr float PI = 3.141592f;
}
namespace dxmath
{
	struct float2
	{
		union
		{
			struct { float x, y; };
			float v[2];
		};
	};

	struct float3
	{
		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};

	struct float4
	{
		union
		{
			struct { float x, y, z, w; };
			float v[4];
		};
	};
	class Vector2 : public float2
	{
	public:
		Vector2();
		Vector2(const Vector2& src);
		Vector2(float x, float y);

		~Vector2() = default;
	};
	class Vector3 : public float3
	{	
	public:
		Vector3();
		Vector3(const Vector3& src);
		Vector3( float x, float y, float z );

		~Vector3() = default;
	public:
		Vector3 operator + (Vector3 const & destVector);
		Vector3 operator - (Vector3 const& destVector);
		
		Vector3 operator * (float const& scala);
		Vector3 operator / (float const& scala);
		Vector3 operator + (float const& scala);

		bool operator == (Vector3 const& v0);

	public:
		float LengthSquared();
		float Length();
		Vector3 Normal();

		float Dot(Vector3 const& destVector);
		Vector3 Cross(Vector3 const& destVector);
		float Angle(Vector3& destVector);
	};

	class Vector4 : public float4
	{
	public:
		Vector4();
		Vector4(const Vector4& src);
		Vector4(float x, float y, float z, float w);

		~Vector4() = default;
	};
}


