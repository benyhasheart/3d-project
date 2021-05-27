#include "FirstPersonViewCamera.h"

using namespace DirectX;

bool FirstPersonViewCamera::Update(Graphics& graphics) noexcept
{
	mQuaternionRotation = ::XMQuaternionRotationRollPitchYawFromVector(mPitchYawRoll);
	mViewMatrix = ::XMMatrixAffineTransformation(::XMVectorSet(1.0f,1.0f,1.0f,0.0f), ::XMVectorZero(), mQuaternionRotation, mPositionVector);
	
	::XMVECTOR determinent = ::XMMatrixDeterminant(mViewMatrix);
	mViewMatrix = ::XMMatrixInverse(&determinent, mViewMatrix);

	UpdateVectorStateFormViewMatrix();

	Camera::Update(graphics);

	return false;
}
