#pragma once
#include <sstream>
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader() = default;
	VertexShader(Graphics& graphics, const std::wstring& filePath, const std::string& endtryPoint = "main" );

	VertexShader(Graphics& graphics) = delete;
	VertexShader& operator= (Graphics& graphics ) = delete;
	
	virtual ~VertexShader() = default;
public:
	virtual void Bind(Graphics& graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept final { return false; }
public:
	ID3DBlob* GetBytecodeBlob();
	const ID3D11VertexShader* GetVertexShder();
private:
	Microsoft::WRL::ComPtr<ID3DBlob> mBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
};

