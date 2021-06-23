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
    // �۾�����(  Ÿ��Ʋ ��/��輱/�޴�/��ũ�� �� ���� ������ ������ ����), ������ ��Ÿ��, �޴�����
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    mhWnd = CreateWindow(mClassName, windowTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,this);
    if (!mhWnd)
        return false;


    // ������ ��Ÿ���� �����ϰ� ��ũ�� ������ Ŭ���̾�Ʈ ������ ��´�.
    mWindowStyle = GetWindowLong(mhWnd, GWL_STYLE);
    GetWindowRect(mhWnd, &mWindowBounds);
    GetClientRect(mhWnd, &mWindowClient);

    CenterWindow(mhWnd);
    UpdateWindow(mhWnd);

    // ������ Ŭ���̾�Ʈ ���̿� ���̸� ��´�.
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
    // ȭ�� ��ũ���� �ػ�(���̿� ����)�� ��´�.
    int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

    // ������ Ŭ���̾�Ʈ �߾Ӱ� ȭ�� ��ũ�� �߾��� �����.
    int iDestX = (iScreenWidth - (mWindowBounds.right - mWindowBounds.left)) / 2;
    int iDestY = (iScreenHeight - (mWindowBounds.bottom - mWindowBounds.top)) / 2;

    // �����츦 ȭ���߾����� �̵���Ų��.
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
      
        if (SIZE_RESTORED == wParam) // ������ â ũ�Ⱑ ��������
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            mGFX->ResizeDevice(width, height);
        }
        else if (SIZE_MAXIMIZED == wParam) // ������ â �ִ�ũ��. Ǯ ��ũ���� âũ�� �ִ�ȭ�� �޽����� ���� �Ұ�. 
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            // fullscreen �б�
            // mGFX->IsFullScreen(true); // fullscreen ����
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
