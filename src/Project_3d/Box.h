#pragma once
#include "Drawable.h"
#include "BindableBase.h"
#include "BoundingBox.h"

class Box : public Drawable
{
public:
	Box() = default;
	Box(Graphics& graphics);
	Box(Graphics& graphics, DirectX::XMVECTOR scale, DirectX::XMVECTOR pitchYawRoll, DirectX::XMVECTOR translate);
	virtual ~Box() = default; 
public:
	virtual DirectX::XMMATRIX GetTransform() const noexcept override final;

	virtual bool Initialize(Graphics& graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept override final;
	virtual bool PreRender(Graphics& graphics) noexcept override final;
	virtual bool Render(Graphics& graphics) noexcept override final;
	virtual bool PostRender(Graphics& graphics) noexcept override final;
	virtual bool Release() noexcept override final {return false;}

public:
	//temporary getter setter
	void SetScale(DirectX::XMVECTOR scaleVector) noexcept ;
	const DirectX::XMVECTOR GetScale() const noexcept;

	void SetRotation(DirectX::XMVECTOR rotationVector) noexcept;
	const DirectX::XMVECTOR GetRotation() const noexcept;

	void SetTranslation(DirectX::XMVECTOR translationVector) noexcept;
	const DirectX::XMVECTOR GetTranslation() const noexcept;

	const mydx::BoundingBox& GetBoundingBox() const noexcept;


private:
	DirectX::XMMATRIX mTransform;

	//temporary
	DirectX::XMVECTOR mScaleVector = {1.0f, 1.0f, 1.0f};
	DirectX::XMVECTOR mRotationVector = {};
	DirectX::XMVECTOR mTranslationVector = {};

	DirectX::XMVECTOR mPositionVector;
	DirectX::XMVECTOR mPitchYawRoll;

	DirectX::XMVECTOR mQuaternionRotation;

	mydx::BoundingBox mBoundingBox;
};

