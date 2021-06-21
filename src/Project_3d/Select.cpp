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

    //���� -> ī�޶�
    //���� ����� ������� ���ϰų�, ������� _11, _22�� ������ ��ġ�� ������ �̸� Ȱ���Ѵ�.

    // 1)
    ::XMVECTOR determinant = ::XMMatrixDeterminant(mCamera->GetProjectionMatrix());
    ::XMMATRIX inverseProjectionMatirx =  ::XMMatrixInverse(&determinant, mCamera->GetProjectionMatrix());
    
    // ���� ������ * ��������� ����� = �� ������
    ::XMVECTOR viewPosition = ::XMVector3TransformCoord(projectionPosition, inverseProjectionMatirx);

    // �� -> ����
    // �� ����� ��

    ::XMVECTOR viewMatrixDeterminant = ::XMMatrixDeterminant(mCamera->GetViewMatrix());
    ::XMMATRIX inverseViewMatirx = ::XMMatrixInverse(&viewMatrixDeterminant, mCamera->GetViewMatrix());

    // origine�� �������� �俪��� ����
    mRay.Origine = ::XMVectorSet(0.0f,0.0f,0.0f,1.0f);
    mRay.Origine = ::XMVector3TransformCoord(mRay.Origine, inverseViewMatirx);
    // direction�� �������� ���� �俪��� ����
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

    //cross�� ���� ���ʿ� �ִٸ� ������ facenormal�� ����� �����̰�, �ƴϸ� �ݴ� ����.
    //cross ����� facenormal�� dot�� ���� ����̸� ���� ���̰� 90�� ���̰� ���� ����.
    // �ݴ�� ���� �̸� �̹� cross����� ������ �ݴ��̱� ������ ���� �ܺο� �ִ�.
    
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
