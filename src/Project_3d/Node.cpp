#include "Node.h"


mydx::Node::Node()
	:
	mDepth(0),
	mbLeafNode(false),
	mBoundingBox({ 0 })
{
}

void mydx::Node::SetDepth(unsigned int depth) noexcept
{
	mDepth = depth;
}

unsigned int mydx::Node::GetDepth() const noexcept
{
	return mDepth;
}

void mydx::Node::SetIsLeafNode(bool isLeafNode) noexcept
{
	mbLeafNode = isLeafNode;
}

bool mydx::Node::GetIsLeafNode() const noexcept
{
	return mbLeafNode;
}

void mydx::Node::SetBoundingBox(mydx::BoundingBoxData boundingBox) noexcept
{
	mBoundingBox = boundingBox;
}

mydx::BoundingBoxData& mydx::Node::GetBoundingBox() noexcept
{
	return mBoundingBox;
}

std::vector<std::shared_ptr<mydx::Node>>& mydx::Node::GetChildNodeTable() noexcept
{
	return mChildNodeTable;
}

std::vector<DirectX::XMVECTOR>& mydx::Node::GetCornerList() noexcept
{
	return mCornerList;
}

std::vector<unsigned long>& mydx::Node::GetCornerVertexIndexTable() noexcept
{
	return mCornerVertexIndexTable;
}
