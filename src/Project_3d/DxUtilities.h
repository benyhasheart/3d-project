#pragma once
#include <DirectXMath.h>

using namespace DirectX;
namespace mydx
{
	//screen 좌표에서 투영 좌표로 변환. 
	::XMVECTOR ScreenToClient(::XMVECTOR screenPosition, float viewWidth, float viewHeight)
	{
		::XMVECTOR cursorPosition = {};

		if (viewWidth <= 0 || viewHeight <= 0)
			return cursorPosition;
		// 월드 -> 뷰 -> 투영 -> 스크린(뷰포트)
		// 스크린(뷰포트) -> 투영
		
		float posX = (::XMVectorGetX(screenPosition) * 2.0f / viewWidth) - 1 ;
		float posY = -((::XMVectorGetY(screenPosition) * 2.0f / viewHeight) - 1);

		cursorPosition = ::XMVectorSet(posX, posY, 0.0f, 1.0f);

		return cursorPosition;
	}

}