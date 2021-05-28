#pragma once
#include <tchar.h>

#include "Drawable.h"
#include "BindableBase.h"

namespace mydx
{
	struct MapDesc
	{
		UINT RowCellCount;
		UINT ColCellCount;
		float CellDistance;
	};

	struct VertexData
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT4 textureCoordinate;
	};
	
	class Map : public Drawable
	{
	public:
		Map() = default;
		virtual ~Map()= default;

		Map(Graphics& graphics, MapDesc& mapDesc, const TCHAR* textureFileName );

	public:
		void CreateMap(MapDesc& mapDesc);
	public:
		bool Initialize(Graphics& graphics) noexcept override final;
		bool Update(Graphics& graphics) noexcept override final;
		bool PreRender(Graphics& graphics) noexcept override final;
		bool Render(Graphics& graphics) noexcept override final;
		bool PostRender(Graphics& graphics) noexcept override final;
		bool Release() noexcept override final;

		DirectX::XMMATRIX GetTransform() const noexcept override final;

	private:
		std::shared_ptr<VertexBuffer> mVertexBuffer;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
		std::vector<VertexData> mVertexData;
		std::vector<WORD>	mIndices;

		DirectX::XMMATRIX mTransform;
	};
}


