#pragma once
#include "Map.h"
#include "QuardTreeVertexIndex.h"

using namespace mydx;
using namespace DirectX;
mydx::QuardTreeVertexIndex::QuardTreeVertexIndex()
	:
	mMap(nullptr),
	mWidth(0.0f),
	mHeight(0.0f),
	mFaceCount(0)
{
}

bool mydx::QuardTreeVertexIndex::Build(Map* map, float width, float height) noexcept
{
    mWidth = width ;
    mHeight = height ;
    mFaceCount = (mWidth - 1) * (mHeight - 1) * 2 ;

    mRootNode = std::make_shared<mydx::Node>(*createNode(nullptr, 
                                                0, mWidth - 1, 
                                                mWidth  * (mHeight - 1), mWidth  * mHeight - 1));

    if (mRootNode != nullptr)
    {
        QuardTree::buildTree(mRootNode.get());
    }
	return true;
}


bool mydx::QuardTreeVertexIndex::divideSection(Node* node) noexcept
{
    if (node == nullptr)
        return false;

    const int topLeftIndex = static_cast<std::underlying_type<eCornerDirection>::type>(eCornerDirection::TopLeft);
    const int topRightIndex = static_cast<std::underlying_type<eCornerDirection>::type>(eCornerDirection::TopRight);
    const int bottomLeftIndex = static_cast<std::underlying_type<eCornerDirection>::type>(eCornerDirection::BottomLeft);
    const int bottomRightIndex = static_cast<std::underlying_type<eCornerDirection>::type>(eCornerDirection::BottomRight);

    const auto& cornerIndexTable = node->GetCornerVertexIndexTable();
    
    const unsigned long width = cornerIndexTable[topRightIndex] - cornerIndexTable[topLeftIndex];
    const unsigned long height = (cornerIndexTable[bottomLeftIndex] - cornerIndexTable[topLeftIndex] ) / mWidth;

    const unsigned long widthSplitSize = checkDivideSize(width);
    const unsigned long heightSplitSize = checkDivideSize(height);

   

    // 더이상 쪼개지지 않는 조건
    const float minDiviedCount = 1.0f;
    if ((widthSplitSize < minDiviedCount) || (heightSplitSize < minDiviedCount))
    {
        node->SetIsLeafNode(true);
        return false;
    }
    // 지정한 depth보다 클 경우


    //
    //         top
    //left  center right
    //      bottom
    //
    const unsigned long center = (cornerIndexTable[topLeftIndex] + widthSplitSize) +  heightSplitSize * mWidth;
    const unsigned long top = cornerIndexTable[topLeftIndex] + widthSplitSize;
    const unsigned long bottom = cornerIndexTable[bottomLeftIndex] + widthSplitSize;
    const unsigned long left = cornerIndexTable[topLeftIndex] + heightSplitSize * mWidth;
    const unsigned long right = cornerIndexTable[topRightIndex] + heightSplitSize * mWidth;

    //4개 자식 노드 생성
    auto& childNodeTable = node->GetChildNodeTable();
    //topleft 
    childNodeTable.emplace_back(createNode(node,
                                cornerIndexTable[topLeftIndex] , 
                                top,
                                left,
                                center));
    //topRight
    childNodeTable.emplace_back(createNode(node,
                                top,
                                cornerIndexTable[topRightIndex],
                                center,
                                right));
    //bottomLeft
    childNodeTable.emplace_back(createNode(node,
                                left,
                                center,
                                cornerIndexTable[bottomLeftIndex],
                                bottom));
    //bottomRight
    childNodeTable.emplace_back(createNode(node,
                                center,
                                right,
                                bottom,
                                cornerIndexTable[bottomRightIndex]));

    return true;
}

Node* mydx::QuardTreeVertexIndex::createNode(Node* parentNode, unsigned long minX, unsigned long maxX, unsigned long minZ, unsigned long maxZ) noexcept
{
    mydx::Node* node = new mydx::Node();

    if (parentNode != nullptr)
    {
        node->SetDepth(parentNode->GetDepth() + 1u);
    }

    node->GetChildNodeTable().reserve(4);
    node->GetCornerList().reserve(4);
    node->GetCornerVertexIndexTable().reserve(4);

    mydx::BoundingBoxData boundingBox;
    boundingBox.Min = ::XMVectorSet(minX, 0.0f, minZ, 0.0f);
    boundingBox.Max = ::XMVectorSet(maxX, 0.0f, maxZ, 0.0f);

    boundingBox.Center = (boundingBox.Max + boundingBox.Min) / 2.0f;
    boundingBox.Extent[0] = ::XMVectorSet(::XMVectorGetX(boundingBox.Max) - ::XMVectorGetX(boundingBox.Center), 0.0f, 0.0f, 0.0f);
    boundingBox.Extent[1] = ::XMVectorSet(0.0f, ::XMVectorGetY(boundingBox.Max) - ::XMVectorGetY(boundingBox.Center), 0.0f, 0.0f);
    boundingBox.Extent[2] = ::XMVectorSet(0.0f, 0.0f, ::XMVectorGetZ(boundingBox.Max) - ::XMVectorGetZ(boundingBox.Center), 0.0f);
    node->SetBoundingBox(boundingBox);


    node->GetCornerVertexIndexTable().emplace_back(minX);
    node->GetCornerVertexIndexTable().emplace_back(maxX);
    node->GetCornerVertexIndexTable().emplace_back(minZ);
    node->GetCornerVertexIndexTable().emplace_back(maxZ);

	return node;
}

unsigned long mydx::QuardTreeVertexIndex::checkDivideSize(unsigned long size)
{
    float fCount = 0;
    unsigned long dwChecker = size / 2;
    if (dwChecker <= 0) return 0;

    while (dwChecker != 0)
    {
        dwChecker >>= 1;
        fCount += 1.0f;
    }
    float fMinimum = pow(2.0f, fCount - 1.0f);
    float fMaximum = pow(2.0f, fCount);
    // fMin = abs( fMin - P0 ) - abs( P1 - fMin );
    // fMax = abs( fMax - P0 ) - abs( P1 - fMax );
    // return min( fMin, fMax );
    float fMin = fabs(fMinimum - fabs(((float)size - fMinimum)));
    float fMax = fabs(fMaximum - fabs(((float)size - fMaximum)));

    // 비교값이 같다면 작은 분할크기를 반환한다. 예) fMin=1 : fMax=1 => fMinimum=2 : fMaximum=4
    if (fMin <= fMax)
    {
        return fMinimum;
    }
    return fMaximum;
}

