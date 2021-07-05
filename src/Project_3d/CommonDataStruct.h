#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>

namespace mydx
{
	struct VertexData
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT4 normal;
		DirectX::XMFLOAT4 textureCoordinate;
	};

	struct BoundingBoxData
	{
		//AABB condition
		DirectX::XMVECTOR Min;
		DirectX::XMVECTOR Max;
		DirectX::XMVECTOR Center;
		//OBB condition
		//	DirectX::XMFLOAT3 Center;
		DirectX::XMVECTOR Axis[3];
		DirectX::XMVECTOR Extent[3];
	};

	struct MapDesc
	{
		unsigned int RowCellCount;
		unsigned int ColCellCount;
		DirectX::XMVECTOR position;
		DirectX::XMVECTOR scale;
	};

	struct TerrainInfo
	{
		std::string name;
		std::string textureName;
		std::string heightMapName;
		MapDesc mapDesc;
		std::vector<VertexData> vertexTable ;
		std::vector<unsigned long>	indexTable ;
	};
}
