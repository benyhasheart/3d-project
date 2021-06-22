#pragma once

#include "Node.h"
#include "Drawable.h"
#include "Select.h"

class Camera;
class Map;

namespace mydx
{

	class QuardTree : public Drawable
	{
	public:
		QuardTree();
		virtual ~QuardTree() = default;

	public:
		virtual bool Build(float width, float height) noexcept;

	public:
		Node* FindSelectedNode(Node* node, Select* selectClass);
		int UpdateIndexTable(DWORD currentIndex, DWORD topLeft, DWORD topRight, DWORD bottomLeft, DWORD bottomRight, std::vector<DWORD>& indexTable) noexcept;
	public:
		virtual DirectX::XMMATRIX GetTransform() const noexcept override;
		virtual bool Initialize(Graphics& graphics) noexcept override ;
		virtual bool Update(Graphics& graphics) noexcept override ;
		virtual bool PreRender(Graphics& graphics) noexcept override ;
		virtual bool Render(Graphics& graphics) noexcept override ;
		virtual bool PostRender(Graphics& graphics) noexcept override ;
		virtual bool Release() noexcept override  { return false; }

	public:
		void SetCamera(Camera* camera)noexcept;
		void SetMap(Map* map)noexcept;
		void SetSelectClass(Select* select) noexcept;

		void SetDepth(int depth) noexcept;
		int GetDepth() noexcept;

		std::vector<Node*>& GetDrawableNodeTable() noexcept;
		std::vector<DWORD>& GetUpdateIndexTable() noexcept;
	protected:
		virtual bool buildTree(Node* node) noexcept;
		virtual bool divideSection(Node* node) noexcept;
		virtual Node* createNode(Node* parentNode, float minX, float maxX, float minZ, float maxZ) noexcept;

	protected:

		bool findDrawableNode(Node* node) noexcept;
		bool computeSelectedNode(Node* node, DirectX::XMVECTOR start, DirectX::XMVECTOR end);
		



	protected:
		std::shared_ptr<Node> mRootNode;		
		std::vector<Node*> mDrawableNode;
		std::vector<DWORD> mUpdateIndexTable;

		Camera*	mCamera;
		Map* mMap;
		Select* mSelect;
		Node* mFindNode;

		DWORD mWidth;
		DWORD mHeight;
		int mDepth;
		unsigned int mFaceCount;
	};
}


