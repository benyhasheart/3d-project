#pragma once
#include <vector>
#include "Bindable.h"

class VertexBuffer :public Bindable
{
public:
	template<typename E>
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
	};
	virtual ~VertexBuffer() = default;

public:
	virtual void Bind(Graphics& graphics) noexcept override;
	virtual bool Update(Graphics& graphics) noexcept final { return false; }
private:
	UINT mStride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
};
