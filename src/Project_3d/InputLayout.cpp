#include "InputLayout.h"


InputLayout::InputLayout(Graphics& graphics, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputLayoutList, ID3DBlob* blob)
{
	HRESULT hr = S_OK;

	graphics.GetDevice()->CreateInputLayout(
		inputLayoutList.data(), 
		static_cast<UINT>(inputLayoutList.size()),
		blob->GetBufferPointer(), 
		blob->GetBufferSize(),
		mInputLayout.GetAddressOf());
}

void InputLayout::Bind(Graphics& graphics) noexcept
{
	graphics.GetDeviceContext()->IASetInputLayout(mInputLayout.Get());
}