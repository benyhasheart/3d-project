#include "TransformConstantBuffer.h"

using namespace DirectX;


TransformConstantBuffer::TransformConstantBuffer(Graphics& graphics, const Drawable& owner)
	:
	mOwner(owner)
{
	
	if (mConstantBuffer == nullptr)
	{
		mConstantBuffer = std::make_unique<VertexConstantBuffer<::XMMATRIX>>(graphics, XMMATRIX(), D3D11_USAGE_DEFAULT);
	}
}

void TransformConstantBuffer::Bind(Graphics& graphics) noexcept
{
	
	mConstantBuffer->Bind(graphics);
}

bool TransformConstantBuffer::Update(Graphics& graphics) noexcept
{
	XMMATRIX ownerTransform =
		DirectX::XMMatrixTranspose(mOwner.GetTransform() * graphics.GetViewMatrix() * graphics.GetProjectionMatrix());

	mConstantBuffer->Update(graphics, ownerTransform);
	return false;
}
