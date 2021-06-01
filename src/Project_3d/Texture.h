#pragma once
#include <tchar.h>
#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture() = default;
	//Texture(Texture& src) = delete;
	//Texture& operator=(Texture& src) = delete;

	Texture(Graphics& graphics, const TCHAR* fileName);
	Texture(Graphics& graphics, const TCHAR* fileName, const D3D11_USAGE usage);
	virtual ~Texture() = default;

public:
	virtual void Bind(Graphics& graphics)noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept final { return false; }

public:
	const ID3D11Texture2D* GetTexture() noexcept;
	const D3D11_TEXTURE2D_DESC& GetTextureDesc() noexcept;
	const ID3D11ShaderResourceView* GetShaderResourceView() noexcept;
	const D3D11_SHADER_RESOURCE_VIEW_DESC& GetShaderResourceViewDesc() noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture2D;
	D3D11_TEXTURE2D_DESC mTexture2dDesc;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
	D3D11_SHADER_RESOURCE_VIEW_DESC mShaderResourceViewDesc;
		
};

