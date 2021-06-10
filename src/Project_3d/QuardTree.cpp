#include <type_traits>
#include "QuardTree.h"

using namespace DirectX;

mydx::QuardTree::QuardTree()
    :
    mRootNode(nullptr)
{
}

bool mydx::QuardTree::Build(float width, float height) noexcept
{
    const float minX = - width / 2.0f;
    const float maxX = width / 2.0f;
    const float minZ = -height / 2.0f;
    const float maxZ = height / 2.0f;
    
    mRootNode = std::make_shared<mydx::Node>(*createNode(nullptr, minX, maxX, minZ, maxZ));

    if (mRootNode != nullptr)
    {
        return buildTree(mRootNode.get());
    }
    return false;
}

bool mydx::QuardTree::buildTree(Node* node) noexcept
{
    if ( node == nullptr)
        return false;

    if (divideSection(node))
    {
        for (auto& childNode : node->GetChildNodeTable())
        {
            buildTree(childNode.get());
        }
    }

    return true;
}

bool mydx::QuardTree::divideSection(Node* node) noexcept
{
    if (node == nullptr)
        return false;

    const int topLeftIndex = static_cast<std::underlying_type<eCornerDirection>::type>(eCornerDirection::TopLeft);
    const int topRightIndex = static_cast<std::underlying_type<eCornerDirection>::type>(eCornerDirection::TopRight);
    const int bottomLeftIndex = static_cast<std::underlying_type<eCornerDirection>::type>(eCornerDirection::BottomLeft);
    const int bottomRightIndex = static_cast<std::underlying_type<eCornerDirection>::type>(eCornerDirection::BottomRight);

    const auto& cornerList = node->GetCornerList();
    float minX = ::XMVectorGetX(cornerList[topLeftIndex]);
    float maxX = ::XMVectorGetX(cornerList[topRightIndex]);
    float minZ = ::XMVectorGetZ(cornerList[bottomRightIndex]);
    float maxZ = ::XMVectorGetZ(cornerList[topLeftIndex]);
    const float widthHalf = (maxX - minX) / 2.0f;
    const float heightHalf = (maxZ - minZ) / 2.0f;

    // 더이상 쪼개지지 않는 조건
    const float minDiviedCount = 1.0f;
    if ((widthHalf < minDiviedCount) || (heightHalf < minDiviedCount))
    {
        node->SetIsLeafNode(true);
        return false;
    }
    // 지정한 depth보다 클 경우


    //4개 자식 노드 생성
    auto& childNodeTable = node->GetChildNodeTable();
    //topleft
    childNodeTable.emplace_back(createNode(node,
        minX, minX + widthHalf, minZ + heightHalf, maxZ));
    //topRight
    childNodeTable.emplace_back(createNode(node,
        minX + widthHalf, maxX, minZ + heightHalf, maxZ));
    //bottomLeft
    childNodeTable.emplace_back(createNode(node,
        minX, minX + widthHalf, minZ, minZ + heightHalf));
    //bottomRight
    childNodeTable.emplace_back(createNode(node,
        minX + widthHalf, maxX, minZ, minZ + heightHalf));

    return true;
}

mydx::Node* mydx::QuardTree::createNode(Node* parentNode, float minX, float maxX, float minZ, float maxZ) noexcept
{
    mydx::Node* node = new mydx::Node();

    if (parentNode != nullptr)
    {
        node->SetDepth(parentNode->GetDepth() + 1u);
    }

    node->GetChildNodeTable().reserve(4);
    node->GetCornerList().reserve(4);
    
    mydx::BoundingBoxData boundingBox;
    boundingBox.Min = ::XMVectorSet(minX, 0.0f, minZ, 0.0f);
    boundingBox.Max = ::XMVectorSet(maxX, 0.0f, maxZ, 0.0f);

    boundingBox.Center = (boundingBox.Max + boundingBox.Min ) / 2.0f;
    boundingBox.Extent[0] = ::XMVectorSet(::XMVectorGetX(boundingBox.Max) - ::XMVectorGetX(boundingBox.Center), 0.0f,0.0f,0.0f );
    boundingBox.Extent[1] = ::XMVectorSet(0.0f, ::XMVectorGetY(boundingBox.Max) - ::XMVectorGetY(boundingBox.Center),  0.0f, 0.0f);
    boundingBox.Extent[2] = ::XMVectorSet(0.0f, 0.0f, ::XMVectorGetZ(boundingBox.Max) - ::XMVectorGetZ(boundingBox.Center), 0.0f);
    node->SetBoundingBox(boundingBox);


    node->GetCornerList().emplace_back(::XMVectorSet(minX, 0.0f, maxZ,0.0f));
    node->GetCornerList().emplace_back(::XMVectorSet(maxX, 0.0f, maxZ, 0.0f));
    node->GetCornerList().emplace_back(::XMVectorSet(minX, 0.0f, minZ, 0.0f));
    node->GetCornerList().emplace_back(::XMVectorSet(maxX, 0.0f, minZ, 0.0f));

    return node;
}
