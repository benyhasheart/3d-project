#pragma once
#include "Map.h"
#include "Texture.h"
namespace mydx
{
	class Terrain : public Map
	{
	public:
		Terrain(Graphics& graphics, MapDesc& mapDesc, std::shared_ptr<Texture> material = nullptr, std::shared_ptr<Texture> heightMap = nullptr);
		virtual ~Terrain() = default;

	private:
		void createVertexUsingHeightMap(Graphics& graphics, 
										mydx::MapDesc& mapDesc, 
										std::vector<VertexData>& vertexTable, 
										Texture* heightMapTexture);
	private:

		std::shared_ptr<Texture> mMaterial;
		std::shared_ptr<Texture> mHeightMapTexture;
	};
}


