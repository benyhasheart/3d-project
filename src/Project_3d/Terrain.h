#pragma once
#include "Map.h"
#include "Texture.h"
#include "QuardTreeVertexIndex.h"

namespace mydx
{
	class Terrain : public Map
	{
	public:
		Terrain(Graphics& graphics, MapDesc& mapDesc, std::shared_ptr<Texture> material = nullptr, std::shared_ptr<Texture> heightMap = nullptr);
		//build terrain with load data
		Terrain(Graphics& graphics, mydx::TerrainInfo& terrainInfo);
		virtual ~Terrain() = default;

	public:
		mydx::Node* FindSelectedNode(Node* node, Select* selectClass) noexcept;
		int UpdateIndexTable(DWORD currentIndex, DWORD topLeft, DWORD topRight, DWORD bottomLeft, DWORD bottomRight, std::vector<DWORD>& indexTable) noexcept;

	public:
		bool Initialize(Graphics& graphics) noexcept override final;
		bool Update(Graphics& graphics) noexcept override final;

	public:
		std::shared_ptr<Texture>& GetMaterial() noexcept;
		std::shared_ptr<Texture>& GetHeightMap() noexcept;

	protected:
		virtual void createVertices(UINT width, UINT height, DirectX::XMVECTOR scale) noexcept   override  final;
	private:
		void createVertexUsingHeightMap(Graphics& graphics, 
										mydx::MapDesc& mapDesc, 
										std::vector<VertexData>& vertexTable, 
										Texture* heightMapTexture);
	private:

		std::shared_ptr<Texture> mMaterial;
		std::shared_ptr<Texture> mHeightMapTexture;
		
		std::shared_ptr<QuardTreeVertexIndex> mQuardTree;
	};
}


