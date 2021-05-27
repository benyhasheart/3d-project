#include "ModelViewCamera.h"

using namespace DirectX;

ModelViewCamera::ModelViewCamera()
{
}

bool ModelViewCamera::Update(Graphics& graphics) noexcept
{	
	auto rotationMatrix = ::XMMatrixRotationRollPitchYawFromVector(mPitchYawRoll);

	auto localDirVector = ::XMVectorSet(0.0f,0.0f,1.0f,0.0f);
	auto localUpVector = ::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//벡터와 행렬의 곱, 벡터에 행렬이 얼마나 영향을 주는가.
	//XMVector3TransformCoord 이동 포함
	::XMVECTOR worldDirVector = ::XMVector3TransformCoord(localDirVector, rotationMatrix);
	::XMVECTOR worldUpVector = ::XMVector3TransformCoord(localUpVector, rotationMatrix);
	//::XMVector3TransformNormal 이동 미포함

	mPositionVector = mTargetPositionVector - (worldDirVector * mDistance);

	updateViewMatrix(mPositionVector, mTargetPositionVector, worldUpVector);

	ResourceUpdate(graphics);

	return false;
}

void ModelViewCamera::updateViewMatrix(DirectX::FXMVECTOR eye, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up) noexcept
{
	mViewMatrix = ::XMMatrixLookAtLH(eye, target, up);

	UpdateVectorStateFormViewMatrix();
	SetPosition(eye);
	SetTargetPosition(target);
}
