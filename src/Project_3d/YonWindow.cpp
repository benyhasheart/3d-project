#include "YonWindow.h"
#include "imgui_impl_win32.h"

HWND g_hwnd;
YonWindow::YonWindow()
{
	mhInstance = NULL;
	mhWnd = NULL;
	mWindowStyle = 0;
	mWindowBounds = {0};
    mWindowClient = {0};
	windowWidth = 0;
	windowHeight = 0;
    mClassName = L"YonWindow";
}

YonWindow::~YonWindow()
{
    ::DestroyWindow(mhWnd);
    ::UnregisterClass(mClassName, mhInstance);
}


bool YonWindow::InitWindow(HINSTANCE hInstance, const TCHAR* windowTitle, int nCmdShow)
{
    mhInstance = hInstance;
    if (mhInstance == NULL)
    {
        mhInstance = GetModuleHandle(nullptr);
    }

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wndProcSetup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = mClassName;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    if (!RegisterClassEx(&wcex))
        return false;

    // Create window
   
    RECT rc = { 0, 0, 800, 600 };
    // 작업영역(  타이틀 바/경계선/메뉴/스크롤 바 등의 영역을 제외한 영역), 윈도우 스타일, 메뉴여부
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    mhWnd = CreateWindow(mClassName, windowTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,this);
    if (!mhWnd)
        return false;


    // 윈도우 스타일을 저장하고 스크린 영역과 클라이언트 영역을 얻는다.
    mWindowStyle = GetWindowLong(mhWnd, GWL_STYLE);
    GetWindowRect(mhWnd, &mWindowBounds);
    GetClientRect(mhWnd, &mWindowClient);

    CenterWindow(mhWnd);
    UpdateWindow(mhWnd);

    // 윈도우 클라이언트 넓이와 높이를 얻는다.
    windowWidth = mWindowClient.right - mWindowClient.left;
    windowHeight = mWindowClient.bottom - mWindowClient.top;
    // graphics
    mGFX = std::make_unique<Graphics>();
    mGFX->Initialize(mhWnd);

    g_hwnd = mhWnd;
    ShowWindow(mhWnd, nCmdShow);

    return false;
}

void YonWindow::CenterWindow(HWND hWnd)
{
    // 화면 스크린의 해상도(넓이와 높이)을 얻는다.
    int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

    // 윈도우 클라이언트 중앙과 화면 스크린 중앙을 맞춘다.
    int iDestX = (iScreenWidth - (mWindowBounds.right - mWindowBounds.left)) / 2;
    int iDestY = (iScreenHeight - (mWindowBounds.bottom - mWindowBounds.top)) / 2;

    // 윈도우를 화면중앙으로 이동시킨다.
    MoveWindow(hWnd, iDestX, iDestY,
        mWindowBounds.right - mWindowBounds.left,
        mWindowBounds.bottom - mWindowBounds.top,
        true);
}

std::optional<int> YonWindow::ProcessMessages()
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return (int)msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}
 Graphics& YonWindow::GetGFX() const noexcept
{
    return *mGFX;
}

Keyboard& YonWindow::GetKeyboard()  noexcept
{
    return mKeyBoard;
}

Mouse& YonWindow::GetMouse() noexcept
{
   return mMouse;
}

LRESULT YonWindow::wndProcSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCCREATE)
    {
        const CREATESTRUCT* const createData = reinterpret_cast<CREATESTRUCT*>(lParam);
        YonWindow* const wnd = static_cast<YonWindow*>(createData->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
        //set message proc
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&YonWindow::handleMsgThunk));

        return wnd->wndProc(hWnd, message, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, default handler
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT YonWindow::handleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    YonWindow* const wnd = reinterpret_cast<YonWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    return wnd->wndProc(hWnd, msg, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT YonWindow::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) noexcept
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_SIZE:
      
        if (SIZE_RESTORED == wParam) // 윈도우 창 크기가 변했을때
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            mGFX->ResizeDevice(width, height);
        }
        else if (SIZE_MAXIMIZED == wParam) // 윈도우 창 최대크기. 풀 스크린과 창크기 최대화는 메시지로 구분 불가. 
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            // fullscreen 분기
            // mGFX->IsFullScreen(true); // fullscreen 변경
            mGFX->ResizeDevice(width, height);
        }
    break;
    case WM_KILLFOCUS:
        mKeyBoard.ClearState();
        break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        if ((HIWORD(lParam) & KF_REPEAT) == 0)
        {
            mKeyBoard.KeyPressed(static_cast<unsigned char>(wParam));
        }
        
    }
    break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        mKeyBoard.KeyReleased(static_cast<unsigned char>(wParam));
    }
    break;

    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mMouse.SetMove(pt.x,pt.y);
    }
    break;

    case WM_LBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mMouse.OnLeftPressed(pt.x, pt.y);
    }
    break;

    case WM_LBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mMouse.OnLeftRelease(pt.x, pt.y);
    }
    break;


    case WM_RBUTTONDOWN:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mMouse.OnRightPressed(pt.x, pt.y);
    }
    break;

    case WM_RBUTTONUP:
    {
        const POINTS pt = MAKEPOINTS(lParam);
        mMouse.OnRightRelease(pt.x, pt.y);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
