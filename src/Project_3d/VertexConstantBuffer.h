#pragma once
#include "ConstantBuffer.h"

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::mConstantBuffer;
public:
	using ConstantBuffer<C>::ConstantBuffer;

public:
	void Bind(Graphics& graphics) noexcept override
	{
		graphics.GetDeviceContext()->VSSetConstantBuffers(0u, 1u, mConstantBuffer.GetAddressOf());
	}
	virtual bool Update(Graphics& graphics) noexcept final { return false; }

	virtual void Update(Graphics& graphics, const C& constantData) override
	{
		graphics.GetDeviceContext()->UpdateSubresource(mConstantBuffer.Get(), 0, nullptr, &constantData, 0, 0);
	}
};

