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

	mTexture2D->GetDesc(&mTexture2dDesc);

	mShaderResourceView->GetDesc(&mShaderResourceViewDesc);
}

Texture::Texture(Graphics& graphics, const TCHAR* fileName, const D3D11_USAGE usage)
{
	HRESULT hr = S_OK;
	ID3D11Resource* resource;

	/*hr = DirectX::CreateWICTextureFromFile(graphics.GetDevice(),
		fileName,
		&resource,
		mShaderResourceView.GetAddressOf(), 0u);*/
	
	hr = DirectX::CreateWICTextureFromFileEx(graphics.GetDevice(),
			fileName,
			0u,
			usage,
			D3D11_BIND_SHADER_RESOURCE,
			D3D11_CPU_ACCESS_READ,
			0,
			DirectX::WIC_LOADER_DEFAULT,
			&resource,
			mShaderResourceView.GetAddressOf());

	resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mTexture2D.GetAddressOf()));

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
