#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& graphics)
	{
		D3D11_BUFFER_DESC constnatDesc;
		constnatDesc.ByteWidth = sizeof(C);
		constnatDesc.Usage = D3D11_USAGE_DYNAMIC; // use cpu Access.
		constnatDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constnatDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // use cpu Access.
		constnatDesc.MiscFlags = 0;

		graphics.GetDevice()->CreateBuffer(&constnatDesc, nullptr, mConstantBuffer.GetAddressOf());
	};
	ConstantBuffer(Graphics& graphics, const C& constantData)
	{
		D3D11_BUFFER_DESC constnatDesc;
		constnatDesc.ByteWidth = sizeof(constantData);
		constnatDesc.Usage = D3D11_USAGE_DYNAMIC; // use cpu Access.
		constnatDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constnatDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // use cpu Access.
		constnatDesc.MiscFlags = 0;
		constnatDesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &constantData;
		graphics.GetDevice()->CreateBuffer(&constnatDesc, &csd, mConstantBuffer.GetAddressOf());
	};
	//use updataSubresource
	ConstantBuffer(Graphics& graphics, const C& constantData, unsigned int usage /* 0 ~ 4*/)
	{
		D3D11_BUFFER_DESC constnatDesc;
		constnatDesc.ByteWidth = sizeof(constantData);
		constnatDesc.Usage = static_cast<D3D11_USAGE>(usage); // use cpu Access.
		if (usage == D3D11_USAGE_DEFAULT)
		{
			constnatDesc.CPUAccessFlags = 0; // use cpu Access.
		}
		else
		{
			constnatDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // use cpu Access.
		}
		constnatDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constnatDesc.MiscFlags = 0;
		constnatDesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &constantData;
		graphics.GetDevice()->CreateBuffer(&constnatDesc, &csd, mConstantBuffer.GetAddressOf());
	};
	virtual ~ConstantBuffer() = default;

public:
	virtual bool Update(Graphics& graphics) noexcept {return false;}

protected:
	virtual void Update(Graphics& graphics, const C& constantData)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		graphics.GetDeviceContext()->Map(mConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

		const auto destConstantData = reinterpret_cast<C*>(mappedSubresource.pData);
		memcpy_s(destConstantData, sizeof(destConstantData),reinterpret_cast<const void*>(&constantData),sizeof(constantData));

		graphics.GetDeviceContext()->Unmap(mConstantBuffer.Get(), 0);
	};

public:
	ID3D11Buffer* GetBuffer() {return mConstantBuffer.Get(); }
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;
};
