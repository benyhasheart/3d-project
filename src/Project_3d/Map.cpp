#include "Map.h"
#include <DxErr.h>

using namespace DirectX;

mydx::Map::Map()
    :   
    mTransform(DirectX::XMMatrixIdentity()),
    mMapDesc({0})
{
}

mydx::Map::Map(Graphics& graphics, MapDesc& mapDesc, const TCHAR* textureFileName)
    :
    mTransform(DirectX::XMMatrixIdentity()),
    mMapDesc({ 0 })
{
    mMapDesc = mapDesc;
    CreateMap(mapDesc);

    if (textureFileName == nullptr)
    {
        textureFileName = L"../../data/etcTexture/desert.jpg";
    }

    AddBind(std::make_shared<VertexBuffer<VertexData>>(graphics, mVertexData));

    mIndexBuffer = std::make_shared<IndexBuffer>(graphics, mIndices);
    AddIndexBuffer(mIndexBuffer);

    auto mVertexShaderClass = std::make_shared<VertexShader>(graphics, L"CustomMap.hlsl", "vertexShaderMain");
    auto bytecodeBlob = mVertexShaderClass->GetBytecodeBlob();

    AddBind(std::move(mVertexShaderClass));


    const std::vector< D3D11_INPUT_ELEMENT_DESC> layoutList =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "TEXTURECOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    AddBind(std::make_shared<InputLayout>(graphics, layoutList, bytecodeBlob));

    AddBind(std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    AddBind(std::make_shared<PixelShader>(graphics, L"CustomMap.hlsl", "pixelShaderMain"));

    AddBind(std::make_shared<Texture>(graphics, textureFileName));

    AddBind(std::make_shared<TransformConstantBuffer>(graphics, *this));
}

void mydx::Map::CreateMap(MapDesc& mapDesc)
{
    createVertices(mapDesc.ColCellCount, mapDesc.RowCellCount, mapDesc.CellDistance);
    createIndices(mapDesc.ColCellCount, mapDesc.RowCellCount);
    //UINT rowCount = mapDesc.RowCellCount + 1;
    //UINT colCount = mapDesc.ColCellCount + 1;
    //UINT cellCount = rowCount * colCount;
    //// center를 0,0,0 으로 하기 위해
    //float halfRowCellCount = mapDesc.RowCellCount / 2.0f;
    //float halfColCellCount = mapDesc.ColCellCount / 2.0f;

    //mVertexData.reserve(cellCount);

    //for (UINT rowIndex = 0; rowIndex < rowCount; rowIndex++)
    //{
    //    for (UINT colIndex = 0; colIndex < colCount; colIndex++)
    //    {
    //        VertexData vertex = {};

    //        vertex.position.x = (colIndex - halfColCellCount)  * mapDesc.CellDistance;
    //        vertex.position.y = 0.0f;
    //        vertex.position.z = - ((rowIndex - halfRowCellCount) * mapDesc.CellDistance);
    //        vertex.position.w = 1.0f;

    //        vertex.normal = {0.0f, 1.0f, 0.0f, 0.0f};
    //        vertex.color = {1.0f, 0.0f, 0.0f, 1.0f};

    //        vertex.textureCoordinate.x =  colIndex * (1.0f / colCount);
    //        vertex.textureCoordinate.y =  rowIndex * (1.0f / rowCount);

    //        mVertexData.emplace_back(vertex);
    //    }
    //}

    //// cell = 2 face, 1 face = 3 index;
    //const UINT face = 2;
    //const UINT index = 3;
    //const UINT indexCount = mapDesc.RowCellCount * mapDesc.ColCellCount * face * index;

    //mIndices.resize(indexCount);

    //UINT currentIndex = 0;
    //UINT currentVertexIndex = 0;
    //UINT nextVertexIndex = 0;
    //UINT nextRowVertexIndex = 0;
    //UINT rowVertexCount = mapDesc.RowCellCount + 1;
    //UINT colVertexCount = mapDesc.ColCellCount + 1;

    //rowCount = mapDesc.RowCellCount;
    //colCount = mapDesc.ColCellCount;

    //for (UINT rowIndex = 0; rowIndex < colCount; rowIndex++)
    //{
    //    for (UINT colIndex = 0; colIndex < colCount; colIndex++)
    //    {
    //        currentVertexIndex = colVertexCount * rowIndex + colIndex;
    //        nextVertexIndex = currentVertexIndex + 1;
    //        nextRowVertexIndex = colVertexCount * (rowIndex + 1) + colIndex;

    //        // 0 1 2 3 4 5
    //        // 6 7 8 9 10 11
    //        // 016, 176 ...
    //        mIndices[currentIndex ] = currentVertexIndex;
    //        mIndices[currentIndex + 1 ] = nextVertexIndex;
    //        mIndices[currentIndex + 2 ] = nextRowVertexIndex;

    //        mIndices[currentIndex + 3 ] = nextVertexIndex;
    //        mIndices[currentIndex + 4 ] = nextRowVertexIndex + 1;
    //        mIndices[currentIndex + 5 ] = nextRowVertexIndex;
    //        // start index 0,6, 12, 18...
    //        currentIndex += face * index;
    //    }

    //}
    // int a = 1;
}

void mydx::Map::UpdateIndexBuffer(Graphics& graphics, std::vector<DWORD>& indicesTable) noexcept
{
    mIndexBuffer->UpdateIndices(graphics, indicesTable);
}

bool mydx::Map::Initialize(Graphics& graphics) noexcept
{
    return false;
}

bool mydx::Map::Update(Graphics& graphics) noexcept
{
    ResourceUpdate(graphics);
    return false;
}

bool mydx::Map::PreRender(Graphics& graphics) noexcept
{
    ResourceBinding(graphics);
    return false;
}

bool mydx::Map::Render(Graphics& graphics) noexcept
{
    PreRender(graphics);
    Draw(graphics);
    PostRender(graphics);
    return false;
}

bool mydx::Map::PostRender(Graphics& graphics) noexcept
{
    return false;
}

bool mydx::Map::Release() noexcept
{
    return false;
}

DirectX::XMMATRIX mydx::Map::GetTransform() const noexcept
{
    return mTransform;
}

std::vector<mydx::VertexData>& mydx::Map::GetVertexData() noexcept
{
    return mVertexData;
}

mydx::MapDesc& mydx::Map::GetMapDesc() noexcept
{
        return mMapDesc;
}

void mydx::Map::createVertices(UINT width, UINT height, UINT cellDistance)
{
    UINT rowCount = height + 1;
    UINT colCount = width + 1;
    UINT cellCount = rowCount * colCount;
    // center를 0,0,0 으로 하기 위해
    float halfRowCellCount = height / 2.0f;
    float halfColCellCount = width / 2.0f;

    mVertexData.reserve(cellCount);

    for (UINT rowIndex = 0; rowIndex < rowCount; rowIndex++)
    {
        for (UINT colIndex = 0; colIndex < colCount; colIndex++)
        {
            VertexData vertex = {};

            vertex.position.x = (colIndex - halfColCellCount) * cellDistance;
            vertex.position.y = 0.0f;
            vertex.position.z = -((rowIndex - halfRowCellCount) * cellDistance);
            vertex.position.w = 1.0f;

            vertex.normal = { 0.0f, 0.0f, 0.0f, 0.0f };
            vertex.color = { 1.0f, 0.0f, 0.0f, 1.0f };

            vertex.textureCoordinate.x = colIndex * (1.0f / colCount);
            vertex.textureCoordinate.y = rowIndex * (1.0f / rowCount);

            mVertexData.emplace_back(vertex);
        }
    }
}

void mydx::Map::createIndices(UINT width, UINT height)
{
    // cell = 2 face, 1 face = 3 index;
    const UINT face = 2;
    const UINT index = 3;
    const UINT indexCount = height * width * face * index;
    const UINT rowCount = height;
    const UINT colCount = width;

    mIndices.resize(indexCount);

    UINT currentIndex = 0;
    UINT currentVertexIndex = 0;
    UINT nextVertexIndex = 0;
    UINT nextRowVertexIndex = 0;
    UINT rowVertexCount = height + 1;
    UINT colVertexCount = width + 1;

   

    for (UINT rowIndex = 0; rowIndex < rowCount; rowIndex++)
    {
        for (UINT colIndex = 0; colIndex < colCount; colIndex++)
        {
            currentVertexIndex = colVertexCount * rowIndex + colIndex;
            nextVertexIndex = currentVertexIndex + 1;
            nextRowVertexIndex = colVertexCount * (rowIndex + 1) + colIndex;

            // 0 1 2 3 4 5
            // 6 7 8 9 10 11
            // 016, 176 ...
            mIndices[currentIndex] = currentVertexIndex;
            mIndices[currentIndex + 1] = nextVertexIndex;
            mIndices[currentIndex + 2] = nextRowVertexIndex;

            mIndices[currentIndex + 3] = nextVertexIndex;
            mIndices[currentIndex + 4] = nextRowVertexIndex + 1;
            mIndices[currentIndex + 5] = nextRowVertexIndex;
            // start index 0,6, 12, 18...
            currentIndex += face * index;
        }

    }
}

void mydx::Map::createFaceNormal(UINT width, UINT height)
{
    UINT bufferSize = width * height * 2;
    if ( mFaceNormalTabel.size() != bufferSize)
    {
        ::XMVECTOR init = {0.0f,0.0f,0.0f,0.0f};
        mFaceNormalTabel.resize( bufferSize, init);
    }

    const UINT numberIncludedInFace = 3;
    UINT faceNormalIndex = 0; 
    UINT vertexIndex[numberIncludedInFace] = { 0 };
    ::XMVECTOR vertex[3] = {};
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

        mFaceNormalTabel[faceNormalIndex] = faceNormal;
        faceNormalIndex++;

    }



}

void mydx::Map::createVertexNoramlLookupTable()
{
    const UINT oneBlockSizeOfLookupTable = 6u;

    UINT bufferSize = mVertexData.size() * oneBlockSizeOfLookupTable;
    mVertexNormalLookupTabel.resize(bufferSize, -1);

    const UINT numberIncludedInFace = 3u;
    UINT faceCount = mFaceNormalTabel.size();
    for (UINT faceIndex = 0; faceIndex < faceCount; faceIndex++)
    {

        for (UINT indicesIndex = 0; indicesIndex < numberIncludedInFace; indicesIndex++)
        {
            auto vertexIndex = mIndices[faceIndex * numberIncludedInFace + indicesIndex];
          
            for (UINT lookupTableIndex = 0; lookupTableIndex < oneBlockSizeOfLookupTable; lookupTableIndex++)
            {
                //한 정점이 공유할 수 있는 face의 갯수가 최대 6개.
                if (mVertexNormalLookupTabel[vertexIndex * oneBlockSizeOfLookupTable + lookupTableIndex] == -1)
                {
                    mVertexNormalLookupTabel[vertexIndex * oneBlockSizeOfLookupTable + lookupTableIndex] = faceIndex;
                    break;
                }
            }
            
        }
    }
}

void mydx::Map::updateVertexNormal()
{
    ::XMVECTOR averageNormal = {};
    UINT oneblockSizeOfLookupTable = 6u;
    UINT faceNormalIndex = 0u;

    for (UINT vertexIndex = 0; vertexIndex < mVertexData.size(); vertexIndex++)
    {
        for (UINT lookupIndex = 0; lookupIndex < oneblockSizeOfLookupTable; lookupIndex++)
        {
            
            if (mVertexNormalLookupTabel[vertexIndex * oneblockSizeOfLookupTable + lookupIndex] != -1)
            {   
                faceNormalIndex = mVertexNormalLookupTabel[vertexIndex * oneblockSizeOfLookupTable + lookupIndex];
                averageNormal += mFaceNormalTabel[faceNormalIndex];
            }
        }
        averageNormal = ::XMVector3Normalize(averageNormal);
        ::XMStoreFloat4(&mVertexData[vertexIndex].normal, averageNormal);
    }
}

DirectX::XMVECTOR mydx::Map::computeFaceNormal(DirectX::XMVECTOR vertex0, DirectX::XMVECTOR vertex1, DirectX::XMVECTOR vertex2)
{
    DirectX::XMVECTOR edge0 = vertex1 - vertex0;
    DirectX::XMVECTOR edge1 = vertex2 - vertex0;

    DirectX::XMVECTOR cross = ::XMVector3Cross(edge0, edge1);
    DirectX::XMVECTOR normal = ::XMVector3Normalize(cross);
    
    return normal;
}
