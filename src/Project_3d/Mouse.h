#pragma once
#include <queue>
class Mouse
{
public:
	Mouse();
	Mouse(Mouse& src) =delete;
	virtual ~Mouse()=default;

public:
	Mouse& operator = (Mouse& src) = delete;

public:
	void Update() noexcept;
public:
	void SetMove(int x, int y)noexcept;
public: // set,get
	void SetPositionXY(int x, int y)noexcept;
	std::pair<int,int> GetPositionXY()const noexcept;

	void SetPosistionX(int x)noexcept;
	const int& GetPostionX()const noexcept;

	void SetPosistionY(int y)noexcept;
	const int& GetPostionY()const noexcept;

	std::pair<int, int> GetOffsetPositionXY()const noexcept;
	const int& GetOffsetPostionX()const noexcept;
	const int& GetOffsetPostionY()const noexcept;

	void OnLeftPressed(int x, int y)noexcept;
	void OnLeftRelease(int x, int y)noexcept;

	void OnRightPressed(int x, int y)noexcept;
	void OnRightRelease(int x, int y)noexcept;

	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
private:
	int mPositionX;
	int mPositionY;
	int mLastPositionX;
	int mLastPositionY;
	int mOffsetPositionX;
	int mOffsetPositionY;
	bool mbLeftIsPressed;
	bool mbRightIsPressed;
	//std::queue<event> mEventBuffer;
};

