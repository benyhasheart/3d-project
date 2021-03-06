#pragma once
#include <vector>
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& graphics, const std::vector<DWORD> indices);
	virtual ~IndexBuffer() = default;

public:
	void Bind(Graphics& graphics) noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept final { return false; }
	
	void UpdateIndices(Graphics& graphics, std::vector<DWORD>& indicesTable) noexcept;

public:
	UINT GetCount() const ;
	std::vector<DWORD>& GetIndexTable() noexcept;
private:
	UINT mCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	std::vector<DWORD> mIndexTable;
};

