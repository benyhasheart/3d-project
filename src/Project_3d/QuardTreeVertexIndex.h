#pragma once

#include "QuardTree.h"
class Map;
namespace mydx
{
	class QuardTreeVertexIndex : public mydx::QuardTree
	{
	public:
		QuardTreeVertexIndex();
		virtual ~QuardTreeVertexIndex() =  default;

	public:
		bool Build(Map* map, float width, float height) noexcept;
	
	private:
		virtual bool divideSection(Node* node) noexcept override  final;
		virtual Node* createNode(Node* parentNode, unsigned long minX, unsigned long maxX, unsigned long minZ, unsigned long maxZ) noexcept ;
	private:
		//비균형 일때 작은 크기를 반환 ex 5 -> 2, 7 -> 3
		unsigned long checkDivideSize(unsigned long size);

	private:
		Map* mMap;
		float mWidth;
		float mHeight;
		unsigned int mFaceCount;
	};
}


