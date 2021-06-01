#include "HeightMap.h"

using namespace DirectX;

mydx::HeightMap::HeightMap(Graphics& graphics, const TCHAR* textureFileName)
	: mTextureDesc({})
{

	if (textureFileName == nullptr)
	{
		textureFileName = L"../../data/etcTexture/desert.jpg";
	}


	auto mVertexShaderClass = std::make_unique<VertexShader>(graphics, L"CustomMap.hlsl", "vertexShaderMain");
	auto bytecodeBlob = mVertexShaderClass->GetBytecodeBlob();

	AddBind(std::move(mVertexShaderClass));



	const std::vector< D3D11_INPUT_ELEMENT_DESC> layoutList =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURECOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	AddBind(std::make_unique<InputLayout>(graphics, layoutList, bytecodeBlob));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<PixelShader>(graphics, L"CustomMap.hlsl", "pixelShaderMain"));

	std::unique_ptr<Texture> texture = std::make_unique<Texture>(graphics, textureFileName);
	mTexture2d = const_cast<ID3D11Texture2D*>(texture.get()->GetTexture());
	mTextureDesc = texture.get()->GetTextureDesc();

	AddBind(std::move(texture));

	AddBind(std::make_unique<TransformConstantBuffer>(graphics, *this));

	//create vertex, index
	mMapDesc.RowCellCount = mTextureDesc.Height;
	mMapDesc.ColCellCount = mTextureDesc.Width;
	mMapDesc.CellDistance = 1u;
	createVertices(mMapDesc.ColCellCount, mMapDesc.RowCellCount, mMapDesc.CellDistance);
	createIndices(mMapDesc.ColCellCount, mMapDesc.RowCellCount);

	ID3D11Texture2D* stagTex = nullptr; // create Staging texture_____________________________________
	D3D11_TEXTURE2D_DESC stagDesc;
	mTexture2d->GetDesc(&stagDesc);
	stagDesc.ArraySize = 1;
	stagDesc.BindFlags = 0;
	stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagDesc.MipLevels = 1;
	stagDesc.MiscFlags = 0;
	stagDesc.Usage = D3D11_USAGE_STAGING;
	graphics.GetDevice()->CreateTexture2D(&stagDesc, nullptr, &stagTex);


	// read from image_________________________________________________________________________
	if (stagTex != nullptr)
	{
		graphics.GetDeviceContext()->CopyResource(static_cast<ID3D11Resource*>(stagTex), static_cast<ID3D11Resource*>(mTexture2d));
	}
	/*  D3D11_MAPPED_SUBRESOURCE mapped;
	  r_assert(graphic->DContext()->Map(stagTex, 0, D3D11_MAP_READ, 0, &mapped));
	  UINT* arr = new UINT[(mapped.RowPitch / (float)sizeof(UINT)) * stagDesc.Height];
	  ZeroMemory(arr, mapped.RowPitch * stagDesc.Height);
	  CopyMemory(arr, mapped.pData, mapped.RowPitch * stagDesc.Height);
	  graphic->DContext()->Unmap(stagTex, 0);*/

	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	if (stagTex != nullptr)
	{

		graphics.GetDeviceContext()->Map(static_cast<ID3D11Resource*>(stagTex), 0u, D3D11_MAP_READ, 0u, &mappedSubresource);

		BYTE* pixels = reinterpret_cast<BYTE*>(mappedSubresource.pData);
		UINT rowIndex = 0;
		UINT colIndex = 0;

		for (UINT row = 0; row < mTextureDesc.Height; row++)
		{
			//mappedSubresource.RowPitch 사용하는 이유는 압축된 텍스쳐일경우 실제 크기와 데이터 크기가 다름.
			rowIndex = row * mappedSubresource.RowPitch;
			for (UINT col = 0; col < mTextureDesc.Width; col++)
			{
				// rgba 4바이트씩 건너띈다.
				colIndex = col * 4u;
				// 저장된 버텍스 크기로 조회, 
				mVertexData[row * mTextureDesc.Width + col].position.y =
					//높이맵 텍스쳐는 rgba 모두 같기 때문에 0~3까지 같다.
					pixels[rowIndex + colIndex + 0] / 10.0f;
			}
		}


		graphics.GetDeviceContext()->Unmap(static_cast<ID3D11Resource*>(stagTex), 0u);

	}

	const UINT numberIncludedInFace = 3;
	UINT vertexIndex[numberIncludedInFace] = {0};
	::XMVECTOR vertex[3] ={};
	::XMVECTOR faceNormal = {};
	::XMFLOAT4 vertexNormal = {};
	for (UINT indicesIndex = 0; indicesIndex < mIndices.size(); indicesIndex += numberIncludedInFace)
	{
		vertexIndex[0] = mIndices[indicesIndex + 0u];
		vertexIndex[1] = mIndices[indicesIndex + 1u];
		vertexIndex[2] = mIndices[indicesIndex + 2u];
		//face = 3 vertex
		vertex[0] = ::XMLoadFloat4(&mVertexData[vertexIndex[0]].position);
		vertex[1] = ::XMLoadFloat4(&mVertexData[vertexIndex[1]].position);
		vertex[2] = ::XMLoadFloat4(&mVertexData[vertexIndex[2]].position);
		
		faceNormal = computeFaceNormal(vertex[0], vertex[1], vertex[2]);
		////facenormal stored
		//::XMStoreFloat4(&mVertexData[vertexIndex[0]].normal, faceNormal);
		//::XMStoreFloat4(&mVertexData[vertexIndex[1]].normal, faceNormal);
		//::XMStoreFloat4(&mVertexData[vertexIndex[2]].normal, faceNormal);
		//vertexNormal stored
		::XMStoreFloat4(&vertexNormal, faceNormal);
		
		mVertexData[vertexIndex[0]].normal.x += vertexNormal.x;
		mVertexData[vertexIndex[0]].normal.y += vertexNormal.y;
		mVertexData[vertexIndex[0]].normal.z += vertexNormal.z;
		mVertexData[vertexIndex[0]].normal.w += vertexNormal.w;

		mVertexData[vertexIndex[1]].normal.x += vertexNormal.x;
		mVertexData[vertexIndex[1]].normal.y += vertexNormal.y;
		mVertexData[vertexIndex[1]].normal.z += vertexNormal.z;
		mVertexData[vertexIndex[1]].normal.w += vertexNormal.w;

		mVertexData[vertexIndex[2]].normal.x += vertexNormal.x;
		mVertexData[vertexIndex[2]].normal.y += vertexNormal.y;
		mVertexData[vertexIndex[2]].normal.z += vertexNormal.z;
		mVertexData[vertexIndex[2]].normal.w += vertexNormal.w;
		
	}

	AddBind(std::make_unique<VertexBuffer>(graphics, mVertexData));

	AddIndexBuffer(std::make_unique<IndexBuffer>(graphics, mIndices));



	stagTex->Release();
	mTexture2d = nullptr;
}
