#pragma once
#include "Vector.h"

namespace dxmath
{
	struct float4x4
	{
		union 
		{
			struct 
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float matrix[4][4];
		};
	};

	class Matrix : public float4x4
	{
	public:
		Matrix();
		Matrix(const Matrix& src);
		Matrix& operator = (const Matrix& src);
		Matrix operator * (const Matrix& src);
		Matrix operator + (const Matrix& src);
		Matrix operator - (const Matrix& src);

		virtual ~Matrix() = default;

	public:
		void Identity();
		Matrix Transpose();
		Matrix Inverse();

		void Translation( const Vector3& vector);
		void Translation( float x, float y, float z);
		void Scale(const Vector3& vector);
		void Scale(float x, float y, float z);

		void XRotate( float radian);
		void YRotate(float radian);
		void ZRotate(float radian);
		
		void ObjectLookAt(  Vector3& position,  Vector3& target,  Vector3& up);
		//뷰 행렬 계산
		dxmath::Matrix ComputeViewMatrix( Vector3& position,  Vector3& target,  Vector3& up );
		// 투영 행렬 계산
		dxmath::Matrix PerspectiveFovLH( float nearPlane, float farPlane,float fovy, float aspect);

		
	
	};
}


