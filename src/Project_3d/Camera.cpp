#include "Camera.h"

using namespace DirectX;

Camera::Camera()
	:	
	mWorldMatrix(::XMMatrixIdentity()),
	mViewMatrix(::XMMatrixIdentity()),
	mProjectionMatrix(::XMMatrixIdentity()),
	mLookVector(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mUpVector(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mRightVector(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mPositionVector(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mTargetPositionVector(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mPitchYawRoll(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mQuaternionRotation(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mDistance(0.0f)
{
}

Camera& Camera::operator=(Camera& src)
{
    // TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

bool Camera::Update(Graphics& graphics) noexcept
{
	ResourceBinding(graphics);
	
	mFrustum->CreateFrustum(mViewMatrix, mProjectionMatrix);
	mFrustum->SetPitchYawRoll(mPitchYawRoll);
	mFrustum->SetPosition(mPositionVector);
	mFrustum->Update(graphics);
	return false;
}

DirectX::XMMATRIX Camera::GetTransform() const noexcept
{
	return mWorldMatrix;
}

bool Camera::Initialize(Graphics& graphics) noexcept
{

	return false;
}

bool Camera::PreRender(Graphics& graphics) noexcept
{
	ResourceBinding(graphics);
	return false;
}

bool Camera::Render(Graphics& graphics) noexcept
{

	PreRender(graphics);
	Draw(graphics);
	PostRender(graphics);

	mFrustum->Render(graphics);
	return false;
}

bool Camera::PostRender(Graphics& graphics) noexcept
{
	return false;
}

DirectX::XMMATRIX Camera::CreateViewMatrix(DirectX::FXMVECTOR eye, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up) noexcept
{
	mViewMatrix = ::XMMatrixLookAtLH(eye, target, up);

	UpdateVectorStateFormViewMatrix();
	SetPosition(eye);
	SetTargetPosition(target);

	::XMFLOAT4 storedValue;
	::XMStoreFloat4(&storedValue, ::XMVector4Length(target - eye));
	SetDistance(storedValue.x);
	
	return mViewMatrix;
}

bool Camera::CreateFrustum(Graphics& graphics) noexcept
{
	mFrustum = std::make_unique<mydx::Frustum>();
	mFrustum->CreateFrustum(mViewMatrix, mProjectionMatrix);
	mFrustum->Initialize(graphics);
	return true;
}

void Camera::MoveLook(float velocity) noexcept
{
	mPositionVector += mLookVector * velocity;
}

void Camera::MoveUp(float velocity) noexcept
{
	mPositionVector += mUpVector * velocity;
}

void Camera::MoveSide(float velocity) noexcept
{
	mPositionVector += mRightVector * velocity;
}



void Camera::UpdateVectorStateFormViewMatrix() noexcept
{
	::XMFLOAT4X4 convertedFromViewMatrix = {};
	::XMStoreFloat4x4(&convertedFromViewMatrix,mViewMatrix);

	mRightVector = ::XMVectorSet(convertedFromViewMatrix._11, convertedFromViewMatrix._21, convertedFromViewMatrix._31,0.0f);
	mUpVector = ::XMVectorSet(convertedFromViewMatrix._12, convertedFromViewMatrix._22, convertedFromViewMatrix._32, 0.0f);
	mLookVector = ::XMVectorSet(convertedFromViewMatrix._13, convertedFromViewMatrix._23, convertedFromViewMatrix._33, 0.0f);

}

void Camera::SetWorldMatrix(DirectX::XMMATRIX matrix)noexcept
{
	mWorldMatrix = matrix;
}

const DirectX::XMMATRIX& Camera::GetWorldMatrix() const noexcept
{
	return mWorldMatrix;
}

void Camera::SetViewMatrix(DirectX::XMMATRIX matrix)noexcept
{
	mViewMatrix = matrix;
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const noexcept
{
	return mViewMatrix;
}

void Camera::SetProjectionMatrix(DirectX::XMMATRIX matrix)noexcept
{
	mProjectionMatrix = matrix;
}

const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const noexcept
{
	return mProjectionMatrix;
}

void Camera::SetPosition(DirectX::XMVECTOR vector)noexcept
{
	mPositionVector = vector;
}

const DirectX::XMVECTOR& Camera::GetPosition() const noexcept
{
	return mPositionVector;
}

void Camera::SetTargetPosition(DirectX::XMVECTOR vector) noexcept
{
	mTargetPositionVector = vector;
}

const DirectX::XMVECTOR& Camera::GetTargetPosition() const noexcept
{
	return mTargetPositionVector;
}

void Camera::SetDistance(float inData) noexcept
{
	mDistance = inData;
}

const float& Camera::GetDistance() const noexcept
{
	return mDistance;
}

void Camera::AddDistance(float inData) noexcept
{
	mDistance += inData;
}

void Camera::SetPitchYawRoll(DirectX::XMVECTOR vector) noexcept
{
	mPitchYawRoll = vector;
}

const DirectX::XMVECTOR& Camera::GetPitchYawRoll() const noexcept
{
	return mPitchYawRoll;
}

void Camera::AddPitchYawRoll(DirectX::XMVECTOR vector) noexcept
{
	mPitchYawRoll += vector;
}

mydx::Frustum& Camera::GetFrustum() const noexcept
{
	return *mFrustum;
}
