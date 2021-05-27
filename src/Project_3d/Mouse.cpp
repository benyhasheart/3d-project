#include "Mouse.h"
Mouse::Mouse()
	:
	mPositionX(0),
	mPositionY(0),
	mLastPositionX(0),
	mLastPositionY(0),
	mOffsetPositionX(0),
	mOffsetPositionY(0),
	mbLeftIsPressed(false),
	mbRightIsPressed(false)
{

}

void Mouse::Update() noexcept
{
	if (mbLeftIsPressed || mbRightIsPressed)
	{
		mOffsetPositionX = mPositionX - mLastPositionX;
		mOffsetPositionY = mPositionY - mLastPositionY;
		mLastPositionX = mPositionX;
		mLastPositionY = mPositionY;
	}
	
}
void Mouse::SetMove(int x, int y)noexcept
{
	mPositionX = x;
	mPositionY = y;
}

void Mouse::SetPositionXY(int x, int y)noexcept
{
	mPositionX = x;
	mPositionY = y;
}
std::pair<int, int> Mouse::GetPositionXY()const noexcept
{
	return {mPositionX,mPositionY};
}

void Mouse::SetPosistionX(int x)noexcept
{
	mPositionX = x;
}

const int& Mouse::GetPostionX()const noexcept
{
	return mPositionX;
}

void Mouse::SetPosistionY(int y)noexcept
{
	mPositionY = y;
}

const int& Mouse::GetPostionY()const noexcept
{
	return mPositionY;
}

std::pair<int, int> Mouse::GetOffsetPositionXY()const noexcept
{
	return {mOffsetPositionX, mOffsetPositionY};
}

const int& Mouse::GetOffsetPostionX()const noexcept
{
	return mOffsetPositionX;
}

const int& Mouse::GetOffsetPostionY()const noexcept
{
	return mOffsetPositionY;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return mbLeftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return mbRightIsPressed;
}


void Mouse::OnLeftPressed(int x, int y)noexcept
{
	mbLeftIsPressed = true;
	mLastPositionX = x;
	mLastPositionY = y;
	mPositionX = x;
	mPositionY = y;
}

void Mouse::OnLeftRelease(int x, int y)noexcept
{
	mbLeftIsPressed = false;
	mLastPositionX = 0;
	mLastPositionY = 0;
	mOffsetPositionX = 0;
	mOffsetPositionY = 0;
}

void Mouse::OnRightPressed(int x, int y)noexcept
{
	mbRightIsPressed = true;
	mLastPositionX = x;
	mLastPositionY = y;
}

void Mouse::OnRightRelease(int x, int y)noexcept
{
	mbRightIsPressed = false;
	mLastPositionX = 0;
	mLastPositionY = 0;
	mOffsetPositionX = 0;
	mOffsetPositionY = 0;
}