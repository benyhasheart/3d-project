#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char key) noexcept
{
    return mKeyState[key];
}

void Keyboard::KeyPressed(unsigned char key) noexcept
{
    mKeyState[key] = true;
}

void Keyboard::KeyReleased(unsigned char key) noexcept
{
    mKeyState[key] = false;
}

void Keyboard::ClearState() noexcept
{
    mKeyState.reset();
}