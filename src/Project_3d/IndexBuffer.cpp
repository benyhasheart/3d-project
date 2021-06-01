#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& graphics, const std::vector<DWORD> indices)
	:mCount(indices.size())
{

	D3D11_BUFFER_DESC indexDesc;
	indexDesc.ByteWidth = sizeof(DWORD) * indices.size();
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexSubData;
	indexSubData.pSysMem = indices.data();
	graphics.GetDevice()->CreateBuffer(&indexDesc, &indexSubData, mIndexBuffer.GetAddressOf());
}

UINT IndexBuffer::GetCount() const
{
	return mCount;
}

void IndexBuffer::Bind(Graphics& graphics) noexcept
{
	graphics.GetDeviceContext()->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}