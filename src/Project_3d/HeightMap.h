#pragma once
#include "Map.h"

namespace mydx
{
	class HeightMap : public Map
	{
	public:
		HeightMap() = default;
		virtual ~HeightMap() = default;

		HeightMap(Graphics& graphics, const TCHAR* textureFileName);

	public:
		//void CreateHeightMap();

	private:
		ID3D11Texture2D* mTexture2d;
		D3D11_TEXTURE2D_DESC mTextureDesc;
	};
}


