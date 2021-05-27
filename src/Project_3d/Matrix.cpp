#pragma once
#include <cstring>
#include <math.h>

#include "Matrix.h"


dxmath::Matrix::Matrix()
{
    std::memset(matrix, 0, sizeof(matrix));
    Identity();
}

dxmath::Matrix::Matrix(const Matrix& src)
{
    _11 = src._11; _12 = src._12; _13 = src._13; _14 = src._14;
    _21 = src._21; _22 = src._22; _23 = src._23; _24 = src._24;
    _31 = src._31; _32 = src._32; _33 = src._33; _34 = src._34;
    _41 = src._41; _42 = src._42; _43 = src._43; _44 = src._44;
}

dxmath::Matrix& dxmath::Matrix::operator=(const Matrix& src)
{
    Matrix* m = this;
   _11 = src._11; _12 = src._12; _13 = src._13; _14 = src._14;
   _21 = src._21; _22 = src._22; _23 = src._23; _24 = src._24;
   _31 = src._31; _32 = src._32; _33 = src._33; _34 = src._34;
   _41 = src._41; _42 = src._42; _43 = src._43; _44 = src._44;

   std::memcpy(m->matrix,matrix, sizeof(matrix));

   return *m;
}

dxmath::Matrix dxmath::Matrix::operator*(const Matrix& src)
{
    Matrix outMatrix;
   

    for (unsigned int row = 0; row < 4; row++)
    {
        for (unsigned int column = 0; column < 4; column++)
        {
            outMatrix.matrix[row][column] = 
                matrix[row][0] * src.matrix[0][column] +
                matrix[row][1] * src.matrix[1][column] +
                matrix[row][2] * src.matrix[2][column] +
                matrix[row][3] * src.matrix[3][column] ;
        }
    }

    return outMatrix;
}


dxmath::Matrix dxmath::Matrix::operator+(const Matrix& src)
{
    Matrix outMatrix;

    for (unsigned int row = 0; row < 4; row++)
    {
        for (unsigned int column = 0; column < 4; column++)
        {
            outMatrix.matrix[row][column] = matrix[row][column] + src.matrix[row][column];
                
        }
    }

    return outMatrix;
}

dxmath::Matrix dxmath::Matrix::operator-(const Matrix& src)
{
    Matrix outMatrix;

    for (unsigned int row = 0; row < 4; row++)
    {
        for (unsigned int column = 0; column < 4; column++)
        {
            outMatrix.matrix[row][column] = matrix[row][column] - src.matrix[row][column];

        }
    }

    return outMatrix;
}

void dxmath::Matrix::Identity()
{
   _11 = _22 = _33 = _44 = 1.0f;
}

dxmath::Matrix dxmath::Matrix::Transpose()
{
    Matrix outMatrix;

    outMatrix._11 = _11; outMatrix._12 = _21; outMatrix._13 = _31; outMatrix._14 = _41;
    outMatrix._21 = _12; outMatrix._22 = _22; outMatrix._23 = _32; outMatrix._24 = _42;
    outMatrix._31 = _13; outMatrix._32 = _23; outMatrix._33 = _33; outMatrix._34 = _43;
    outMatrix._41 = _14; outMatrix._42 = _24; outMatrix._43 = _34; outMatrix._44 = _44;

    return outMatrix;
}


dxmath::Matrix dxmath::Matrix::Inverse()
{
	Matrix matInverse;
	float det;
	float d12, d13, d23, d24, d34, d41;
	float tmp[16]; /* Allow out == in. */

	/* Inverse = adjoint / det. (See linear algebra texts.)*/

	/* pre-compute 2x2 dets for last two rows when computing */
	/* cofactors of first two rows. */
	d12 = (_31 * _42 - _41 * _32);
	d13 = (_31 * _43 - _41 * _33);
	d23 = (_32 * _43 - _42 * _33);
	d24 = (_32 * _44 - _42 * _34);
	d34 = (_33 * _44 - _43 * _34);
	d41 = (_34 * _41 - _44 * _31);

	tmp[0] = (_22 * d34 - _23 * d24 + _24 * d23);
	tmp[1] = -(_21 * d34 + _23 * d41 + _24 * d13);
	tmp[2] = (_21 * d24 + _22 * d41 + _24 * d12);
	tmp[3] = -(_21 * d23 - _22 * d13 + _23 * d12);

	/* Compute determinant as early as possible using these cofactors. */
	det = _11 * tmp[0] + _12 * tmp[1] + _13 * tmp[2] + _14 * tmp[3];


	{
		float invDet = 1.0f / det;
		/* Compute rest of inverse. */
		tmp[0] *= invDet;
		tmp[1] *= invDet;
		tmp[2] *= invDet;
		tmp[3] *= invDet;

		tmp[4] = -(_12 * d34 - _13 * d24 + _14 * d23) * invDet;
		tmp[5] = (_11 * d34 + _13 * d41 + _14 * d13) * invDet;
		tmp[6] = -(_11 * d24 + _12 * d41 + _14 * d12) * invDet;
		tmp[7] = (_11 * d23 - _12 * d13 + _13 * d12) * invDet;

		/* Pre-compute 2x2 dets for first two rows when computing */
		/* cofactors of last two rows. */
		d12 = _11 * _22 - _21 * _12;
		d13 = _11 * _23 - _21 * _13;
		d23 = _12 * _23 - _22 * _13;
		d24 = _12 * _24 - _22 * _14;
		d34 = _13 * _24 - _23 * _14;
		d41 = _14 * _21 - _24 * _11;

		tmp[8] = (_42 * d34 - _43 * d24 + _44 * d23) * invDet;
		tmp[9] = -(_41 * d34 + _43 * d41 + _44 * d13) * invDet;
		tmp[10] = (_41 * d24 + _42 * d41 + _44 * d12) * invDet;
		tmp[11] = -(_41 * d23 - _42 * d13 + _43 * d12) * invDet;
		tmp[12] = -(_32 * d34 - _33 * d24 + _34 * d23) * invDet;
		tmp[13] = (_31 * d34 + _33 * d41 + _34 * d13) * invDet;
		tmp[14] = -(_31 * d24 + _32 * d41 + _34 * d12) * invDet;
		tmp[15] = (_31 * d23 - _32 * d13 + _33 * d12) * invDet;

		memcpy(matInverse.matrix, tmp, 16 * sizeof(float));
		matInverse = matInverse.Transpose();
	}
	
	return matInverse;
}


void dxmath::Matrix::Translation(const Vector3& vector)
{
    _41 = vector.x; _42 = vector.y; _43 = vector.z;
}

void dxmath::Matrix::Translation(float x, float y, float z)
{
    _41 = x; _42 = y; _43 = z;
}

void dxmath::Matrix::Scale(const Vector3& vector)
{
    _11 = vector.x; _22 = vector.y; _33 = vector.z;
}

void dxmath::Matrix::Scale(float x, float y, float z)
{
    _11 = x; _22 = y; _33 = z;
}

void dxmath::Matrix::XRotate(float radian)
{
    float cosTheta = cosf(radian);
    float sinTheta = sinf(radian);

    _22 = cosTheta; _23 =sinTheta;
    _32 = -sinTheta; _33 = cosTheta;
}

void dxmath::Matrix::YRotate(float radian)
{
    float cosTheta = cosf(radian);
    float sinTheta = sinf(radian);

    _21 = cosTheta; _23 = -sinTheta;
    _31 = sinTheta; _33 = cosTheta;
}

void dxmath::Matrix::ZRotate(float radian)
{
    float cosTheta = cosf(radian);
    float sinTheta = sinf(radian);

    _11 = cosTheta; _12 = -sinTheta;
    _22 = -sinTheta; _23 = cosTheta;
}

void dxmath::Matrix::ObjectLookAt( Vector3& position,  Vector3& target,  Vector3& up)
{
    Vector3 direction = target - position;
    direction = direction.Normal();
    // 두 벡터에 대해 직교인 벡터 생성 (그람 슈미트 직렬 정규화)
    float dot = up.Dot(direction); 
    Vector3 upVector = up - ( direction * dot);
    upVector = upVector.Normal();
    Vector3 rightVector = upVector.Cross(direction);

    _11 = rightVector.x; _12 = rightVector.y; _13 = rightVector.z;
    _21 = upVector.x; _22 = upVector.y; _23 = upVector.z;
    _31 = direction.x; _32 = direction.y; _33 = direction.z;
    _41 = position.x; _42 = position.y; _43 = position.z;

}

dxmath::Matrix dxmath::Matrix::ComputeViewMatrix( Vector3& position,  Vector3& target,  Vector3& up)
{
    Matrix matrix;
    ObjectLookAt(position, target, up);
    
    _41 = _42 = _43 = 0.0f;
    matrix = matrix.Transpose();

    _41 = -( position.x * matrix._11 + position.y * matrix._21 + position.z * matrix._31  );
    _42 = -(position.x * matrix._12 + position.y * matrix._22 + position.z * matrix._32);
    _43 = -(position.x * matrix._13 + position.y * matrix._23 + position.z * matrix._33);
    std::memcpy( &matrix, this, sizeof(dxmath::Matrix));
    return matrix;

}

dxmath::Matrix dxmath::Matrix::PerspectiveFovLH(float nearPlane, float farPlane, float fovy, float aspect)
{
    float h = 1/tanf(fovy * 0.5f);
    float w = h/aspect;
    float q = farPlane / ( farPlane - nearPlane);

    Matrix matrix;
    
    matrix._11 = w;
    matrix._22 = h;
    matrix._33 = q;
    matrix._43 = -q*farPlane;
    matrix._34 = 1;

    return matrix;
}
