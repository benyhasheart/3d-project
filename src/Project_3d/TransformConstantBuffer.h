#pragma once

#include "Drawable.h"
#include "VertexConstantBuffer.h"


class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& graphics, const Drawable& owner);
public:
	void Bind(Graphics& graphics)noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept final; 
private:
	const Drawable& mOwner;
	std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> mConstantBuffer;
};

