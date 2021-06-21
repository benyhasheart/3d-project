#include <memory>
#include "BoundingBox.h"

using namespace mydx;
using namespace DirectX;


mydx::BoundingBox::BoundingBox(Graphics& graphics, DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extens)
	:
	mCenter(center),
	mExtens(extens),
	mTransform(::XMMatrixIdentity())
{
	mBoundingBoxData.Center = {0.0f,0.0f,0.0f};
	mBoundingBoxData.Min = {-1.0f, -1.0f, -1.0f};
	mBoundingBoxData.Max = { 1.0f, 1.0f, 1.0f };
	mBoundingBoxData.Axis[0] = { 1.0f, 0.0f, 0.0f};
	mBoundingBoxData.Axis[1] = { 0.0f, 1.0f, 0.0f };
	mBoundingBoxData.Axis[2] = { 0.0f, 0.0f, 1.0f };
	mBoundingBoxData.Extent[0] = { 1.0f, 0.0f, 0.0f };
	mBoundingBoxData.Extent[1] = { 0.0f, 1.0f, 0.0f };
	mBoundingBoxData.Extent[2] = { 0.0f, 0.0f, 1.0f };

	Initialize(graphics);
}

mydx::BoundingBox::BoundingBox()
	:
	mCenter({0.0f,0.0f,0.0f}),
	mExtens({1.0f,1.0f,1.0f}),
	mTransform(::XMMatrixIdentity())
{
	mBoundingBoxData.Center = { 0.0f,0.0f,0.0f };
	mBoundingBoxData.Min = { -1.0f, -1.0f, -1.0f };
	mBoundingBoxData.Max = { 1.0f, 1.0f, 1.0f };
	mBoundingBoxData.Axis[0] = { 1.0f, 0.0f, 0.0f };
	mBoundingBoxData.Axis[1] = { 0.0f, 1.0f, 0.0f };
	mBoundingBoxData.Axis[2] = { 0.0f, 0.0f, 1.0f };
	mBoundingBoxData.Extent[0] = { 1.0f, 0.0f, 0.0f };
	mBoundingBoxData.Extent[1] = { 0.0f, 1.0f, 0.0f };
	mBoundingBoxData.Extent[2] = { 0.0f, 0.0f, 1.0f };
}

DirectX::XMMATRIX mydx::BoundingBox::GetTransform() const noexcept 
{
	return mTransform;
}

bool mydx::BoundingBox::Initialize(Graphics& graphics) noexcept  
{
	struct VerTex
	{
		struct
		{
			float x, y, z, w;
		} position;
		struct
		{
			float x, y, z, w;
		} color;
	};

	const std::vector<VerTex> vertices =
	{
		{ -1.0f,-1.0f,-1.0f,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ 1.0f,-1.0f,-1.0f,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ -1.0f,1.0f,-1.0f,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ 1.0f,1.0f,-1.0f,1.0f,		1.0f, 0.0f, 0.0f, 1.0f},
		{ -1.0f,-1.0f,1.0f,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ 1.0f,-1.0f,1.0f,1.0f,		1.0f, 0.0f, 0.0f, 1.0f},
		{ -1.0f,1.0f,1.0f,1.0f,		1.0f, 0.0f, 0.0f, 1.0f},
		{ 1.0f,1.0f,1.0f,1.0f,		1.0f, 0.0f, 0.0f, 1.0f},
	};
	const std::vector<DWORD> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	AddBind(std::make_shared<VertexBuffer<VerTex>>(graphics, vertices));

	

	AddIndexBuffer(std::make_shared<IndexBuffer>(graphics, indices));

	auto mVertexShaderClass = std::make_shared<VertexShader>(graphics, L"BaseVertexShader.hlsl", "main");
	auto bytecodeBlob = mVertexShaderClass->GetBytecodeBlob();

	AddBind(std::move(mVertexShaderClass));


	const std::vector< D3D11_INPUT_ELEMENT_DESC> layoutList =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	AddBind(std::make_shared<InputLayout>(graphics, layoutList, bytecodeBlob));

	AddBind(std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<PixelShader>(graphics, L"BasePixelShader.hlsl", "main"));

	AddBind(std::make_shared<TransformConstantBuffer>(graphics, *this));


	D3D11_RASTERIZER_DESC RSdesc = {};

	RSdesc.DepthClipEnable = TRUE;
	RSdesc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME
	RSdesc.CullMode = D3D11_CULL_BACK;
	graphics.GetDevice()->CreateRasterizerState(&RSdesc, mRasterizerStateWireFrame.GetAddressOf());

	RSdesc.FillMode = D3D11_FILL_SOLID;
	graphics.GetDevice()->CreateRasterizerState(&RSdesc, mRasterizerStateSolid.GetAddressOf());

	return false;
}

bool mydx::BoundingBox::Update(Graphics& graphics) noexcept
{
	ResourceUpdate(graphics);
	return false;
}

bool mydx::BoundingBox::PreRender(Graphics& graphics) noexcept
{
	ResourceBinding(graphics);
	return false;
}

bool mydx::BoundingBox::Render(Graphics& graphics) noexcept
{
	PreRender(graphics);
	//temperory
	graphics.GetDeviceContext()->RSSetState(mRasterizerStateWireFrame.Get());

	Draw(graphics);

	graphics.GetDeviceContext()->RSSetState(mRasterizerStateSolid.Get());
	PostRender(graphics);
	return false;
}

bool mydx::BoundingBox::PostRender(Graphics& graphics) noexcept
{
	return false;
}

void mydx::BoundingBox::CreateBoundingBox(DirectX::XMVECTOR min, DirectX::XMVECTOR max, DirectX::XMVECTOR center, DirectX::XMMATRIX transform) noexcept
{
	mBoundingBoxData.Center = center;
	mBoundingBoxData.Min = min;
	mBoundingBoxData.Max = max;

	//x,y,z rotation
	mBoundingBoxData.Axis[0] = ::XMVector3TransformCoord(mBoundingBoxData.Axis[0], transform);
	mBoundingBoxData.Axis[1] = ::XMVector3TransformCoord(mBoundingBoxData.Axis[1], transform);
	mBoundingBoxData.Axis[2] = ::XMVector3TransformCoord(mBoundingBoxData.Axis[2], transform);

	mBoundingBoxData.Axis[0] = ::XMVector3Normalize(mBoundingBoxData.Axis[0]);
	mBoundingBoxData.Axis[1] = ::XMVector3Normalize(mBoundingBoxData.Axis[1]);
	mBoundingBoxData.Axis[2] = ::XMVector3Normalize(mBoundingBoxData.Axis[2]);
	 
	const ::XMVECTOR halfDistance = ( max - min ) * 0.5f;


	mBoundingBoxData.Extent[0] = ::XMVector3Dot( mBoundingBoxData.Axis[0], halfDistance);
	mBoundingBoxData.Extent[1] = ::XMVector3Dot(mBoundingBoxData.Axis[1], halfDistance);
	mBoundingBoxData.Extent[2] = ::XMVector3Dot(mBoundingBoxData.Axis[2], halfDistance);

	//test

	mTransform = transform;
}

const BoundingBoxData& mydx::BoundingBox::GetBoundingBoxData() const noexcept
{
	return mBoundingBoxData;
}

void mydx::BoundingBox::SetBoundingBoxData(BoundingBoxData& data) noexcept
{
	mBoundingBoxData = data;
}
