#pragma once
#include "CommonDataStruct.h"
#include "Drawable.h"
#include "EnumData.h"
#include "BindableBase.h"

namespace mydx
{

class BoundingBox;
class Frustum final : public Drawable
{
public:
	Frustum();
	virtual ~Frustum() = default;
	Frustum(Graphics& graphics);

	Frustum(Frustum& src) = delete;
	Frustum& operator = (Frustum& src) = delete;

public:
	virtual DirectX::XMMATRIX GetTransform() const noexcept override final;

	virtual bool Initialize(Graphics& graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept override;
	virtual bool PreRender(Graphics& graphics) noexcept override final;
	virtual bool Render(Graphics& graphics) noexcept override final;
	virtual bool PostRender(Graphics& graphics) noexcept override final;
	virtual bool Release() noexcept override final { return false; }

public:
	bool CreateFrustum(DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix ) noexcept;
	DirectX::XMFLOAT4 CreatePlane(DirectX::XMVECTOR point0, DirectX::XMVECTOR point1, DirectX::XMVECTOR point2) noexcept;

public:
	bool InspectionPoint(DirectX::XMVECTOR point) noexcept;
	bool InspectionSphere(DirectX::XMVECTOR point, float radius) noexcept;
	mydx::eCollisionResult InspectOBBAndPlane(const BoundingBoxData& box );
public:
	void SetPitchYawRoll(DirectX::XMVECTOR vector)noexcept;
	const DirectX::XMVECTOR& GetPitchYawRoll() const noexcept;

	void SetPosition(DirectX::XMVECTOR vector)noexcept;
	const DirectX::XMVECTOR& GetPosition() const noexcept;

private:
	DirectX::XMVECTOR mFrustumPoints[8];
	DirectX::XMFLOAT4 mPlane[6];

	DirectX::XMVECTOR mPositionVector;
	DirectX::XMVECTOR mPitchYawRoll;
	DirectX::XMVECTOR mQuaternionRotation;
	DirectX::XMMATRIX mTransform;
};

}

