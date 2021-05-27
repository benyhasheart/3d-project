#include "Texture.h"
#include "WICTextureLoader.h"

#pragma comment(lib, "DirectXTK.lib")

Texture::Texture(Graphics& graphics, const TCHAR* fileName)
{
	HRESULT hr = S_OK;
	ID3D11Resource* resource;

	hr = DirectX::CreateWICTextureFromFile(	graphics.GetDevice(),
											fileName,
											&resource, 
											mShaderResourceView.GetAddressOf(), 0u);

	resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mTexture2D.GetAddressOf()));
	resource->Release();

	mTexture2D->GetDesc(&mTexture2dDesc);

	mShaderResourceView->GetDesc(&mShaderResourceViewDesc);
}

void Texture::Bind(Graphics& graphics) noexcept
{
	graphics.GetDeviceContext()->PSSetShaderResources(0u,1u, mShaderResourceView.GetAddressOf());
}

const ID3D11Texture2D* Texture::GetTexture() noexcept
{
	return mTexture2D.Get();
}

const D3D11_TEXTURE2D_DESC& Texture::GetTextureDesc() noexcept
{
	return mTexture2dDesc;
}

const ID3D11ShaderResourceView* Texture::GetShaderResourceView() noexcept
{
	return mShaderResourceView.Get();
}

const D3D11_SHADER_RESOURCE_VIEW_DESC& Texture::GetShaderResourceViewDesc() noexcept
{
	return mShaderResourceViewDesc;
}
