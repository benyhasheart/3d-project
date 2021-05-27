#pragma once
#include <bitset>

class Keyboard
{
public:
	Keyboard() = default;
	Keyboard(Keyboard& src) = delete;
	Keyboard& operator&(Keyboard& src) = delete;

	virtual ~Keyboard() = default;

public:

	bool KeyIsPressed(unsigned char key)noexcept;
	void KeyPressed(unsigned char key)noexcept;
	void KeyReleased(unsigned char key)noexcept;

	void ClearState() noexcept;

private:
	static constexpr unsigned int mKeySize = 256u;
	std::bitset<mKeySize> mKeyState;
};

