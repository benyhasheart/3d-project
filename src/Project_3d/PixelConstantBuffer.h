#pragma once
#include "ConstantBuffer.h"

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
private:
	using ConstantBuffer<C>::mConstantBuffer;

public:
	using ConstantBuffer<C>::ConstantBuffer;

public:
	virtual void Bind(Graphics& graphics) noexcept override
	{
		graphics.GetDeviceContext()->PSSetConstantBuffers(0u, 1u, mConstantBuffer.GetAddressOf());
	}
	virtual bool Update(Graphics& graphics) noexcept final { return false; }
};

