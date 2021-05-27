#include <memory>
#include "Frustum.h"
#include "BoundingBox.h"

using namespace DirectX;
mydx::Frustum::Frustum()
    :
    mPositionVector(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
    mPitchYawRoll(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
    mQuaternionRotation(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
    mTransform(::XMMatrixIdentity())
{
    // NDC space points
    mFrustumPoints[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
    mFrustumPoints[1] = { 1.0f,  -1.0f, 0.0f, 1.0f };
    mFrustumPoints[2] = { -1.0f, 1.0f, 0.0f, 1.0f };
    mFrustumPoints[3] = { 1.0f,  1.0f, 0.0f, 1.0f };
    mFrustumPoints[4] = { -1.0f, -1.0f, 1.0f, 1.0f };
    mFrustumPoints[5] = { 1.0f,  -1.0f, 1.0f, 1.0f };
    mFrustumPoints[6] = { -1.0f, 1.0f, 1.0f, 1.0f };
    mFrustumPoints[7] = { 1.0f,  1.0f, 1.0f, 1.0f };
    std::fill_n(mPlane, 6, ::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

}
mydx::Frustum::Frustum(Graphics& graphics)
{
    // NDC space points
    mFrustumPoints[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
    mFrustumPoints[1] = { 1.0f,  -1.0f, 0.0f, 1.0f };
    mFrustumPoints[2] = { -1.0f, 1.0f, 0.0f, 1.0f };
    mFrustumPoints[3] = { 1.0f,  1.0f, 0.0f, 1.0f };
    mFrustumPoints[4] = { -1.0f, -1.0f, 1.0f, 1.0f };
    mFrustumPoints[5] = { 1.0f,  -1.0f, 1.0f, 1.0f };
    mFrustumPoints[6] = { -1.0f, 1.0f, 1.0f, 1.0f };
    mFrustumPoints[7] = { 1.0f,  1.0f, 1.0f, 1.0f };
    std::fill_n(mPlane, 6, ::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f));

}



bool mydx::Frustum::CreateFrustum(DirectX::XMMATRIX& viewMatrix, DirectX::XMMATRIX& projectionMatrix) noexcept
{
    ::XMMATRIX inverseMatrix = ::XMMatrixIdentity();
    ::XMMATRIX combineMatrix = ::XMMatrixIdentity();
    ::XMVECTOR determinant = {0};

    // ( viewMatrix * projectionMatrix ) inverse
    combineMatrix = ::XMMatrixMultiply(viewMatrix, projectionMatrix);
    determinant = ::XMMatrixDeterminant(combineMatrix);
    inverseMatrix = ::XMMatrixInverse(&determinant, combineMatrix);

    mFrustumPoints[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
    mFrustumPoints[1] = { 1.0f,  -1.0f, 0.0f, 1.0f };
    mFrustumPoints[2] = { -1.0f, 1.0f, 0.0f, 1.0f };
    mFrustumPoints[3] = { 1.0f,  1.0f, 0.0f, 1.0f };
    mFrustumPoints[4] = { -1.0f, -1.0f, 1.0f, 1.0f };
    mFrustumPoints[5] = { 1.0f,  -1.0f, 1.0f, 1.0f };
    mFrustumPoints[6] = { -1.0f, 1.0f, 1.0f, 1.0f };
    mFrustumPoints[7] = { 1.0f,  1.0f, 1.0f, 1.0f };
    // local * world * view * projection -> local * world
    for( int i = 0 ; i < 8; i++)
    {
        mFrustumPoints[i] = ::XMVector3TransformCoord(mFrustumPoints[i], inverseMatrix);
    }
    /*mFrustumPoints[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
    mFrustumPoints[1] = { 1.0f,  -1.0f, 0.0f, 1.0f };
    mFrustumPoints[2] = { -1.0f, 1.0f, 0.0f, 1.0f };
    mFrustumPoints[3] = { 1.0f,  1.0f, 0.0f, 1.0f };
    mFrustumPoints[4] = { -1.0f, -1.0f, 1.0f, 1.0f };
    mFrustumPoints[5] = { 1.0f,  -1.0f, 1.0f, 1.0f };
    mFrustumPoints[6] = { -1.0f, 1.0f, 1.0f, 1.0f };
    mFrustumPoints[7] = { 1.0f,  1.0f, 1.0f, 1.0f };*/

    //마주보는 앞면
    mPlane[0] = CreatePlane(mFrustumPoints[0], mFrustumPoints[3], mFrustumPoints[2]);
    //뒷면
    mPlane[1] = CreatePlane(mFrustumPoints[4], mFrustumPoints[6], mFrustumPoints[7]);
    //좌
    mPlane[2] =CreatePlane(mFrustumPoints[4], mFrustumPoints[2], mFrustumPoints[6]);
    //우
    mPlane[3] = CreatePlane(mFrustumPoints[5], mFrustumPoints[7], mFrustumPoints[3]);
    //위
    mPlane[4] = CreatePlane(mFrustumPoints[2], mFrustumPoints[7], mFrustumPoints[6]);
    //아래
    mPlane[5] = CreatePlane(mFrustumPoints[0], mFrustumPoints[4], mFrustumPoints[5]);

    return false;
}

DirectX::XMFLOAT4 mydx::Frustum::CreatePlane(DirectX::XMVECTOR point0, DirectX::XMVECTOR point1, DirectX::XMVECTOR point2) noexcept
{   
    ::XMFLOAT4 resultPlane = {0.0f,0.0f,0.0f,0.0f};
    ::XMFLOAT4 convertPlaneNormal;
    ::XMFLOAT4 convertPoint;
    ::XMVECTOR edge0 = point1 - point0;
    ::XMVECTOR edge1 = point2 - point0;

    ::XMVECTOR planeNormal = ::XMVector3Cross(edge0, edge1 );
    planeNormal = ::XMVector3Normalize(planeNormal);

    //ax + by + cz + d = 0

    ::XMStoreFloat4(&convertPlaneNormal, planeNormal);
    ::XMStoreFloat4(&convertPoint, point0);

    resultPlane.x = convertPlaneNormal.x;
    resultPlane.y = convertPlaneNormal.y;
    resultPlane.z = convertPlaneNormal.z;
    // d = -(ax + by + cz)
    resultPlane.w = -( convertPlaneNormal.x * convertPoint.x + 
                       convertPlaneNormal.y * convertPoint.y + 
                       convertPlaneNormal.z * convertPoint.z );
                     

    return resultPlane;
}

bool mydx::Frustum::InspectionPoint(DirectX::XMVECTOR point) noexcept
{
    ::XMFLOAT4 convertPoint;
    ::XMStoreFloat4(&convertPoint,point);

    for (auto plane : mPlane)
    {
        // ax + by + cz + d = 0
        //평면의 노말 방향과 같은 방향에 존재한다면 길이는 양수, 다르면 음수
        const float distance = plane.x * convertPoint.x + plane.y * convertPoint.y + plane.z * convertPoint.z + plane.w;

        if ( distance < 0.0f)
            return false;
    }
    return true;
}

bool mydx::Frustum::InspectionSphere(DirectX::XMVECTOR point, float radius) noexcept
{
    ::XMFLOAT4 convertPoint;
    ::XMStoreFloat4(&convertPoint, point);

    for (auto plane : mPlane)
    {
        // ax + by + cz + d = 0
        //평면의 노말 방향과 같은 방향에 존재한다면 길이는 양수, 다르면 음수
        const float distance = plane.x * convertPoint.x + plane.y * convertPoint.y + plane.z * convertPoint.z + plane.w;

        if (distance <= -radius)
            return false;
    }
    return true;
}

bool mydx::Frustum::InspectOBBAndPlane(const BoundingBox& box)
{
    

    float planeToCenter = {};
    float distance = {};
    ::XMVECTOR direction = {};
    BoundingBoxData boxData = box.GetBoundingBoxData();

    // d = N dot ( normal(axis) * extend)xyz
    for (auto plane : mPlane)
    {
        //x axis
        direction = boxData.Axis[0] * boxData.Extent[0];
        distance = plane.x * ::XMVectorGetX(direction) + plane.y * ::XMVectorGetY(direction) + plane.z * ::XMVectorGetZ(direction);

        //y axis
        direction += boxData.Axis[1] * boxData.Extent[1];
        distance = plane.x * ::XMVectorGetX(direction) + plane.y * ::XMVectorGetY(direction) + plane.z * ::XMVectorGetZ(direction);

        //z axis
        direction += boxData.Axis[2] * boxData.Extent[2];
        distance = plane.x * ::XMVectorGetX(direction) + plane.y * ::XMVectorGetY(direction) + plane.z * ::XMVectorGetZ(direction);

        //plane to center distance

        planeToCenter = plane.x * ::XMVectorGetX(boxData.Center) 
                        + plane.y * ::XMVectorGetY(boxData.Center) 
                        + plane.z * ::XMVectorGetZ(boxData.Center) 
                        + plane.w; // plane.w is between plane and origin distance
        
        if ( planeToCenter <= -distance)
        {
            return false;
        }
  
    }

    return true;
}


bool mydx::Frustum::Update(Graphics& graphics) noexcept
{
    mQuaternionRotation = ::XMQuaternionRotationRollPitchYawFromVector(mPitchYawRoll);
    mTransform = ::XMMatrixAffineTransformation(::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), ::XMVectorZero(), mQuaternionRotation, mPositionVector);

    ResourceUpdate(graphics);
	return false;
}


DirectX::XMMATRIX mydx::Frustum::GetTransform() const noexcept
{
	return mTransform;
}

bool mydx::Frustum::Initialize(Graphics& graphics) noexcept
{
    struct VerTex
    {
        ::XMVECTOR position;
        ::XMFLOAT4 color;
    };


   /* struct VerTex
    {
        struct
        {
            float x, y, z, w;
        } position;
        struct
        {
            float x, y, z, w;
        } color;
    };
    const std::vector<VerTex> vertices =
    {
        { ::XMVectorGetX(mFrustumPoints[0]), ::XMVectorGetY(mFrustumPoints[0]), ::XMVectorGetZ(mFrustumPoints[0]), ::XMVectorGetW(mFrustumPoints[0]), 1.0f,0.0f,0.0f,1.0f },
        { ::XMVectorGetX(mFrustumPoints[1]), ::XMVectorGetY(mFrustumPoints[1]), ::XMVectorGetZ(mFrustumPoints[1]), ::XMVectorGetW(mFrustumPoints[1]), 1.0f,0.0f,0.0f,1.0f},
        { ::XMVectorGetX(mFrustumPoints[2]), ::XMVectorGetY(mFrustumPoints[2]), ::XMVectorGetZ(mFrustumPoints[2]), ::XMVectorGetW(mFrustumPoints[2]), 1.0f,0.0f,0.0f,1.0f},
        { ::XMVectorGetX(mFrustumPoints[3]), ::XMVectorGetY(mFrustumPoints[3]), ::XMVectorGetZ(mFrustumPoints[3]), ::XMVectorGetW(mFrustumPoints[3]), 1.0f,0.0f,0.0f,1.0f},
        { ::XMVectorGetX(mFrustumPoints[4]), ::XMVectorGetY(mFrustumPoints[4]), ::XMVectorGetZ(mFrustumPoints[4]), ::XMVectorGetW(mFrustumPoints[4]), 1.0f,0.0f,0.0f,1.0f},
        {::XMVectorGetX(mFrustumPoints[5]), ::XMVectorGetY(mFrustumPoints[5]), ::XMVectorGetZ(mFrustumPoints[5]), ::XMVectorGetW(mFrustumPoints[5]),  1.0f,0.0f,0.0f,1.0f},
        {::XMVectorGetX(mFrustumPoints[6]), ::XMVectorGetY(mFrustumPoints[6]), ::XMVectorGetZ(mFrustumPoints[6]), ::XMVectorGetW(mFrustumPoints[6]),  1.0f,0.0f,0.0f,1.0f},
        { ::XMVectorGetX(mFrustumPoints[7]), ::XMVectorGetY(mFrustumPoints[7]), ::XMVectorGetZ(mFrustumPoints[7]), ::XMVectorGetW(mFrustumPoints[7]), 1.0f,0.0f,0.0f,1.0f},
    };*/
    /*  mFrustumPoints[0] = { -1.0f, -1.0f, 0.0f, 0.0f };
      mFrustumPoints[1] = { -1.0f,  1.0f, 0.0f, 0.0f };
      mFrustumPoints[2] = { 1.0f, -1.0f, 0.0f, 0.0f };
      mFrustumPoints[3] = { 1.0f,  1.0f, 0.0f, 0.0f };
      mFrustumPoints[4] = { -1.0f, -1.0f, 1.0f, 0.0f };
      mFrustumPoints[5] = { -1.0f,  1.0f, 1.0f, 0.0f };
      mFrustumPoints[6] = { 1.0f, -1.0f, 1.0f, 0.0f };
      mFrustumPoints[7] = { 1.0f,  1.0f, 1.0f, 0.0f };*/

    const std::vector<VerTex> vertices =
    {
        { mFrustumPoints[0], ::XMFLOAT4(1.0f,0.0f,0.0f,0.2f) },
        { mFrustumPoints[1],   ::XMFLOAT4(1.0f,0.0f,0.0f,0.2f)},
        {  mFrustumPoints[2],  ::XMFLOAT4(0.0f,1.0f,0.0f,0.2f)},
        { mFrustumPoints[3],   ::XMFLOAT4(0.0f,1.0f,0.0f,0.2f)},
        { mFrustumPoints[4],   ::XMFLOAT4(0.0f,1.0f,0.0f,0.2f)},
        {mFrustumPoints[5],    ::XMFLOAT4(0.0f,1.0f,0.0f,0.2f)},
        {mFrustumPoints[6],    ::XMFLOAT4(0.0f,1.0f,0.0f,0.2f)},
        { mFrustumPoints[7],   ::XMFLOAT4(0.0f,1.0f,0.0f,0.2f)},
    };


    AddBind(std::make_unique<VertexBuffer>(graphics, vertices));

    const std::vector<unsigned short> indices =
    {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4
    };

    AddIndexBuffer(std::make_unique<IndexBuffer>(graphics, indices));

    auto mVertexShaderClass = std::make_unique<VertexShader>(graphics, L"BaseVertexShader.hlsl", "main");
    auto bytecodeBlob = mVertexShaderClass->GetBytecodeBlob();

    AddBind(std::move(mVertexShaderClass));


    const std::vector< D3D11_INPUT_ELEMENT_DESC> layoutList =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    AddBind(std::make_unique<InputLayout>(graphics, layoutList, bytecodeBlob));

    AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

    AddBind(std::make_unique<PixelShader>(graphics, L"BasePixelShader.hlsl", "main"));

    AddBind(std::make_unique<TransformConstantBuffer>(graphics, *this));
	return false;
}

bool mydx::Frustum::PreRender(Graphics& graphics) noexcept
{
	ResourceBinding(graphics);
	return false;
}

bool mydx::Frustum::Render(Graphics& graphics) noexcept
{
	PreRender(graphics);
	Draw(graphics);
	PostRender(graphics);
	return false;
}

bool mydx::Frustum::PostRender(Graphics& graphics) noexcept
{
	return false;
}

void  mydx::Frustum::SetPitchYawRoll(DirectX::XMVECTOR vector) noexcept
{
    mPitchYawRoll = vector;
}

const DirectX::XMVECTOR& mydx::Frustum::GetPitchYawRoll() const noexcept
{
    return mPitchYawRoll;
}

void mydx::Frustum::SetPosition(DirectX::XMVECTOR vector)noexcept
{
    mPositionVector = vector;
}

const DirectX::XMVECTOR& mydx::Frustum::GetPosition() const noexcept
{
    return mPositionVector;
}