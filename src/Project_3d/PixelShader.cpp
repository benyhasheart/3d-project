#include "PixelShader.h"

#pragma comment(lib, "d3dcompiler.lib")

PixelShader::PixelShader(Graphics& graphics, const std::wstring& filePath, const std::string& endtryPoint)
{
	HRESULT hr = S_OK;
	ID3DBlob* errorMsg = nullptr;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( _DEBUG )
	shaderFlags = D3DCOMPILE_DEBUG;
#endif

	hr = D3DCompileFromFile(filePath.c_str(), NULL, NULL, endtryPoint.c_str(), "ps_5_0", shaderFlags, NULL, mBytecodeBlob.GetAddressOf(), &errorMsg);
	graphics.GetDevice()->CreatePixelShader(mBytecodeBlob->GetBufferPointer(),
		mBytecodeBlob->GetBufferSize(),
		nullptr, mPixelShader.GetAddressOf());
}

void PixelShader::Bind(Graphics& graphics) noexcept
{
	graphics.GetDeviceContext()->PSSetShader(mPixelShader.Get(), nullptr, 0u);
}

ID3DBlob* PixelShader::GetBytecodeBlob()
{
	return mBytecodeBlob.Get();
}

const ID3D11PixelShader* PixelShader::GetPixelShader()
{
	return mPixelShader.Get();
}