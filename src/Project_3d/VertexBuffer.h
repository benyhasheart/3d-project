#pragma once
#include <vector>
#include "Bindable.h"

template<typename E>
class VertexBuffer :public Bindable
{
public:
	
	VertexBuffer(Graphics& graphics, const std::vector<E> vertices)
		:mStride(sizeof(E))
	{
		UINT countVertices = sizeof(vertices) / sizeof(vertices[0]);

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = static_cast<UINT>(sizeof(E) * vertices.size());
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(vertices);

		D3D11_SUBRESOURCE_DATA subData;
		subData.pSysMem = vertices.data();
		graphics.GetDevice()->CreateBuffer(&desc, &subData, mVertexBuffer.GetAddressOf());

		mVertexTable = vertices;
	};
	virtual ~VertexBuffer() = default;

public:
	virtual void Bind(Graphics& graphics) noexcept override
	{
			UINT offset = 0u;
			graphics.GetDeviceContext()->IASetVertexBuffers(0, 1u, mVertexBuffer.GetAddressOf(), &mStride, &offset);
	}
	virtual bool Update(Graphics& graphics) noexcept override final;

public:
	std::vector<E>& GetVertexTable() noexcept;
private:
	UINT mStride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	std::vector<E> mVertexTable;
};

template<typename E>
bool VertexBuffer<E>::Update(Graphics& graphics) noexcept
{

	graphics.GetDeviceContext()->UpdateSubresource(mVertexBuffer.Get(), 0u, nullptr, mVertexTable.data(), 0u, 0u);
	return false;
}

template<typename E>
std::vector<E>& VertexBuffer<E>::GetVertexTable() noexcept
{
	return mVertexTable;
}
