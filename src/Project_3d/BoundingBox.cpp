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

	//const std::vector<VerTex> vertices =
	//{
	//	{ mCenter.x - mExtens.x, mCenter.y - mExtens.y, mCenter.z - mExtens.z,1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
	//	{ mCenter.x + mExtens.x,mCenter.y - mExtens.y, mCenter.z - mExtens.z,1.0f,0.0f, 0.0f, 1.0f, 1.0f},
	//	{ mCenter.x - mExtens.x, mCenter.y + mExtens.y, mCenter.z - mExtens.z,1.0f,0.0f, 0.0f, 1.0f, 1.0f},
	//	{ mCenter.x + mExtens.x,mCenter.y + mExtens.y, mCenter.z - mExtens.z,1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
	//	{ mCenter.x - mExtens.x, mCenter.y - mExtens.y, mCenter.z + mExtens.z,1.0f,0.0f, 0.0f, 1.0f, 1.0f},
	//	{ mCenter.x + mExtens.x, mCenter.y - mExtens.y, mCenter.z + mExtens.z,1.0f,0.0f, 0.0f, 1.0f, 1.0f},
	//	{ mCenter.x - mExtens.x, mCenter.y + mExtens.y, mCenter.z + mExtens.z,1.0f,0.0f, 0.0f, 1.0f, 1.0f},
	//	{ mCenter.x + mExtens.x, mCenter.y + mExtens.y, mCenter.z + mExtens.z,1.0f,0.0f, 0.0f, 1.0f, 1.0f},
	//};

	//const std::vector<unsigned short> indices =
	//{
	//	0,1,3,2,0,
	//	4,6,2,3,7,
	//	5,1,5,4,6,7
	//};
	//::XMFLOAT3 convertCenter ={};
	//::XMStoreFloat3(&convertCenter, mBoundingBoxData.Center);

	/*const std::vector<VerTex> vertices =
	{
		{ convertCenter.x -1.0f,	convertCenter.y -1.0f,	convertCenter.z -1.0f,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ convertCenter.x + 1.0f,	convertCenter.y -1.0f,	convertCenter.z -1.0f,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ convertCenter.x -1.0f,	convertCenter.y + 1.0f,	convertCenter.z -1.0f,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ convertCenter.x + 1.0f,	convertCenter.y + 1.0f,	convertCenter.z -1.0f,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ convertCenter.x -1.0f,	convertCenter.y -1.0f,	convertCenter.z + 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f},
		{ convertCenter.x + 1.0f,	convertCenter.y -1.0f,	convertCenter.z + 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f},
		{ convertCenter.x -1.0f,	convertCenter.y + 1.0f,	convertCenter.z + 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f},
		{ convertCenter.x + 1.0f,	convertCenter.y + 1.0f,	convertCenter.z + 1.0f,1.0f,1.0f, 0.0f, 0.0f, 1.0f},
	};*/
	//::XMVECTOR extent[3] = {};
	//extent[0] = mBoundingBoxData.Axis[0] * mBoundingBoxData.Extent[0];
	//extent[1] = mBoundingBoxData.Axis[1] * mBoundingBoxData.Extent[1];
	//extent[2] = mBoundingBoxData.Axis[2] * mBoundingBoxData.Extent[2];
	//
	//auto minus = (mBoundingBoxData.Max - mBoundingBoxData.Min) * 0.5f;

	//extent[0] = mBoundingBoxData.Axis[0] * minus;
	//extent[1] = mBoundingBoxData.Axis[1] * minus;
	//extent[2] = mBoundingBoxData.Axis[2] * minus;
	////extent[0] =  mBoundingBoxData.Extent[0];
	////extent[1] =  mBoundingBoxData.Extent[1];
	////extent[2] =  mBoundingBoxData.Extent[2];
	//::XMVECTOR vertex[8] = {};
	//vertex[0] = mBoundingBoxData.Center - extent[0] - extent[1] - extent[2];
	//vertex[1] = mBoundingBoxData.Center + extent[0] - extent[1] - extent[2];
	//vertex[2] = mBoundingBoxData.Center - extent[0] + extent[1] - extent[2];
	//vertex[3] = mBoundingBoxData.Center + extent[0] + extent[1] - extent[2];
	//vertex[4] = mBoundingBoxData.Center - extent[0] - extent[1] + extent[2];
	//vertex[5] = mBoundingBoxData.Center + extent[0] - extent[1] + extent[2];
	//vertex[6] = mBoundingBoxData.Center - extent[0] + extent[1] + extent[2];
	//vertex[7] = mBoundingBoxData.Center + extent[0] + extent[1] + extent[2];

	/*const std::vector<VerTex> vertices =
	{
		{ ::XMVectorGetX(vertex[0]),	::XMVectorGetY(vertex[0]),	::XMVectorGetZ(vertex[0]) ,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ ::XMVectorGetX(vertex[1]),	::XMVectorGetY(vertex[1]),	::XMVectorGetZ(vertex[1]) ,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ ::XMVectorGetX(vertex[2]),	::XMVectorGetY(vertex[2]),	::XMVectorGetZ(vertex[2]) ,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ ::XMVectorGetX(vertex[3]),	::XMVectorGetY(vertex[3]),	::XMVectorGetZ(vertex[3]) ,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ ::XMVectorGetX(vertex[4]),	::XMVectorGetY(vertex[4]),	::XMVectorGetZ(vertex[4]) ,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ ::XMVectorGetX(vertex[5]),	::XMVectorGetY(vertex[5]),	::XMVectorGetZ(vertex[5]) ,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ ::XMVectorGetX(vertex[6]),	::XMVectorGetY(vertex[6]),	::XMVectorGetZ(vertex[6]) ,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{ ::XMVectorGetX(vertex[7]),	::XMVectorGetY(vertex[7]),	::XMVectorGetZ(vertex[7]) ,1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
	};*/
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
	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	AddBind(std::make_unique<VertexBuffer>(graphics, vertices));

	

	AddIndexBuffer(std::make_unique<IndexBuffer>(graphics, indices));

	auto mVertexShaderClass = std::make_unique<VertexShader>(graphics, L"BaseVertexShader.hlsl", "main");
	auto bytecodeBlob = mVertexShaderClass->GetBytecodeBlob();

	AddBind(std::move(mVertexShaderClass));


	const std::vector< D3D11_INPUT_ELEMENT_DESC> layoutList =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	AddBind(std::make_unique<InputLayout>(graphics, layoutList, bytecodeBlob));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<PixelShader>(graphics, L"BasePixelShader.hlsl", "main"));

	AddBind(std::make_unique<TransformConstantBuffer>(graphics, *this));


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
	mBoundingBoxData.Extent[1] = ::XMVector3Dot(mBoundingBoxData.Axis[0], halfDistance);
	mBoundingBoxData.Extent[2] = ::XMVector3Dot(mBoundingBoxData.Axis[0], halfDistance);

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
