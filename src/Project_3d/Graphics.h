#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Graphics
{
public:
	Graphics();
	Graphics(Graphics& src ) =delete;
	Graphics& operator=(Graphics& src) = delete;
	~Graphics();

public:
	// first call func. createDevice ... etc
	HRESULT Initialize(HWND hWnd, UINT width = 800, UINT height = 600, bool isFullScreen = false);
	//before begin render.
	void ClearBuffer(float r, float g, float b, float a);
	//begin render
	void EndFrame();

	HRESULT ResizeDevice(UINT width, UINT height);
	//change swapchain description.
	HRESULT IsFullScreen(BOOL isFull);

public:
	void SetFullScreenFlag(bool isFullScreen);
	bool GetFullScreenFlag();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

	ID3D11RenderTargetView* GetRenderTargetView() const noexcept;
	// -- temporary func
	
	void SetViewMatrix(DirectX::XMMATRIX src) noexcept;
	const DirectX::XMMATRIX GetViewMatrix() const noexcept;

	void SetProjectionMatrix(DirectX::XMMATRIX src ) noexcept;
	const DirectX::XMMATRIX GetProjectionMatrix() const noexcept;
	
	// -- end
	
// create device and etc..
public:
	HRESULT CreateRenderTargetView();
	HRESULT CreateDepthStensilView();
	HRESULT CreateViewPort();
	bool	CleanUpDevice();

private:
	HRESULT createDevice();
	HRESULT createGIFactory();
	HRESULT createSwapChain(HWND hWnd, UINT width, UINT height, bool isFullScreen);

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			mDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		mDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			mSwapChain;
	Microsoft::WRL::ComPtr<IDXGIFactory>			mGiFactory;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;
	D3D11_VIEWPORT									mViewPort;
	D3D_DRIVER_TYPE									mDriverType;
	D3D_FEATURE_LEVEL								mFeatrueLevel;

	DirectX::XMMATRIX								mProjectionMatrix;
	DirectX::XMMATRIX								mViewMatrix;

	bool											mIsFullScreenMode;

};

