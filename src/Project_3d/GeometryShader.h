#pragma once
#include <sstream>
#include "Bindable.h"

class GeometryShader : public Bindable
{
public:
	GeometryShader() =default;
	GeometryShader(GeometryShader& src) = delete;
	GeometryShader& operator=(GeometryShader& src) = delete;

	GeometryShader(Graphics& graphics, const std::wstring& filePath, const std::string& endtryPoint = "main");

	virtual ~GeometryShader() = default;
public:
	virtual void Bind(Graphics& graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept final { return false; }

public:
	ID3DBlob* GetBytecodeBlob();
	const ID3D11GeometryShader* GetVertexShder();
private:
	Microsoft::WRL::ComPtr<ID3DBlob> mBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGeometryShader;
};

