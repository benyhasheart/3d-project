#include "Camera.h"
#include "DxUtilities.h"
#include "Select.h"
#include "YonWindow.h"

using namespace mydx;
using namespace DirectX;
Select::Select()
    :
    mCamera(nullptr),
    mRay({}),
    mIntersectingLineSegment({})
{
    mRay.Extent = 100.0f;
}

DirectX::XMVECTOR mydx::Select::ScreenToWorld(DirectX::XMVECTOR position, float screenWidth, float screenHeight) noexcept
{
    
    ::XMVECTOR projectionPosition = {};
    projectionPosition = mydx::ScreenToClient(position, screenWidth, screenHeight);

    //투영 -> 카메라
    //투영 행렬의 역행렬을 곱하거나, 투영행렬 _11, _22만 영향이 미치기 때문에 이를 활용한다.

    // 1)
    ::XMVECTOR determinant = ::XMMatrixDeterminant(mCamera->GetProjectionMatrix());
    ::XMMATRIX inverseProjectionMatirx =  ::XMMatrixInverse(&determinant, mCamera->GetProjectionMatrix());
    
    // 투영 포지션 * 투영행렬의 역행렬 = 뷰 포지션
    ::XMVECTOR viewPosition = ::XMVector3TransformCoord(projectionPosition, inverseProjectionMatirx);

    // 뷰 -> 월드
    // 뷰 역행렬 곱

    ::XMVECTOR viewMatrixDeterminant = ::XMMatrixDeterminant(mCamera->GetViewMatrix());
    ::XMMATRIX inverseViewMatirx = ::XMMatrixInverse(&viewMatrixDeterminant, mCamera->GetViewMatrix());

    // origine은 원점에서 뷰역행렬 영향
    mRay.Origine = ::XMVectorSet(0.0f,0.0f,0.0f,1.0f);
    mRay.Origine = ::XMVector3TransformCoord(mRay.Origine, inverseViewMatirx);
    // direction은 뷰포지션 에서 뷰역행렬 영향
    mRay.Direction = viewPosition;
    mRay.Direction = ::XMVector3TransformNormal(mRay.Direction, inverseViewMatirx);
    mRay.Direction = ::XMVector3Normalize(mRay.Direction);

    return  mRay.Direction;
}

bool mydx::Select::PointInPolygon(DirectX::XMVECTOR intersectionPoint, DirectX::XMVECTOR vertex0, DirectX::XMVECTOR vertex1, DirectX::XMVECTOR vertex2) noexcept
{
    ::XMVECTOR intersectionPointVector = {};
    ::XMVECTOR cross ={};
    ::XMVECTOR dot = {};
    ::XMVECTOR edge0 = vertex1 - vertex0;
    ::XMVECTOR edge1 = vertex2 - vertex1;
    ::XMVECTOR edge2 = vertex0 - vertex2;
    ::XMVECTOR faceNormal =::XMVector3Normalize(::XMVector3Cross(edge0, edge1));

    //cross을 통해 안쪽에 있다면 방향이 facenormal과 비슷한 방향이고, 아니면 반대 방향.
    //cross 결과와 facenormal과 dot을 통해 양수이면 둘의 차이가 90도 차이가 나지 않음.
    // 반대로 음수 이면 이미 cross결과가 방향이 반대이기 때문에 도형 외부에 있다.
    
    // 0
    intersectionPointVector = intersectionPoint - vertex0;

    cross = ::XMVector3Normalize(::XMVector3Cross(edge0, intersectionPointVector));
    dot = ::XMVector3Normalize(::XMVector3Dot(cross, faceNormal));

    if ( ::XMVectorGetX(dot) < 0.0f) return false;

    // 1
    intersectionPointVector = intersectionPoint - vertex1;

    cross = ::XMVector3Normalize(::XMVector3Cross(edge1, intersectionPointVector));
    dot = ::XMVector3Normalize(::XMVector3Dot(cross, faceNormal));

    if (::XMVectorGetX(dot) < 0.0f) return false;

    // 2
    intersectionPointVector = intersectionPoint - vertex2;

    cross = ::XMVector3Normalize(::XMVector3Cross(edge2, intersectionPointVector));
    dot = ::XMVector3Normalize(::XMVector3Dot(cross, faceNormal));

    if (::XMVectorGetX(dot) < 0.0f) return false;
    
    return true;
}

bool mydx::Select::CheckInterSection(DirectX::XMVECTOR start, DirectX::XMVECTOR end, DirectX::XMVECTOR vertex0, DirectX::XMVECTOR vertex1, DirectX::XMVECTOR vertex2)
{
    DirectX::XMVECTOR edge0 = vertex1 - vertex0;
    DirectX::XMVECTOR edge1 = vertex2 - vertex0;

    DirectX::XMVECTOR faceNormal = ::XMVector3Normalize(::XMVector3Cross(edge0, edge1));

    //distance between vertex and start point
    DirectX::XMVECTOR distanceStartToVertex = vertex0 - start;

    // d = ax + by + cz;
    DirectX::XMVECTOR resultStartToVertex = ::XMVector3Dot(faceNormal, distanceStartToVertex);

    //distnace between start point and end point;
    DirectX::XMVECTOR distanceStartToEnd = end - start;

    // d = ax + by + cz;
    DirectX::XMVECTOR resultStartToEnd = ::XMVector3Dot(faceNormal, distanceStartToEnd);

    //distance ratio
    DirectX::XMVECTOR ratio = resultStartToVertex / resultStartToEnd;
    
    if (::XMVectorGetX(ratio) < 0.0f || ::XMVectorGetX(ratio) > 1.0f)
    {
        return false;
    }

    mIntersectingLineSegment = start + distanceStartToEnd * ratio;


    return true;
}

bool Select::Initialize(Graphics& graphics) noexcept
{
    return false;
}

bool Select::Update(Graphics& graphics) noexcept
{
    POINT cursor;
    GetCursorPos(&cursor);
    ::ScreenToClient(g_hwnd, &cursor);

    ::XMVECTOR position = ::XMVectorSet(cursor.x, cursor.y, 1.0f, 0.0f);
    ScreenToWorld(position, 800.0f, 600.0f);


    return false;
}

bool Select::PreRender(Graphics& graphics) noexcept
{
    return false;
}

bool Select::Render(Graphics& graphics) noexcept
{
    return false;
}

bool Select::PostRender(Graphics& graphics) noexcept
{
    return false;
}

bool Select::Release() noexcept
{
    return false;
}

DirectX::XMMATRIX Select::GetTransform() const noexcept
{
    return DirectX::XMMATRIX();
}

void mydx::Select::SetCamera(Camera* camera) noexcept
{
    mCamera = camera;
}

Ray& mydx::Select::GetRay() noexcept
{
    return mRay;
}

DirectX::XMVECTOR& mydx::Select::GetIntersectingLineSegment() noexcept
{
    return mIntersectingLineSegment;
}
