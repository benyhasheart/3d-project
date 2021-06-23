#include "Graphics.h"
#include "DxErr.h"

namespace wrl = Microsoft::WRL;
//namespace dx = DirectX;

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "D3DCompiler.lib")


Graphics::Graphics()
{
	mDriverType = D3D_DRIVER_TYPE_NULL;
	mFeatrueLevel = D3D_FEATURE_LEVEL_11_0;
	mViewPort = {0};
	mIsFullScreenMode = false;
	mViewMatrix = DirectX::XMMatrixIdentity();
	mProjectionMatrix = DirectX::XMMatrixIdentity();
}

Graphics::~Graphics()
{
	mSwapChain->SetFullscreenState(false, NULL);
}

HRESULT Graphics::createDevice()
{
	HRESULT hr = S_OK;
	UINT createDeviceFlags = 0u;
#ifdef IS_DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !_DEBUG

	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_UNKNOWN,
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT driverTypesCount = sizeof( driverTypes ) / sizeof( driverTypes[0]);
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};

	UINT featureLevelsCount =  sizeof(featureLevels) / sizeof(featureLevels[0]);

	for (UINT driverTypeIndex = 0u; driverTypeIndex < driverTypesCount; driverTypeIndex++)
	{
		mDriverType = driverTypes[driverTypeIndex];
		if (SUCCEEDED(hr = D3D11CreateDevice(
			NULL, mDriverType, NULL,
			createDeviceFlags, featureLevels, featureLevelsCount, D3D11_SDK_VERSION,
			mDevice.GetAddressOf(),
			&mFeatrueLevel,
			mDeviceContext.GetAddressOf())))
		{
			if (FAILED(hr) || mFeatrueLevel < D3D_FEATURE_LEVEL_11_0)
			{
				mDeviceContext->Release();
				mDevice->Release();
			}
			break;
		}
	}

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

HRESULT Graphics::createGIFactory()
{
	HRESULT hr = S_OK;

	if ( mDevice == nullptr) return hr = E_FAIL;
	/*if (FAILED(hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(mGiFactory.GetAddressOf()) )))
	{
		return hr;
	}*/

	wrl::ComPtr<IDXGIDevice> pDXGIDevice;
	hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(pDXGIDevice.GetAddressOf()));

	wrl::ComPtr<IDXGIAdapter> pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(pDXGIAdapter.GetAddressOf()));

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(mGiFactory.GetAddressOf()));

	return hr;
}

HRESULT Graphics::createSwapChain(HWND hWnd, UINT width, UINT height, bool isFullScreen)
{
	HRESULT hr = S_OK;
	SetFullScreenFlag(isFullScreen);
	if( mGiFactory == nullptr) return hr = S_FALSE;
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1u;
	sd.SampleDesc.Quality = 0u;
	sd.Windowed = !mIsFullScreenMode;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(hr = mGiFactory->CreateSwapChain(mDevice.Get(), &sd, mSwapChain.GetAddressOf())))
	{
		return hr;
	}
	return hr;
}

HRESULT Graphics::CreateRenderTargetView()
{
	HRESULT hr = S_OK;

	wrl::ComPtr<ID3D11Texture2D> backbuffer;

	hr = mSwapChain->GetBuffer(0u, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backbuffer.GetAddressOf()));
	if ( FAILED(hr))
		return hr;

	hr = mDevice->CreateRenderTargetView( backbuffer.Get(), NULL, mRenderTargetView.GetAddressOf());
	
	if (FAILED(hr))
		return hr;

	
	
	return hr;
}
HRESULT Graphics::CreateDepthStensilView()
{
	HRESULT hr = S_OK;
	
	wrl::ComPtr<ID3D11Texture2D> depthStencilTexture;
	D3D11_TEXTURE2D_DESC	descDepth = {};
	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView = {};

	DXGI_SWAP_CHAIN_DESC descSwapChain = {};

	
	hr = mSwapChain->GetDesc(&descSwapChain);
	if (FAILED(hr))
		return hr;

	descDepth.Width = descSwapChain.BufferDesc.Width;
	descDepth.Height = descSwapChain.BufferDesc.Height;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0u;
	descDepth.MiscFlags = 0u;
	
	hr = mDevice->CreateTexture2D(&descDepth, nullptr, depthStencilTexture.GetAddressOf());
	
	if (FAILED(hr))
		return hr;

	descDepthStencilView.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepthStencilView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Texture2D.MipSlice = 0u;

	hr = mDevice->CreateDepthStencilView(depthStencilTexture.Get(), &descDepthStencilView, mDepthStencilView.GetAddressOf());
	
	if (FAILED(hr))
		return hr;

	//create depth stensil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = true;
	wrl::ComPtr<ID3D11DepthStencilState> DSState;
	mDevice->CreateDepthStencilState(&dsDesc, &DSState);

	//bind depth state
	mDeviceContext->OMSetDepthStencilState(DSState.Get(), 1u);

	return hr;
}
HRESULT Graphics::CreateViewPort()
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC desc;

	mSwapChain->GetDesc(&desc);

	mViewPort.Width = desc.BufferDesc.Width;
	mViewPort.Height = desc.BufferDesc.Height;
	/*mViewPort.Width = 600;
	mViewPort.Height = 400;*/
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	mViewPort.TopLeftX = 0;
	mViewPort.TopLeftY = 0;


	return hr;
}
bool	Graphics::CleanUpDevice()
{
	//프로그램을 닫기전에 창모드로 전환 후 닫는다. 메모리 누수 방지.
	mSwapChain->SetFullscreenState(false,NULL);
	mRenderTargetView->Release();
	mDepthStencilView->Release();
	mSwapChain->Release();
	mDeviceContext->Release();
	mDevice->Release();
	return true;
}

HRESULT Graphics::Initialize(HWND hWnd, UINT width, UINT height, bool isFullScreen)
{
	HRESULT hr = S_OK;

	createDevice();
	createGIFactory();
	createSwapChain(hWnd ,width, height, isFullScreen);
	CreateViewPort();
	CreateRenderTargetView();
	CreateDepthStensilView();

	mDeviceContext->RSSetViewports(1, &mViewPort);
	//mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), nullptr);
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	// Alt + Enter 키를 막는다.
	if (FAILED(hr = mGiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER)))
	{
		return hr;
	}

	return hr;
}

void Graphics::ClearBuffer(float r, float g, float b, float a)
{
	const float clearColor[4] = { r,g,b,a};
	mDeviceContext->ClearRenderTargetView( mRenderTargetView.Get(), clearColor);
	mDeviceContext->ClearDepthStencilView( mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void Graphics::EndFrame()
{
	mSwapChain->Present(1u,0u);
}

HRESULT Graphics::ResizeDevice(UINT width, UINT height)
{
	

	HRESULT hr = S_OK;

	if (mDevice == nullptr) return S_FALSE;
	//release RenderTarget and depthStensil.

	mDeviceContext->OMSetRenderTargets( 0, nullptr, nullptr);
	if ( mRenderTargetView ) mRenderTargetView->Release();
	if ( mDepthStencilView ) mDepthStencilView->Release();

	//resize backBuffer

	DXGI_SWAP_CHAIN_DESC currentDesc;
	mSwapChain->GetDesc(&currentDesc);
	if ( FAILED( hr = mSwapChain->ResizeBuffers( currentDesc.BufferCount,
											width, height,
											currentDesc.BufferDesc.Format,
											currentDesc.Flags) ))
	{
		return hr;
	}

	
	if (FAILED(hr = CreateRenderTargetView()))
	{
		return hr;
	}

	if (FAILED(hr = CreateDepthStensilView()))
	{
		return hr;
	}


	if (FAILED(hr = CreateViewPort()))
	{
		return hr;
	}

	mDeviceContext->RSSetViewports(1, &mViewPort);
	mDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	return hr;

}

HRESULT Graphics::IsFullScreen(BOOL isFull)
{
	HRESULT hr = S_OK;
	
	BOOL isScreenMode = false;
	mSwapChain->GetFullscreenState(&isScreenMode,nullptr);

	if (isScreenMode != isFull)
	{
		SetFullScreenFlag(isFull);
		mSwapChain->SetFullscreenState(isFull,nullptr);
	}

	return hr;
}

void Graphics::SetFullScreenFlag(bool isFullScreen)
{
	mIsFullScreenMode = isFullScreen;
}

bool Graphics::GetFullScreenFlag()
{
	return mIsFullScreenMode;
}

ID3D11Device* Graphics::GetDevice()
{
	return mDevice.Get();
}

ID3D11DeviceContext* Graphics::GetDeviceContext()
{
	return mDeviceContext.Get();
}

IDXGISwapChain* Graphics::GetSwapChain()
{
	return mSwapChain.Get();
}


void Graphics::SetViewMatrix(DirectX::XMMATRIX src) noexcept
{
	mViewMatrix = src;
}
const DirectX::XMMATRIX Graphics::GetViewMatrix() const noexcept
{
	return mViewMatrix;
}

void Graphics::SetProjectionMatrix(DirectX::XMMATRIX src) noexcept
{
	mProjectionMatrix = src;
}
const DirectX::XMMATRIX Graphics::GetProjectionMatrix() const noexcept
{
	return mProjectionMatrix;
}

ID3D11RenderTargetView* Graphics::GetRenderTargetView() const noexcept
{
	return mRenderTargetView.Get();
}