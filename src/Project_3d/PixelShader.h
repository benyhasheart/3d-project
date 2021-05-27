#pragma once
#include <sstream>
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader() = default;
	PixelShader(Graphics & graphics, const std::wstring & filePath, const std::string & endtryPoint = "main");

	PixelShader(Graphics & graphics) = delete;
	PixelShader& operator= (Graphics & graphics) = delete;

	virtual ~PixelShader() = default;

public:
	virtual void Bind(Graphics & graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept final { return false; }

public:
	ID3DBlob* GetBytecodeBlob();
	const ID3D11PixelShader* GetPixelShader();
private:
	Microsoft::WRL::ComPtr<ID3DBlob> mBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
};

