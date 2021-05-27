#include "GeometryShader.h"

GeometryShader::GeometryShader(Graphics& graphics, const std::wstring& filePath, const std::string& endtryPoint )
{
	HRESULT hr = S_OK;
	ID3DBlob* errorMsg = nullptr;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( _DEBUG )
	shaderFlags = D3DCOMPILE_DEBUG;
#endif

	// vertex shader
	hr = D3DCompileFromFile(filePath.c_str(), NULL, NULL, endtryPoint.c_str(), "gs_5_0", shaderFlags, NULL, mBytecodeBlob.GetAddressOf(), &errorMsg);
	graphics.GetDevice()->CreateGeometryShader(mBytecodeBlob->GetBufferPointer(),
		mBytecodeBlob->GetBufferSize(),
		nullptr, mGeometryShader.GetAddressOf());
}

void GeometryShader::Bind(Graphics& graphics) noexcept
{
	graphics.GetDeviceContext()->GSSetShader(mGeometryShader.Get(), nullptr, 0u);
}

ID3DBlob* GeometryShader::GetBytecodeBlob()
{
	return mBytecodeBlob.Get();
}

const ID3D11GeometryShader* GeometryShader::GetVertexShder()
{
	return mGeometryShader.Get();
}