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

	//���Ϳ� ����� ��, ���Ϳ� ����� �󸶳� ������ �ִ°�.
	//XMVector3TransformCoord �̵� ����
	::XMVECTOR worldDirVector = ::XMVector3TransformCoord(localDirVector, rotationMatrix);
	::XMVECTOR worldUpVector = ::XMVector3TransformCoord(localUpVector, rotationMatrix);
	//::XMVector3TransformNormal �̵� ������

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
