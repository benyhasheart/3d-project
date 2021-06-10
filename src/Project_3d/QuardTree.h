#pragma once

#include "Node.h"

namespace mydx
{

	class QuardTree
	{
	public:
		QuardTree();
		virtual ~QuardTree() = default;

	public:
		virtual bool Build(float width, float height) noexcept;
	protected:
		virtual bool buildTree(Node* node) noexcept;
		virtual bool divideSection(Node* node) noexcept;
		virtual Node* createNode(Node* parentNode, float minX, float maxX, float minZ, float maxZ) noexcept;

	protected:
		std::shared_ptr<Node> mRootNode;		
		
	};
}


