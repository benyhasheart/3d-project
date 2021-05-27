#pragma once
#include <vector> 
#include "Bindable.h"
class InputLayout : public Bindable
{
public:
	InputLayout() = default;
	InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutList, ID3DBlob* blob );
	InputLayout(InputLayout& src) = delete;
	InputLayout& operator=(InputLayout& src) = delete;

	virtual ~InputLayout() = default;

public:
	virtual void Bind(Graphics& graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept final { return false; }

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
};

