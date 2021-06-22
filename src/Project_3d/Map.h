#pragma once
#include <tchar.h>

#include "CommonDataStruct.h"
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
	
	class Map : public Drawable
	{
	public:
		Map();
		virtual ~Map()= default;

		Map(Graphics& graphics, MapDesc& mapDesc, const TCHAR* textureFileName );

	public:
		void CreateMap(MapDesc& mapDesc);
		void UpdateIndexBuffer(Graphics& graphics, std::vector<DWORD>& indicesTable) noexcept;
	public:
		bool Initialize(Graphics& graphics) noexcept override final;
		bool Update(Graphics& graphics) noexcept override final;
		bool PreRender(Graphics& graphics) noexcept override final;
		bool Render(Graphics& graphics) noexcept override final;
		bool PostRender(Graphics& graphics) noexcept override final;
		bool Release() noexcept override final;

		DirectX::XMMATRIX GetTransform() const noexcept override final;

	public:
		std::vector<VertexData>& GetVertexData() noexcept;
		MapDesc& GetMapDesc() noexcept;

		std::shared_ptr<VertexBuffer<mydx::VertexData>>& GetVertexBuffer() noexcept;
		std::shared_ptr<IndexBuffer>& GetIndexBuffer() noexcept;
	protected:
		void createVertices(UINT width, UINT height, UINT cellDistance);
		void createIndices(UINT width, UINT height);
		void createFaceNormal(UINT width, UINT height);
		void createVertexNoramlLookupTable();
		void updateVertexNormal();
		DirectX::XMVECTOR computeFaceNormal(DirectX::XMVECTOR vertex0, DirectX::XMVECTOR vertex1, DirectX::XMVECTOR vertex2);

	protected:
		std::shared_ptr<VertexBuffer<VertexData>> mVertexBuffer;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
		std::vector<VertexData> mVertexData;
		std::vector<DWORD>	mIndices;
		std::vector<DirectX::XMVECTOR> mFaceNormalTabel;
		std::vector<int> mVertexNormalLookupTabel;
		MapDesc mMapDesc;


		
		DirectX::XMMATRIX mTransform;
	};
}


