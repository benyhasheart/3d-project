#pragma once

#include "CommonDataStruct.h"
#include "Drawable.h"
#include "BindableBase.h"


namespace mydx
{

class BoundingBox : public Drawable
{
public:
	BoundingBox(Graphics& graphics, DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extens);
	BoundingBox();
	virtual ~BoundingBox()= default;

	BoundingBox(BoundingBox& src) = delete;
	BoundingBox& operator = (BoundingBox& src) = delete;

public:
	virtual DirectX::XMMATRIX GetTransform() const noexcept override final;

	virtual bool Initialize(Graphics& graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept override;
	virtual bool PreRender(Graphics& graphics) noexcept override final;
	virtual bool Render(Graphics& graphics) noexcept override final;
	virtual bool PostRender(Graphics& graphics) noexcept override final;
	virtual bool Release() noexcept override final { return false; }

public:
	void CreateBoundingBox(DirectX::XMVECTOR min, DirectX::XMVECTOR max, DirectX::XMVECTOR center, DirectX::XMMATRIX transform) noexcept;

public:
	const BoundingBoxData& GetBoundingBoxData() const noexcept;
	void SetBoundingBoxData(BoundingBoxData& data) noexcept;

private:

private:
	DirectX::XMFLOAT3 mCenter;
	DirectX::XMFLOAT3 mExtens;
	DirectX::XMMATRIX mTransform;

	BoundingBoxData mBoundingBoxData;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerStateWireFrame;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerStateSolid;
};

}


