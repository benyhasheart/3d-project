#include <type_traits>

#include "CommonDataStruct.h"
#include "Camera.h"
#include "EnumData.h"
#include "Map.h"
#include "QuardTree.h"

using namespace DirectX;

mydx::QuardTree::QuardTree()
	:
	mRootNode(nullptr),
	mCamera(nullptr),
	mMap(nullptr),
	mSelect(nullptr),
	mFindNode(nullptr),
	mWidth(0.0f),
	mHeight(0.0f),
	mFaceCount(0),
	mDepth(10)
{
}

bool mydx::QuardTree::Build(float width, float height) noexcept
{
	const float minX = -width / 2.0f;
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



DirectX::XMMATRIX mydx::QuardTree::GetTransform() const noexcept
{
	return DirectX::XMMATRIX();
}

bool mydx::QuardTree::Initialize(Graphics& graphics) noexcept
{
	return false;
}

bool mydx::QuardTree::Update(Graphics& graphics) noexcept
{
	mDrawableNode.clear();
	findDrawableNode(mRootNode.get());
	return true;
}

bool mydx::QuardTree::PreRender(Graphics& graphics) noexcept
{
	ResourceBinding(graphics);
	return false;
}

bool mydx::QuardTree::Render(Graphics& graphics) noexcept
{
	PreRender(graphics);
	Draw(graphics);
	PostRender(graphics);
	return false;
}

bool mydx::QuardTree::PostRender(Graphics& graphics) noexcept
{
	return false;
}

void mydx::QuardTree::SetCamera(Camera* camera) noexcept
{
	mCamera = camera;
}

void mydx::QuardTree::SetMap(Map* map) noexcept
{
	mMap = map;
}

void mydx::QuardTree::SetSelectClass(Select* select) noexcept
{
	mSelect = select;
}


void mydx::QuardTree::SetDepth(int depth) noexcept
{
	mDepth = depth;
}

int mydx::QuardTree::GetDepth() noexcept
{
	return mDepth;
}

std::vector<mydx::Node*>& mydx::QuardTree::GetDrawableNodeTable() noexcept
{
	return mDrawableNode;
}

std::vector<DWORD>& mydx::QuardTree::GetUpdateIndexTable() noexcept
{
	return mUpdateIndexTable;
}

bool mydx::QuardTree::buildTree(Node* node) noexcept
{
	if (node == nullptr)
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
	::XMMATRIX transform = ::XMMatrixIdentity();
	boundingBox.Center = (boundingBox.Max + boundingBox.Min) / 2.0f;
	transform = ::XMMatrixMultiply(transform, ::XMMatrixTranslationFromVector(boundingBox.Center));
	boundingBox.Center = (boundingBox.Max + boundingBox.Min) / 2.0f;
	boundingBox.Axis[0] = ::XMVector3Normalize(::XMVector3TransformCoord(::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), transform));
	boundingBox.Axis[1] = ::XMVector3Normalize(::XMVector3TransformCoord(::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), transform));
	boundingBox.Axis[2] = ::XMVector3Normalize(::XMVector3TransformCoord(::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), transform));
	boundingBox.Extent[0] = ::XMVectorSet(::XMVectorGetX(boundingBox.Max) - ::XMVectorGetX(boundingBox.Center), 0.0f, 0.0f, 0.0f);
	boundingBox.Extent[1] = ::XMVectorSet(0.0f, ::XMVectorGetY(boundingBox.Max) - ::XMVectorGetY(boundingBox.Center), 0.0f, 0.0f);
	boundingBox.Extent[2] = ::XMVectorSet(0.0f, 0.0f, ::XMVectorGetZ(boundingBox.Max) - ::XMVectorGetZ(boundingBox.Center), 0.0f);
	node->SetBoundingBox(boundingBox);


	node->GetCornerList().emplace_back(::XMVectorSet(minX, 0.0f, maxZ, 0.0f));
	node->GetCornerList().emplace_back(::XMVectorSet(maxX, 0.0f, maxZ, 0.0f));
	node->GetCornerList().emplace_back(::XMVectorSet(minX, 0.0f, minZ, 0.0f));
	node->GetCornerList().emplace_back(::XMVectorSet(maxX, 0.0f, minZ, 0.0f));

	return node;
}

bool mydx::QuardTree::findDrawableNode(Node* node) noexcept
{
	if (mCamera == nullptr)
		return false;

	mydx::eCollisionResult eResult = {};

	eResult = mCamera->GetFrustum().InspectOBBAndPlane(node->GetBoundingBox());
	/* if (node->GetDepth() >= 1 && eResult != eCollisionResult::Back)
	 {
		 mDrawableNode.push_back(node);
		 return true;
	 }*/
	 // 리프노드이면서 걸쳐있는 경우 포함.
	if (node->GetIsLeafNode() && eResult != eCollisionResult::Back)
	{
		mDrawableNode.push_back(node);
		return true;
	}
	// 완전 포함
	if (eResult == eCollisionResult::Front)
	{
		mDrawableNode.push_back(node);
		return true;
	}
	// 걸쳐져있는 경우
	if (eResult == eCollisionResult::Spanning)
	{

		for (auto& childNode : node->GetChildNodeTable())
		{
			findDrawableNode(childNode.get());
		}
	}

	return true;
}

bool mydx::QuardTree::computeSelectedNode(Node* node, DirectX::XMVECTOR start, DirectX::XMVECTOR end)
{

	// 0  1
	// 2  3
	::XMVECTOR edge0 = ::XMVectorSetX(
		node->GetBoundingBox().Max,
		::XMVectorGetX(node->GetBoundingBox().Min));
	::XMVECTOR edge1 = node->GetBoundingBox().Max;
	::XMVECTOR edge2 = node->GetBoundingBox().Min;
	::XMVECTOR edge3 = ::XMVectorSetX(
		node->GetBoundingBox().Min,
		::XMVectorGetX(node->GetBoundingBox().Max));

	if (node->GetIsLeafNode())
	{
		bool result0 = mSelect->PointInPolygon(
			mSelect->GetIntersectingLineSegment(),
			edge1, edge2, edge0);

		bool result1 = mSelect->PointInPolygon(
			mSelect->GetIntersectingLineSegment(),
			edge1, edge3, edge2);

		if (result0 || result1)
		{
			mFindNode = node;
			return true;
		}

		return false;
	}

	// 1 cell = 2face 2 페이스 모두 체크 해 봐야한다.
	if (mSelect->CheckInterSection(start, end, edge0, edge1, edge2))
	{

		for (auto& childNode : node->GetChildNodeTable())
		{
			if (computeSelectedNode(childNode.get(), start, end))
			{
				return true;
			}
		}

	}

	return false;
}

mydx::Node* mydx::QuardTree::FindSelectedNode(Node* node, Select* selectClass)
{
	if (mSelect == nullptr && selectClass == nullptr)
		return nullptr;

	if (node == nullptr) node = mRootNode.get();

	mSelect = selectClass;

	::XMVECTOR start = selectClass->GetRay().Origine;
	::XMVECTOR end = start + selectClass->GetRay().Direction * selectClass->GetRay().Extent;


	if (computeSelectedNode(node, start, end))
	{
		return mFindNode;
	}

	return nullptr;
}

int mydx::QuardTree::UpdateIndexTable(DWORD currentIndex, DWORD topLeft, DWORD topRight, DWORD bottomLeft, DWORD bottomRight, std::vector<DWORD>& indexTable) noexcept
{

	int faceCount = 0;
	UINT colVertexCount = mWidth + 1;
	DWORD startRow = topLeft / colVertexCount;
	DWORD endRow = bottomLeft / colVertexCount;
	// DWORD endRow = 150;

	DWORD startCol = topLeft % colVertexCount;
	DWORD endCol = topRight % colVertexCount;
	UINT rowVertexCount = mHeight + 1;


	for (DWORD row = startRow; row < endRow; row++)
	{
		for (DWORD col = startCol; col < endCol; col++)
		{
			DWORD nextCol = col + 1;
			DWORD nextRow = row + 1;
			DWORD index = currentIndex;

			indexTable[index + 0] = colVertexCount * row + col;
			indexTable[index + 1] = colVertexCount * row + nextCol;
			indexTable[index + 2] = colVertexCount * nextRow + col;
			indexTable[index + 3] = indexTable[index + 1];
			indexTable[index + 4] = colVertexCount * nextRow + nextCol;
			indexTable[index + 5] = indexTable[index + 2];

			faceCount += 2;
			currentIndex += 6;
		}
	}
	return faceCount;
}
