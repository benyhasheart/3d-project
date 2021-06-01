#include "Map.h"

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

    AddBind(std::make_unique<VertexBuffer>(graphics, mVertexData));

    AddIndexBuffer(std::make_unique<IndexBuffer>(graphics, mIndices));

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

    AddBind(std::make_unique<Texture>(graphics, textureFileName));

    AddBind(std::make_unique<TransformConstantBuffer>(graphics, *this));
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

DirectX::XMVECTOR mydx::Map::computeFaceNormal(DirectX::XMVECTOR vertex0, DirectX::XMVECTOR vertex1, DirectX::XMVECTOR vertex2)
{
    DirectX::XMVECTOR edge0 = vertex1 - vertex0;
    DirectX::XMVECTOR edge1 = vertex2 - vertex0;

    DirectX::XMVECTOR cross = ::XMVector3Cross(edge0, edge1);
    DirectX::XMVECTOR normal = ::XMVector3Normalize(cross);
    
    return normal;
}
