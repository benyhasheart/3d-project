#pragma once
#include <Windows.h>
#include <optional>
#include <memory>
#include <string>
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"

extern HWND g_hwnd;
extern Graphics* g_graphics;
class YonWindow
{
public:
	YonWindow();
	YonWindow(YonWindow& src) = delete;
	YonWindow& operator=(YonWindow& src) =delete;
	~YonWindow();

public:
	bool InitWindow(HINSTANCE hInstance, const TCHAR* windowTitle, UINT width, UINT height, int nCmdShow);
	void CenterWindow(HWND hWnd);
public:
	std::optional<int> ProcessMessages();
public:
	Graphics& GetGFX() const noexcept;
	Keyboard& GetKeyboard()  noexcept;
	Mouse& GetMouse() noexcept;
private:
	static LRESULT CALLBACK wndProcSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) ;
	
	LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept;
	
public:
	static UINT windowWidth;
	static UINT windowHeight;
private:
	HINSTANCE mhInstance;
	HWND mhWnd;
	DWORD mWindowStyle;
	RECT mWindowBounds;
	RECT mWindowClient;
	const TCHAR* mClassName;

	std::unique_ptr<Graphics> mGFX;
	Keyboard mKeyBoard;
	Mouse mMouse;
};

