#pragma once
#include <DirectXMath.h>

using namespace DirectX;
namespace mydx
{
	//screen ��ǥ���� ���� ��ǥ�� ��ȯ. 
	::XMVECTOR ScreenToClient(::XMVECTOR screenPosition, float viewWidth, float viewHeight)
	{
		::XMVECTOR cursorPosition = {};

		if (viewWidth <= 0 || viewHeight <= 0)
			return cursorPosition;
		// ���� -> �� -> ���� -> ��ũ��(����Ʈ)
		// ��ũ��(����Ʈ) -> ����
		
		float posX = (::XMVectorGetX(screenPosition) * 2.0f / viewWidth) - 1 ;
		float posY = -((::XMVectorGetY(screenPosition) * 2.0f / viewHeight) - 1);

		cursorPosition = ::XMVectorSet(posX, posY, 0.0f, 1.0f);

		return cursorPosition;
	}

}