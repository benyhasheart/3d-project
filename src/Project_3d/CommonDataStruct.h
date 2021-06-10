#pragma once
#include <DirectXMath.h>

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
}
