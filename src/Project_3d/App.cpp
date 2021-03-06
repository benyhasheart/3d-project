#include <d3dcompiler.h>
#include <random>

#include "App.h"
#include "EnumData.h"
#include "FirstPersonViewCamera.h"
#include "Matrix.h"
#include "ModelViewCamera.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

extern HWND g_hwnd;
App::App()
	:
	mCurrentSelectTerrain(nullptr)
{
	Initialize();

}

int App::Excute()
{

	while (true)
	{
		if (const auto e = mWindow.ProcessMessages())
		{
			return e.value();
		}
		Update();
		Render();
	}

}


bool App::Initialize()
{
	mWindow.InitWindow(NULL, L"YonWindow", 800, 600, SW_SHOWDEFAULT);

#pragma region imgui
	mUi = std::make_unique<mydx::UI>(mWindow.GetGFX());
	App* ap = this;
	mUi->SetOwenrApp(ap);
#pragma endregion imgui

	//create vertex

	HRESULT hr = S_OK;


	//VerTex vertices[] = 
	//{
	//	-0.5f, 0.5f, 0.5f,
	//	0.5f, 0.5f, 0.5f,
	//	0.5f, -0.5f, 0.5f,
	//	-0.5f, -0.5f, 0.5f,
	//};

	/*mStride = sizeof(VerTex);
	UINT countVertices = sizeof( vertices ) / sizeof( vertices[0] );

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof( VerTex) * countVertices;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = vertices;
	mWindow.GetGFX().GetDevice()->CreateBuffer(&desc, &subData, mVertexBuffer.GetAddressOf());*/


	/*std::vector<VerTex> vertexData =
	{
		{-0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f},
	};
	mVertexBufferClass = std::make_unique<VertexBuffer>(mWindow.GetGFX(), vertexData);*/

	//create indexBuffer

	//WORD indices[] = 
	//{
	//	0,1,2,
	//	0,2,3,
	//};


	/*UINT countIndex = sizeof(indices) /sizeof(indices[0]);

	D3D11_BUFFER_DESC indexDesc;
	indexDesc.ByteWidth =  sizeof(WORD) * countIndex;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexSubData;
	indexSubData.pSysMem = indices;
	mWindow.GetGFX().GetDevice()->CreateBuffer( &indexDesc, &indexSubData, mIndexBuffer.GetAddressOf());*/



	// create constantBuffer

	//16 byte 32.. 48... 16 byte unit translate.
	//struct ConstantData
	//{
	//	float x;
	//	float y;
	//	float z;
	//	float w;
	//};

	//D3D11_BUFFER_DESC constnatDesc;
	//constnatDesc.ByteWidth = sizeof(ConstantData);
	//constnatDesc.Usage = D3D11_USAGE_DYNAMIC; // use cpu Access.
	//constnatDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//constnatDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // use cpu Access.
	//constnatDesc.MiscFlags = 0;

	//mWindow.GetGFX().GetDevice()->CreateBuffer( &constnatDesc, nullptr, mConstantBuffer.GetAddressOf());

	//mVertexConstantBuffer = std::make_unique<VertexConstantBuffer<ConstantData>>(mWindow.GetGFX());




//	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#if defined( _DEBUG )
//	shaderFlags = D3DCOMPILE_DEBUG;
//#endif
//	
//ID3DBlob* VertexshaderBuffer = nullptr;
//	ID3DBlob* PixelshaderBuffer = nullptr;
//	ID3DBlob* errorMsg = nullptr;
//	// vertex shader
//	hr = D3DCompileFromFile(L"VertexShader.vsh",NULL,NULL,"main","vs_5_0", shaderFlags,NULL, &VertexshaderBuffer,&errorMsg);
//	mWindow.GetGFX().GetDevice()->CreateVertexShader(VertexshaderBuffer->GetBufferPointer(),
//		VertexshaderBuffer->GetBufferSize(),
//													nullptr,mVertexShader.GetAddressOf() );

	/*D3DReadFileToBlob(L"VertexShader.vsh", &VertexshaderBuffer);
	mWindow.GetGFX().GetDevice()->CreateVertexShader (
		VertexshaderBuffer->GetBufferPointer(),
		VertexshaderBuffer->GetBufferSize(),
		nullptr,
		&mVertexShader
	);*/

	// pixelShader
	/*hr = D3DCompileFromFile(L"PixelShader.psh", NULL, NULL, "main", "ps_5_0", shaderFlags, NULL, &PixelshaderBuffer, &errorMsg);
	mWindow.GetGFX().GetDevice()->CreatePixelShader(PixelshaderBuffer->GetBufferPointer(),
		PixelshaderBuffer->GetBufferSize(),
		nullptr, mPixelShader.GetAddressOf());*/

		/*D3DReadFileToBlob(L"PixelShader.psh", &PixelshaderBuffer);
		mWindow.GetGFX().GetDevice()->CreatePixelShader(
			PixelshaderBuffer->GetBufferPointer(),
			PixelshaderBuffer->GetBufferSize(),
			nullptr,
			&mPixelShader
		);*/

	mVertexShaderClass = std::make_unique<VertexShader>(mWindow.GetGFX(), L"VertexShader.vsh", "main");
	mPixelShaderClass = std::make_unique<PixelShader>(mWindow.GetGFX(), L"PixelShader.psh", "main");
	//mGeometryShaderClass = std::make_unique<GeometryShader>(mWindow.GetGFX(), L"GeometryShader.gsh", "main");

	//inputlayout
	//const D3D11_INPUT_ELEMENT_DESC layout[] = 
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//};

	//hr = mWindow.GetGFX().GetDevice()->CreateInputLayout(layout, 1, mVertexShaderClass->GetBytecodeBlob()->GetBufferPointer(), mVertexShaderClass->GetBytecodeBlob()->GetBufferSize(),
	//&mInputLayout );

	//const std::vector< D3D11_INPUT_ELEMENT_DESC> layoutList = 
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//};

	//mInputLayoutClass = std::make_unique<InputLayout>(mWindow.GetGFX(), layoutList, mVertexShaderClass->GetBytecodeBlob());

	mTopologyClass = std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_RASTERIZER_DESC RSdesc = {};

	RSdesc.DepthClipEnable = TRUE;
	RSdesc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME
	//RSdesc.FillMode = D3D11_FILL_WIREFRAME;
	RSdesc.CullMode = D3D11_CULL_BACK;
	//RSdesc.ScissorEnable = TRUE; // ?????? ???? ?????? ???? ?????? ???? (OM???? ???? ???? ????)
	mWindow.GetGFX().GetDevice()->CreateRasterizerState(&RSdesc, mRasterizerStateSolid.GetAddressOf());

	RSdesc.FillMode = D3D11_FILL_WIREFRAME;
	mWindow.GetGFX().GetDevice()->CreateRasterizerState(&RSdesc, mRasterizerStateWireFrame.GetAddressOf());

	//mWindow.GetGFX().GetDeviceContext()->RSSetState(mRasterizerStateSolid.Get());
	mWindow.GetGFX().GetDeviceContext()->RSSetState(mRasterizerStateSolid.Get());

#pragma region LoadTextureFromFile

	//hr = mWindow.GetGFX().GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mBackBuffer.GetAddressOf()));

	//mBackBuffer->GetDesc(&mDescBackBuffer);

	///*HRESULT __cdecl CreateWICTextureFromFile(
	//_In_ ID3D11Device* d3dDevice,
	//_In_z_ const wchar_t* szFileName,
	//_Outptr_opt_ ID3D11Resource** texture,
	//_Outptr_opt_ ID3D11ShaderResourceView** textureView,
	//_In_ size_t maxsize = 0) noexcept;*/

	//ID3D11Resource* resource;
	//hr = DirectX::CreateWICTextureFromFile(mWindow.GetGFX().GetDevice(), L"../../data/etcTexture/desert.jpg", &resource, nullptr, 800u);

	//if (FAILED(hr))
	//{
	//	return;
	//}

	//resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mTexture1.GetAddressOf()));
	//resource->Release();

	//mTexture1->GetDesc(&mDescTexture1);


	//hr = DirectX::CreateWICTextureFromFile(mWindow.GetGFX().GetDevice(), L"../../data/etcTexture/Koala.jpg", &resource, nullptr, 400u);

	//if (FAILED(hr))
	//{
	//	return;
	//}

	//resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mTexture2.GetAddressOf()));
	//resource->Release();

	//mTexture2->GetDesc(&mDescTexture2);


	//mDescTexture1.Format = mDescBackBuffer.Format;

#pragma endregion LoadTextureFromFile

#pragma region TextureMappingToObject

	/*std::vector<VerTex> vertexData =
	{
		{-0.5f, 0.5f, 0.5f,1.0f, 0.0f,0.0f},
		{0.5f, 0.5f, 0.5f,1.0f, 1.0f,0.0f},
		{0.5f, -0.5f, 0.5f,1.0f, 1.0f,1.0f},
		{-0.5f, -0.5f, 0.5f,1.0f, 0.0f,1.0f},
	};*/
	/*std::vector<VerTex> vertexData =
	{
		{-1.0f,  1.0f,  0.5f,1.0f,0.0f,0.0f},
		{1.0f,  1.0f, 0.5f,1.0f,3.0f, 0.0f},
		{1.0f, -1.0f, 0.5f,1.0f,3.0f,3.0f},
		{-1.0f, -1.0f, 0.5f,1.0f,0.0f,3.0f},
	};*/

	/*ID3D11Resource* resource;
	hr = DirectX::CreateWICTextureFromFile(mWindow.GetGFX().GetDevice(), L"../../data/etcTexture/desert.jpg", &resource, mShaderResourceView.GetAddressOf(), 0u);

	if (FAILED(hr))
	{
		return;
	}

	resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mTexture1.GetAddressOf()));
	resource->Release();

	mTexture1->GetDesc(&mDescTexture1);*/

#pragma endregion TextureMappingToObject

#pragma region	SamplerState

	mSamplerDesc = {};
	mSamplerDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(1);
	mSamplerDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(1);
	mSamplerDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(1);
	mSamplerDesc.BorderColor[0] = 1.0f;
	mSamplerDesc.BorderColor[1] = 0.0f;
	mSamplerDesc.BorderColor[2] = 0.0f;
	mSamplerDesc.BorderColor[3] = 1.0f;
	mSamplerDesc.MaxAnisotropy = 16;

	//unsigned int minmapCount = mTexture->GetShaderResourceViewDesc().Texture2D.MipLevels;
	//mSamplerDesc.MinLOD = static_cast<float>(minmapCount);
	////mSamplerDesc.MinLOD = static_cast<float>(5);
	//mSamplerDesc.MaxLOD = mSamplerDesc.MinLOD + 2;
	mSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	mWindow.GetGFX().GetDevice()->CreateSamplerState(&mSamplerDesc, mSamplerState.GetAddressOf());

#pragma endregion SamplerState

#pragma region ViewPort

	mViewPort[0].TopLeftX = 0;
	mViewPort[0].TopLeftY = 0;
	mViewPort[0].Width = 400;
	mViewPort[0].Height = 600;
	mViewPort[0].MinDepth = 0.0f;
	mViewPort[0].MaxDepth = 1.0f;

	mViewPort[1].TopLeftX = 400;
	mViewPort[1].TopLeftY = 0;
	mViewPort[1].Width = 400;
	mViewPort[1].Height = 300;
	mViewPort[1].MinDepth = 0.0f;
	mViewPort[1].MaxDepth = 1.0f;

	mViewPort[2].TopLeftX = 0;
	mViewPort[2].TopLeftY = 300;
	mViewPort[2].Width = 400;
	mViewPort[2].Height = 300;
	mViewPort[2].MinDepth = 0.0f;
	mViewPort[2].MaxDepth = 1.0f;

	mViewPort[3].TopLeftX = 400;
	mViewPort[3].TopLeftY = 300;
	mViewPort[3].Width = 400;
	mViewPort[3].Height = 300;
	mViewPort[3].MinDepth = 0.0f;
	mViewPort[3].MaxDepth = 1.0f;
#pragma endregion ViewPort

#pragma region	View, Projection transform

	//mCamera = std::make_unique<ModelViewCamera>();
	mCamera = std::make_shared<FirstPersonViewCamera>();
	mCamera->CreateViewMatrix(
		::XMVectorSet(0.0f, 10.0f, -10.0f, 0.0f),
		::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	mWindow.GetGFX().SetViewMatrix(mCamera.get()->GetViewMatrix());
	float aspect = static_cast<float>(mWindow.windowWidth) / static_cast<float>(mWindow.windowHeight);
	mWindow.GetGFX().SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspect, 1.0f, 2000.0f));
	mCamera.get()->SetProjectionMatrix(mWindow.GetGFX().GetProjectionMatrix());

	mCamera.get()->CreateFrustum(mWindow.GetGFX());

	mTopViewCamera = std::make_unique<Camera>();
	mTopViewCamera->CreateViewMatrix(
		::XMVectorSet(0.0f, 50.0f, 0.0f, 0.0f),
		::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)
	);

	// mBoxShape = std::make_unique<Box>(mWindow.GetGFX());
#pragma endregion

#pragma region stencil state

	mDepthStencilDesc = {};

	mDepthStencilDesc.DepthEnable = TRUE;
	mDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	mDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	mDepthStencilDesc.StencilEnable = TRUE;
	mDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	mDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	//1?? ???????? ?????? ??????
	mDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
	mDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	mDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	mDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	mWindow.GetGFX().GetDevice()->CreateDepthStencilState(&mDepthStencilDesc, mStencilState1.GetAddressOf());

	mWindow.GetGFX().GetDeviceContext()->OMSetDepthStencilState(mStencilState1.Get(), 1u);

	//2?? ???????? ?????? ??????
	mDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
	mDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	mDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER;
	mDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;


	mWindow.GetGFX().GetDevice()->CreateDepthStencilState(&mDepthStencilDesc, mStencilState2.GetAddressOf());

	//mWindow.GetGFX().GetDeviceContext()->OMSetDepthStencilState(mStencilState2.Get(), 1u);

#pragma endregion

	mBoundingBox = std::make_unique<mydx::BoundingBox>(mWindow.GetGFX(), ::XMFLOAT3(0.0f, 0.0f, 0.0f), ::XMFLOAT3(1.0f, 1.0f, 1.0f));

	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(-20.0f, 20.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	// mBoxes.reserve(20);
	 /*for (int i = 0; i < 1; i++)
	 {
		 auto box = std::make_shared<Box>(
				mWindow.GetGFX(),
				::XMVECTOR({ 1.0f,1.0f,1.0f }),
				::XMVECTOR({adist(rng),adist(rng) ,adist(rng)}),
				::XMVECTOR({ ddist(rng),ddist(rng) ,ddist(rng) }));
		 mBoxes.emplace_back(box);
	 }*/
	 //auto box = std::make_shared<Box>(
		// mWindow.GetGFX(),
		// ::XMVECTOR({ 1.0f,1.0f,1.0f }),
		// ::XMVECTOR({ 0.0f,0.0f,0.0f }),
		// ::XMVECTOR({ 0.0f,0.0f,0.0f }));
		// mBoxes.emplace_back(box);

	mydx::MapDesc mapDesc = {};
	mapDesc.ColCellCount = 8;
	mapDesc.RowCellCount = 8;
	mapDesc.scale = ::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

	mMap = std::make_shared<mydx::Map>(mWindow.GetGFX(), mapDesc, nullptr);
	mQuardTreeVertexIndex = std::make_shared<mydx::QuardTreeVertexIndex>();
	mQuardTreeVertexIndex->SetMap(mMap.get());
	mQuardTreeVertexIndex->Build(mMap.get(), mMap.get()->GetMapDesc().ColCellCount, mMap.get()->GetMapDesc().RowCellCount);
	mQuardTreeVertexIndex->SetCamera(mCamera.get());

	//mHeightMap = std::make_shared<mydx::HeightMap>(mWindow.GetGFX(),L"../../data/map/WaterHeight.jpg");
	////mQuardTree = std::make_unique<mydx::QuardTreeVertexIndex>();
	////mQuardTree->Build(8,8);
	//mQuardTreeVertexIndex = std::make_shared<mydx::QuardTreeVertexIndex>();
	////mQuardTreeVertexIndex->SetDepth(1);
	//mQuardTreeVertexIndex->SetMap(mHeightMap.get());
	//mQuardTreeVertexIndex->Build(mHeightMap.get(), mHeightMap.get()->GetMapDesc().ColCellCount, mHeightMap.get()->GetMapDesc().RowCellCount);
	////mQuardTreeVertexIndex->Build(mHeightMap.get(), 8,8);
	//mQuardTreeVertexIndex->SetCamera(mCamera.get());

	mSelect = std::make_shared<mydx::Select>();
	mSelect->SetCamera(mCamera.get());

	// mTerrain = std::make_shared<mydx::Terrain>(*g_graphics, mapDesc);
	return false;
}

bool App::Update()
{
	mWindow.GetMouse().Update();


	//struct ConstantData
	//{
	//	float x;
	//	float y;
	//	float z;
	//	float w;
	//};
	//// time
	//static float time = 0.0f;
	//time += 0.0001f;
	//static float balocity = 0.0f;
	//balocity = cosf(time)*0.5f + 0.5f; // -1 ~ 1 -> 0 ~ 1

	////constnatBuffer update
	///*D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	//	mWindow.GetGFX().GetDeviceContext()->Map( mConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	//auto constantData = reinterpret_cast<ConstantData*>(mappedSubresource.pData);
	//constantData->z = balocity;
	//mWindow.GetGFX().GetDeviceContext()->Unmap( mConstantBuffer.Get(), 0);*/
	////--

	//static ConstantData conData = {};
	//conData.z = balocity;

	//// --
	//UINT offset = 0u;
	////mWindow.GetGFX().GetDeviceContext()->IASetVertexBuffers( 0, 1u, mVertexBuffer.GetAddressOf(), &mStride, &offset);
	//mVertexBufferClass->Bind(mWindow.GetGFX());
	////mWindow.GetGFX().GetDeviceContext()->IASetIndexBuffer( mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	//mIndexBufferClass->Bind(mWindow.GetGFX());
	////mWindow.GetGFX().GetDeviceContext()->IASetInputLayout( mInputLayout.Get());
	//mInputLayoutClass->Bind(mWindow.GetGFX());
	////mWindow.GetGFX().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//mTopologyClass->Bind(mWindow.GetGFX());

	////mWindow.GetGFX().GetDeviceContext()->VSSetShader( mVertexShader.Get(), nullptr, 0u);
	//mVertexShaderClass->Bind(mWindow.GetGFX());
	////mWindow.GetGFX().GetDeviceContext()->VSSetConstantBuffers( 0u, 1u, mConstantBuffer.GetAddressOf());
	////mVertexConstantBuffer->Bind(mWindow.GetGFX());
	//mTransformConstantBuffer2->Bind(mWindow.GetGFX());
	///*mTransformData2.matrixWorld = DirectX::XMMatrixTranslation(1.0f, 0.0f, 0.0f);
	//mTransformData2.matrixWorld = DirectX::XMMatrixTranspose(mTransformData2.matrixWorld);
	//mWindow.GetGFX().GetDeviceContext()->UpdateSubresource(mTransformConstantBuffer2.get()->GetBuffer(), 0, nullptr, &mTransformData2, 0, 0);
	//mTransformData2.matrixWorld = DirectX::XMMatrixTranspose(mTransformData2.matrixWorld);*/

	//mWindow.GetGFX().GetDeviceContext()->HSSetShader( nullptr, nullptr, 0u);
	//mWindow.GetGFX().GetDeviceContext()->DSSetShader( nullptr, nullptr, 0u);
	////mWindow.GetGFX().GetDeviceContext()->GSSetShader( nullptr, nullptr, 0u);
	////mGeometryShaderClass->Bind(mWindow.GetGFX());



	///*D3D11_RECT rects[1];
	//rects[0].left = 0;
	//rects[0].right = 640;
	//rects[0].top = 0;
	//rects[0].bottom = 480 / 2;
	//mWindow.GetGFX().GetDeviceContext()->RSSetScissorRects(1u, rects);*/

	////mWindow.GetGFX().GetDeviceContext()->PSSetShader( mPixelShader.Get(), nullptr, 0u);
	////mPixelShaderClass->Bind(mWindow.GetGFX());
	////mWindow.GetGFX().GetDeviceContext()->PSSetShaderResources(0u,1u,mShaderResourceView.GetAddressOf());
	///*mTexture->Bind(mWindow.GetGFX());
   // mWindow.GetGFX().GetDeviceContext()->PSSetSamplers(0u, 1u, mSamplerState.GetAddressOf());

	//mWindow.GetGFX().GetDeviceContext()->DrawIndexed( 6, 0u, 0u);*/

	//// second plane
	///*TransformConstantData2 data = mTransformData2;*/

	////data.matrixWorld = DirectX::XMMatrixRotationY(t) * DirectX::XMMatrixTranslation(1.0f, 0.0f, 0.0f) * mTransformData2.matrixWorld;
	////data.matrixWorld = DirectX::XMMatrixTranspose(data.matrixWorld);
	///*mWindow.GetGFX().GetDeviceContext()->UpdateSubresource(mTransformConstantBuffer2.get()->GetBuffer(), 0, nullptr, &data, 0, 0);
	//mWindow.GetGFX().GetDeviceContext()->DrawIndexed(6, 0u, 0u);*/

	///*mWindow.GetGFX().GetDeviceContext()->RSSetViewports(1, &mViewPort[0]);
	//mWindow.GetGFX().GetDeviceContext()->DrawIndexed(6,0u, 0u);

	//mWindow.GetGFX().GetDeviceContext()->RSSetViewports(1, &mViewPort[1]);
	//mWindow.GetGFX().GetDeviceContext()->DrawIndexed(6, 0u, 0u);

	//mWindow.GetGFX().GetDeviceContext()->RSSetViewports(1, &mViewPort[2]);
	//mWindow.GetGFX().GetDeviceContext()->DrawIndexed(6, 0u, 0u);

	//mWindow.GetGFX().GetDeviceContext()->RSSetViewports(1, &mViewPort[3]);
	//mWindow.GetGFX().GetDeviceContext()->DrawIndexed(6, 0u, 0u);*/

	//// --
	//

	////texture copy


	////mWindow.GetGFX().GetDeviceContext()->CopyResource(mBackBuffer.Get(), mTexture1.Get());

	////D3D11_BOX sourceRegion;
	////sourceRegion.left = 0;					// ???????? ???? ???? X
	////sourceRegion.right = mDescTexture2.Width; // ???????? ??   ???? X
	////sourceRegion.top = 0;					// ???????? ???? ???? Y
	////sourceRegion.bottom = mDescTexture2.Height;// ???????? ??   ???? Y
	////sourceRegion.front = 0;
	////sourceRegion.back = 1;

	////mWindow.GetGFX().GetDeviceContext()->CopySubresourceRegion(mBackBuffer.Get(),0, 0,0,0,mTexture2.Get(), 0,&sourceRegion);

	// --


	//mWindow.GetGFX().GetDeviceContext()->RSSetViewports(1, &mViewPort[0]);
	const auto frameTime = mTimer.Mark();
	static float t = 0;
	t += ((sinf(frameTime) + 1.0f) * 0.5f) * frameTime;

	static ::XMFLOAT4 value = { 0.0f,0.0f,0.0f,5.0f };
	float offset = 0.0f;
	if (mWindow.GetKeyboard().KeyIsPressed('W'))
	{
		offset += 30 * frameTime;
		mCamera->MoveLook(offset);
	}
	else if (mWindow.GetKeyboard().KeyIsPressed('S'))
	{
		offset -= 2 * frameTime;
		mCamera->MoveLook(offset);
	}
	else if (mWindow.GetKeyboard().KeyIsPressed('A'))
	{
		offset -= 2 * frameTime;
		mCamera->MoveSide(offset);

	}
	else if (mWindow.GetKeyboard().KeyIsPressed('D'))
	{
		offset += 2 * frameTime;
		mCamera->MoveSide(offset);
	}
	else if (mWindow.GetKeyboard().KeyIsPressed('Q'))
	{
		offset += 2 * frameTime;
		mCamera->MoveUp(offset);
	}
	else if (mWindow.GetKeyboard().KeyIsPressed('E'))
	{
		offset -= 2 * frameTime;
		mCamera->MoveUp(offset);
	}
	if (mWindow.GetMouse().RightIsPressed())
	{
		const auto offset = mWindow.GetMouse().GetOffsetPostionX();

		::XMFLOAT4 yaw = {};
		yaw.y = offset;
		yaw.x = mWindow.GetMouse().GetOffsetPostionY() * 0.01f;
		::XMFLOAT4 lastPitchYawRoll = {};
		::XMStoreFloat4(&lastPitchYawRoll, mCamera->GetPitchYawRoll());

		auto pitchYawRoll = mCamera->GetPitchYawRoll();

		yaw.y = lastPitchYawRoll.y + offset * 0.01f;
		yaw.x = lastPitchYawRoll.x + mWindow.GetMouse().GetOffsetPostionY() * 0.01f;

		mCamera->SetPitchYawRoll(::XMLoadFloat4(&yaw));
	}

	if (mWindow.GetMouse().LeftIsPressed())
	{

		//mCamera->AddPitchYawRoll(::XMLoadFloat4(&yaw));

		mSelect->Update(mWindow.GetGFX());
		//::XMVECTOR endPoint = mSelect->GetRay().Direction * mSelect->GetRay().Extent;
		////?????? ?????? ?????? ???????? ?????? 3???? ?????? ????????.
		//for (auto box : mBoxes)
		//{
		   // auto& indexTable = box->GetIndexBuffer()->GetIndexTable();
		   // auto& vertexTable = box->GetVertexBuffer()->GetVertexTable();

		   // for (UINT tableIndex = 0; tableIndex < indexTable.size(); )
		   // {
		   //	 ::XMFLOAT4& vertexPosition0 = vertexTable[indexTable[tableIndex]].position;
		   //	 ::XMFLOAT4& vertexPosition1 = vertexTable[indexTable[tableIndex + 1]].position;
		   //	 ::XMFLOAT4& vertexPosition2 = vertexTable[indexTable[tableIndex + 2]].position;

		   //	 //???? ????
		   //	 if (mSelect->CheckInterSection(mSelect->GetRay().Origine, endPoint,
		   //		::XMLoadFloat4(&vertexPosition0),
		   //		 ::XMLoadFloat4(&vertexPosition1),
		   //		 ::XMLoadFloat4(&vertexPosition2)))
		   //	 {
		   //	 // ?????? ?????? ?????????? ????
		   //		 if (mSelect->PointInPolygon(
		   //			 mSelect->GetIntersectingLineSegment(),
		   //			 ::XMLoadFloat4(&vertexPosition0),
		   //			 ::XMLoadFloat4(&vertexPosition1),
		   //			 ::XMLoadFloat4(&vertexPosition2)))
		   //		 {
		   //			 ::XMFLOAT4& vertexColor0 = vertexTable[indexTable[tableIndex]].color;
		   //			 ::XMFLOAT4& vertexColor1 = vertexTable[indexTable[tableIndex + 1]].color;
		   //			 ::XMFLOAT4& vertexColor2 = vertexTable[indexTable[tableIndex + 2]].color;
		   //
		   //			 box->GetVertexBuffer()->Update(mWindow.GetGFX());
		   //			 break;
		   //		 }
		   //	 }
		   //	 

		   //	tableIndex += 3;
		   // }
		//}
		std::vector<DWORD> indexTable;
		mydx::Node* findNode = nullptr;
		if (mCurrentSelectTerrain != nullptr)
		{
			findNode = mCurrentSelectTerrain->FindSelectedNode(nullptr, mSelect.get());
			size_t tableSize = 6;
			indexTable.resize(tableSize);
			if (findNode != nullptr)
			{
				mCurrentSelectTerrain->UpdateIndexTable(0,
					findNode->GetCornerVertexIndexTable()[0],
					findNode->GetCornerVertexIndexTable()[1],
					findNode->GetCornerVertexIndexTable()[2],
					findNode->GetCornerVertexIndexTable()[3],
					indexTable);

				auto& vertexTable = mCurrentSelectTerrain->GetVertexBuffer()->GetVertexTable();
				float increas = 10.0f * mTimer.Mark();
				for (UINT indices = 0; indices < indexTable.size();)
				{
					::XMFLOAT4& vertex0 = vertexTable[indexTable[indices]].position;
					::XMFLOAT4& vertex1 = vertexTable[indexTable[indices + 1]].position;
					::XMFLOAT4& vertex2 = vertexTable[indexTable[indices + 2]].position;
					::XMFLOAT4& vertex3 = vertexTable[indexTable[indices + 4]].position;

					vertex0.y += increas;
					vertex1.y += increas;
					vertex2.y += increas;
					vertex3.y += increas;

					indices += 6;
				}
				mCurrentSelectTerrain->GetVertexBuffer()->Update(mWindow.GetGFX());
			}

		}




	}


	//mCamera->AddPitchYawRoll(::XMLoadFloat4(&yaw));

	::XMVECTOR vector = ::XMLoadFloat4(&value);
	//mCamera->Update(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
	mCamera->Update(mWindow.GetGFX());
	mWindow.GetGFX().SetViewMatrix(mCamera.get()->GetViewMatrix());

	//const auto oldRotationVector = mBoxShape->GetRotation();
	//mBoxShape->SetRotation(::XMVectorSet(t,0.0f,0.0f,0.0f));
   // mBoxShape->Update(mWindow.GetGFX());


   /* mBoundingBox->Update(mWindow.GetGFX());

	for (auto box : mBoxes)
	{
		box->Update(mWindow.GetGFX());
	}*/

	for (auto& terrain : mTerrainList)
	{
		terrain.second->Update(mWindow.GetGFX());
	}

	//mBoxShape->SetRotation(oldRotationVector);
	//mBoxShape->SetRotation(::XMVectorSet(0.0f, t, 0.0f, 0.0f));


	//mWindow.GetGFX().GetDeviceContext()->OMSetDepthStencilState(mStencilState2.Get(), 1u);
	////mWindow.GetGFX().GetDeviceContext()->RSSetViewports(1, &mViewPort[1]);

	//mBoxShape->Update(0.0f);
	//mBoxShape->Draw(mWindow.GetGFX());

	mQuardTreeVertexIndex->Update(mWindow.GetGFX());

	mMap->UpdateIndexBuffer(mWindow.GetGFX(), mQuardTreeVertexIndex->GetUpdateIndexTable());
	mMap->Update(mWindow.GetGFX());

	/*mHeightMap->UpdateIndexBuffer(mWindow.GetGFX(), mQuardTreeVertexIndex->GetUpdateIndexTable());
	mHeightMap->Update(mWindow.GetGFX());*/

	mUi->Update();

	return false;
}
bool App::Render()
{
	mWindow.GetGFX().ClearBuffer(1.0f, 1.0f, 1.0f, 1.0f);
	mWindow.GetGFX().GetDeviceContext()->OMSetDepthStencilState(mStencilState1.Get(), 1u);


	mCamera->Render(mWindow.GetGFX());

	// mBoxShape->Render(mWindow.GetGFX());
	// mBoundingBox->Render(mWindow.GetGFX());

	for (auto& terrain : mTerrainList)
	{
		terrain.second->Render(mWindow.GetGFX());
	}

	/*for (auto& box : mBoxes)
	{
		if (mCamera->GetFrustum().InspectOBBAndPlane(box.get()->GetBoundingBox().GetBoundingBoxData()) == mydx::eCollisionResult::Back)
			continue;
		box->Render(mWindow.GetGFX());
	}*/

	 UINT number = 1u;
	mWindow.GetGFX().GetDeviceContext()->RSGetViewports(&number, &mViewPort[0]);

	mWindow.GetGFX().GetDeviceContext()->RSSetViewports(1, &mViewPort[1]);
	::XMMATRIX oldViewMatrix = mWindow.GetGFX().GetViewMatrix();
	//???????? ???? ??????, ???? ???????? ?? ???????????? ????
	mWindow.GetGFX().SetViewMatrix(mTopViewCamera->GetViewMatrix());
	mCamera->SetViewMatrix(mTopViewCamera->GetViewMatrix());
	mCamera->Update(mWindow.GetGFX());
	mCamera->Render(mWindow.GetGFX());
	/*if (mCamera->GetFrustum().InspectionPoint(mBoxShape->GetTranslation()))
	{
		mBoxShape->Update(mWindow.GetGFX());
		mBoxShape->Render(mWindow.GetGFX());
	}*/
	
	/*for (auto& box : mBoxes)
	{
		if ( mCamera->GetFrustum().InspectOBBAndPlane(box.get()->GetBoundingBox().GetBoundingBoxData()) == mydx::eCollisionResult::Back)
			continue;

		box->Update(mWindow.GetGFX());
		box->Render(mWindow.GetGFX());
	}*/

	
	/*mMap->Update(mWindow.GetGFX());
	mMap->Render(mWindow.GetGFX());*/
	
	//mHeightMap->Update(mWindow.GetGFX());
	//mHeightMap->Render(mWindow.GetGFX());

	/*for (auto& terrain : mTerrainList)
	{
		terrain.second->Update(mWindow.GetGFX());
	}
	for (auto& terrain : mTerrainList)
	{
		terrain.second->Render(mWindow.GetGFX());
	}*/

	
	mCamera->SetViewMatrix(oldViewMatrix);
	
	//mBoundingBox->Update(mWindow.GetGFX());
	//mBoundingBox->Render(mWindow.GetGFX());
	//??????, ?????? ??????
	mWindow.GetGFX().SetViewMatrix(oldViewMatrix);
	mWindow.GetGFX().GetDeviceContext()->RSSetViewports(1, &mViewPort[0]);

	//mWindow.GetGFX().GetDeviceContext()->RSSetState(mRasterizerStateWireFrame.Get());
	//mMap->Update(mWindow.GetGFX());
	//mMap->Render(mWindow.GetGFX());
	//mWindow.GetGFX().GetDeviceContext()->RSSetState(mRasterizerStateSolid.Get());
	/*mHeightMap->Update(mWindow.GetGFX());
	mHeightMap->Render(mWindow.GetGFX());*/
	
	//mHeightMap->Render(mWindow.GetGFX());
	/*mWindow.GetGFX().GetDeviceContext()->RSSetState(mRasterizerStateWireFrame.Get());
	mWindow.GetGFX().GetDeviceContext()->RSSetState(mRasterizerStateSolid.Get());*/

	mUi->Render();

	mWindow.GetGFX().EndFrame();
	return false;
}
bool App::Release()
{
	return false;
}

void App::AddTerrain(std::shared_ptr<mydx::Terrain>& map) noexcept
{
	std::stringstream numbering;
	numbering << "Terrain" << mTerrainList.size() + 1;

	mTerrainList.insert({ numbering.str() ,map });
}

void App::RemoveTerrain(std::string& terrainName) noexcept
{
	auto findIterator = FindTerrain(terrainName);

	if (findIterator != mTerrainList.end())
		mTerrainList.erase(findIterator);

}

std::unordered_map<std::string, std::shared_ptr<mydx::Terrain>>::iterator
App::FindTerrain(std::string& terrainName) noexcept
{
	auto iter = mTerrainList.find(terrainName);

	return iter;
}

void App::SelectTerrainActor(std::string& terrainName) noexcept
{
	auto findIterator = FindTerrain(terrainName);

	if (findIterator != mTerrainList.end())
		mCurrentSelectTerrain = findIterator->second;
}

bool App::BuildTerrain(mydx::MapDesc& mapDesc, std::shared_ptr<Texture> material, std::shared_ptr<Texture> heightMap)
{
	std::shared_ptr<mydx::Terrain> map = std::make_shared<mydx::Terrain>(*g_graphics, mapDesc, material, heightMap);

	if (map == nullptr)
		return false;

	AddTerrain(map);
	map->SetCamera(mCamera);
	map->Initialize(mWindow.GetGFX());
	mCurrentSelectTerrain = map;

	return true;
}

bool App::BuildTerrain(mydx::TerrainInfo& terrainInfo)
{
	std::shared_ptr<mydx::Terrain> map = std::make_shared<mydx::Terrain>(*g_graphics, terrainInfo);

	if (map == nullptr)
		return false;

	AddTerrain(map);
	map->SetCamera(mCamera);
	mCurrentSelectTerrain = map;

	return false;
}

std::unordered_map<std::string, std::shared_ptr<mydx::Terrain>>& App::GetTerrainList() noexcept
{
	return mTerrainList;
}

std::shared_ptr<mydx::Terrain>& App::GetCurrentTerrain() noexcept
{
	return mCurrentSelectTerrain;
}
