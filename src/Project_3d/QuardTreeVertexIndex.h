#pragma once

#include "QuardTree.h"

namespace mydx
{
	class QuardTreeVertexIndex : public mydx::QuardTree
	{
	public:
		QuardTreeVertexIndex();
		virtual ~QuardTreeVertexIndex() =  default;

	public:
		bool Build(Map* map, DWORD width, DWORD height) noexcept;

	//public:
	//	virtual bool Initialize(Graphics& graphics) noexcept final override;
		virtual bool Update(Graphics& graphics) noexcept final override;
	//	virtual bool PreRender(Graphics& graphics) noexcept final override;
	//	virtual bool Render(Graphics& graphics) noexcept final override;
	//	virtual bool PostRender(Graphics& graphics) noexcept final override;
	//	virtual bool Release() noexcept final override { return false; }
	
	private:
		virtual bool divideSection(Node* node) noexcept override  final;
		virtual Node* createNode(Node* parentNode, unsigned long minX, unsigned long maxX, unsigned long minZ, unsigned long maxZ) noexcept ;

	private:
		//비균형 일때 작은 크기를 반환 ex 5 -> 2, 7 -> 3
		unsigned long checkDivideSize(unsigned long size);
		void ComputeBoungigBox(Node* pNode) noexcept;

	};
}


