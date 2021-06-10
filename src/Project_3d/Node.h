#pragma once
#include <vector>
#include <memory>

#include "CommonDataStruct.h"

namespace mydx
{
	enum class eCornerDirection
	{
		TopLeft = 0,
		TopRight,
		BottomLeft,
		BottomRight
	};

	class Node
	{
	public:
		Node();
		virtual ~Node() = default;

	public:
		void SetDepth(unsigned int depth) noexcept;
		unsigned int GetDepth() const noexcept;

		void SetIsLeafNode(bool isLeafNode) noexcept;
		bool GetIsLeafNode() const noexcept;

		void SetBoundingBox(mydx::BoundingBoxData boundingBox) noexcept;
		mydx::BoundingBoxData& GetBoundingBox() noexcept;

		std::vector<std::shared_ptr<Node>>& GetChildNodeTable()  noexcept;
		std::vector<DirectX::XMVECTOR>& GetCornerList()  noexcept;
		std::vector<unsigned long>& GetCornerVertexIndexTable()  noexcept;

	private:
		unsigned int mDepth;
		bool	mbLeafNode;
		std::vector<std::shared_ptr<Node>> mChildNodeTable;
		std::vector<DirectX::XMVECTOR> mCornerList;
		std::vector<unsigned long> mCornerVertexIndexTable;

		mydx::BoundingBoxData mBoundingBox;
	};
}


