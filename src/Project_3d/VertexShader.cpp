#include "VertexShader.h"

#pragma comment(lib, "d3dcompiler.lib")

VertexShader::VertexShader(Graphics& graphics, const std::wstring& filePath, const std::string& endtryPoint )
{
	HRESULT hr = S_OK;
	ID3DBlob* errorMsg = nullptr;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( _DEBUG )
	shaderFlags = D3DCOMPILE_DEBUG;
#endif
	
	// vertex shader
	hr = D3DCompileFromFile(filePath.c_str(), NULL, NULL, endtryPoint.c_str(), "vs_5_0", shaderFlags, NULL, mBytecodeBlob.GetAddressOf(), &errorMsg);
	graphics.GetDevice()->CreateVertexShader(mBytecodeBlob->GetBufferPointer(),
												mBytecodeBlob->GetBufferSize(),
												nullptr, mVertexShader.GetAddressOf());
}

void VertexShader::Bind(Graphics& graphics) noexcept
{
	graphics.GetDeviceContext()->VSSetShader(mVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecodeBlob()
{
	return mBytecodeBlob.Get();
}

const ID3D11VertexShader* VertexShader::GetVertexShder()
{
	return mVertexShader.Get();
}