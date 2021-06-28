#include "Terrain.h"

mydx::Terrain::Terrain(Graphics& graphics, MapDesc& mapDesc, std::shared_ptr<Texture> material, std::shared_ptr<Texture> heightMap)
	:
	mMaterial(nullptr),
	mHeightMapTexture(nullptr)
{

	auto mVertexShaderClass = std::make_shared<VertexShader>(graphics, L"\CustomMap.hlsl", "vertexShaderMain");
	auto bytecodeBlob = mVertexShaderClass->GetBytecodeBlob();

	AddBind(std::move(mVertexShaderClass));



	const std::vector< D3D11_INPUT_ELEMENT_DESC> layoutList =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXTURECOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	AddBind(std::make_shared<InputLayout>(graphics, layoutList, bytecodeBlob));

	AddBind(std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<PixelShader>(graphics, L"\CustomMap.hlsl", "pixelShaderMain"));

	AddBind(std::make_shared<TransformConstantBuffer>(graphics, *this));

	mMaterial = material;

	if (mMaterial != nullptr)
		AddBind(mMaterial);

	mHeightMapTexture = heightMap;
	mMapDesc = mapDesc;

	if (mHeightMapTexture != nullptr)
	{
		createVertexUsingHeightMap(	graphics,
									mMapDesc,
									mVertexData, mHeightMapTexture.get());
		createIndices(mMapDesc.ColCellCount, mMapDesc.RowCellCount);
	}
	else
	{
		createVertices(mMapDesc.ColCellCount, mMapDesc.RowCellCount, mMapDesc.CellDistance);
		createIndices(mMapDesc.ColCellCount, mMapDesc.RowCellCount);
	}
	


	createFaceNormal(mMapDesc.ColCellCount, mMapDesc.RowCellCount);
	createVertexNoramlLookupTable();
	updateVertexNormal();

	mVertexBuffer = std::make_shared<VertexBuffer<VertexData>>(graphics, mVertexData);
	AddBind(mVertexBuffer);

	mIndexBuffer = std::make_shared<IndexBuffer>(graphics, mIndices);
	AddIndexBuffer(mIndexBuffer);




}

void mydx::Terrain::createVertexUsingHeightMap(Graphics& graphics, mydx::MapDesc& mapDesc, std::vector<VertexData>& vertexTable, Texture* heightMapTexture)
{
	
	ID3D11Texture2D* stagTex = nullptr; // create Staging texture_____________________________________
	D3D11_TEXTURE2D_DESC stagDesc;
	stagDesc = heightMapTexture->GetTextureDesc();
	stagDesc.ArraySize = 1;
	stagDesc.BindFlags = 0;
	stagDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	stagDesc.MipLevels = 1;
	stagDesc.MiscFlags = 0;
	stagDesc.Usage = D3D11_USAGE_STAGING;
	graphics.GetDevice()->CreateTexture2D(&stagDesc, nullptr, &stagTex);

	auto& textureDesc = heightMapTexture->GetTextureDesc();
	mapDesc.ColCellCount = textureDesc.Width;
	mapDesc.RowCellCount = textureDesc.Height;

	createVertices(mapDesc.ColCellCount, mapDesc.RowCellCount, mapDesc.CellDistance);

	// read from image_________________________________________________________________________
	if (stagTex != nullptr)
	{
		graphics.GetDeviceContext()->CopyResource(static_cast<ID3D11Resource*>(stagTex), static_cast<ID3D11Resource*>(const_cast<ID3D11Texture2D*>(heightMapTexture->GetTexture())));
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
		
		
		for (UINT row = 0; row < textureDesc.Height; row++)
		{
			//mappedSubresource.RowPitch ����ϴ� ������ ����� �ؽ����ϰ�� ���� ũ��� ������ ũ�Ⱑ �ٸ�.
			rowIndex = row * mappedSubresource.RowPitch;
			for (UINT col = 0; col < textureDesc.Width; col++)
			{
				// rgba 4����Ʈ�� �ǳʶ��.
				colIndex = col * 4u;
				// ����� ���ؽ� ũ��� ��ȸ, 
				vertexTable[row * textureDesc.Width + col].position.y =
					//���̸� �ؽ��Ĵ� rgba ��� ���� ������ 0~3���� ����.
					pixels[rowIndex + colIndex + 0] / 10.0f;
			}
		}


		graphics.GetDeviceContext()->Unmap(static_cast<ID3D11Resource*>(stagTex), 0u);

	}

	stagTex->Release();
}


