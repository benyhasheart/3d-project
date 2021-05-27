#pragma once
#include "BindableBase.h"
#include "Drawable.h"
#include "Frustum.h"

class Camera : public Drawable
{
public:
	Camera();
	Camera(Camera& src) = default;
	Camera& operator = (Camera& src);
	virtual ~Camera() = default;

public:

	virtual DirectX::XMMATRIX GetTransform() const noexcept override final;

	virtual bool Initialize(Graphics& graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept override ;
	virtual bool PreRender(Graphics& graphics) noexcept override final;
	virtual bool Render(Graphics& graphics) noexcept override final;
	virtual bool PostRender(Graphics& graphics) noexcept override final;
	virtual bool Release() noexcept override final { return false; }
	
public:
	DirectX::XMMATRIX CreateViewMatrix(DirectX::FXMVECTOR eye, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up) noexcept;
	bool CreateFrustum(Graphics& graphics) noexcept;
	
	virtual void MoveLook(float velocity) noexcept;
	virtual void MoveUp(float velocity) noexcept;
	virtual void MoveSide(float velocity) noexcept;

public: //get,set
	void SetWorldMatrix(DirectX::XMMATRIX matrix) noexcept;
	const DirectX::XMMATRIX& GetWorldMatrix() const noexcept;

	void SetViewMatrix(DirectX::XMMATRIX matrix)noexcept;
	const DirectX::XMMATRIX& GetViewMatrix() const noexcept;

	void SetProjectionMatrix(DirectX::XMMATRIX matrix)noexcept;
	const DirectX::XMMATRIX& GetProjectionMatrix() const noexcept;

	void SetPosition(DirectX::XMVECTOR vector)noexcept;
	const DirectX::XMVECTOR& GetPosition() const noexcept;

	void SetTargetPosition(DirectX::XMVECTOR vector)noexcept;
	const DirectX::XMVECTOR& GetTargetPosition() const noexcept;

	void SetDistance(float inData)noexcept;
	const float& GetDistance() const noexcept;
	void AddDistance(float inData)noexcept;

	void SetPitchYawRoll(DirectX::XMVECTOR vector)noexcept;
	const DirectX::XMVECTOR& GetPitchYawRoll() const noexcept;
	void AddPitchYawRoll(DirectX::XMVECTOR vector)noexcept;

	mydx::Frustum& GetFrustum() const noexcept;

protected:
	void UpdateVectorStateFormViewMatrix() noexcept;


protected:
	DirectX::XMMATRIX mWorldMatrix;
	DirectX::XMMATRIX mViewMatrix;
	DirectX::XMMATRIX mProjectionMatrix;

	DirectX::XMVECTOR mLookVector;
	DirectX::XMVECTOR mUpVector;
	DirectX::XMVECTOR mRightVector;

	DirectX::XMVECTOR mTargetPositionVector;
	DirectX::XMVECTOR mPositionVector;
	DirectX::XMVECTOR mPitchYawRoll;

	DirectX::XMVECTOR mQuaternionRotation;

	float mDistance;

	std::unique_ptr<mydx::Frustum> mFrustum;

};

