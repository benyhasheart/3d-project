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
		Map();
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

	protected:
		void createVertices(UINT width, UINT height, UINT cellDistance);
		void createIndices(UINT width, UINT height);
		DirectX::XMVECTOR computeFaceNormal(DirectX::XMVECTOR vertex0, DirectX::XMVECTOR vertex1, DirectX::XMVECTOR vertex2);

	protected:
		std::shared_ptr<VertexBuffer> mVertexBuffer;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
		std::vector<VertexData> mVertexData;
		std::vector<DWORD>	mIndices;
		MapDesc mMapDesc;
		
		DirectX::XMMATRIX mTransform;
	};
}


