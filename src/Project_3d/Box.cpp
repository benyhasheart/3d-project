#include <memory>

#include "Box.h"

using namespace DirectX;

Box::Box(Graphics& graphics)
	:mTransform(::XMMatrixIdentity()),
	mPositionVector(::XMVectorSet(0.0f,0.0f,0.0f,1.0f)),
	mScaleVector(::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f)),
	mPitchYawRoll(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mQuaternionRotation(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
{
	Initialize(graphics);
	mBoundingBox.Initialize(graphics);
}

Box::Box(Graphics& graphics, DirectX::XMVECTOR scale, DirectX::XMVECTOR pitchYawRoll, DirectX::XMVECTOR translate)
	:mTransform(::XMMatrixIdentity()),
	mPositionVector(::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
	mScaleVector(::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f)),
	mPitchYawRoll(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
	mQuaternionRotation(::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f))
{
	Initialize(graphics);

	mScaleVector = scale;
	mPitchYawRoll = pitchYawRoll;
	mPositionVector = translate;

	mQuaternionRotation = ::XMQuaternionRotationRollPitchYawFromVector(pitchYawRoll);
	mTransform = ::XMMatrixAffineTransformation(scale, ::XMVectorZero(), mQuaternionRotation, translate);

	mBoundingBox.CreateBoundingBox(
		::XMVECTOR({-1.0f,-1.0f,-1.0f}),
		::XMVECTOR({1.0f,1.0f,1.0f }),
		translate,
		mTransform
	);
	mBoundingBox.Initialize(graphics);
}

bool Box::Update(Graphics& graphics) noexcept
{
	/*::XMMATRIX scaleMatrix = ::XMMatrixScalingFromVector(mScaleVector);
	::XMMATRIX rotationMatrix = ::XMMatrixRotationRollPitchYawFromVector(mRotationVector);
	::XMMATRIX translationMatrix = ::XMMatrixTranslationFromVector(mTranslationVector);
	mTransform = scaleMatrix * rotationMatrix * translationMatrix;*/

	mQuaternionRotation = ::XMQuaternionRotationRollPitchYawFromVector(mPitchYawRoll);

	mTransform = ::XMMatrixAffineTransformation(mScaleVector, ::XMVectorZero(), mQuaternionRotation, mPositionVector);

	ResourceUpdate(graphics);

	mBoundingBox.Update(graphics);

	return false;
}

DirectX::XMMATRIX Box::GetTransform() const noexcept
{
	return mTransform;
}

bool Box::Initialize(Graphics& graphics) noexcept
{
	struct VerTex
	{
		struct
		{
			float x, y, z, w;
		} position;
		struct
		{
			float x, y;
		} taxtureCoord;
	};

	const std::vector<VerTex> vertices =
	{
		{ -1.0f,-1.0f,-1.0f,1.0f, 0.0f, 1.0f },
		{ 1.0f,-1.0f,-1.0f,1.0f, 1.0f, 1.0f},
		{ -1.0f,1.0f,-1.0f,1.0f, 0.0f, 0.0f},
		{ 1.0f,1.0f,-1.0f,1.0f, 1.0f, 0.0f},
		{ -1.0f,-1.0f,1.0f,1.0f, 0.0f, 1.0f},
		{ 1.0f,-1.0f,1.0f,1.0f, 1.0f, 1.0f},
		{ -1.0f,1.0f,1.0f,1.0f, 0.0f, 0.0f},
		{ 1.0f,1.0f,1.0f,1.0f, 1.0f, 0.0f},
	};

	std::vector<VerTex> vertexData;

	AddBind(std::make_shared<VertexBuffer>(graphics, vertices));

	const std::vector<DWORD> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	AddIndexBuffer(std::make_shared<IndexBuffer>(graphics, indices));

	auto mVertexShaderClass = std::make_shared<VertexShader>(graphics, L"VertexShader.vsh", "main");
	auto bytecodeBlob = mVertexShaderClass->GetBytecodeBlob();

	AddBind(std::move(mVertexShaderClass));


	const std::vector< D3D11_INPUT_ELEMENT_DESC> layoutList =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	AddBind(std::make_shared<InputLayout>(graphics, layoutList, bytecodeBlob));

	AddBind(std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_shared<PixelShader>(graphics, L"PixelShader.psh", "main"));

	AddBind(std::make_shared<Texture>(graphics, L"../../data/etcTexture/desert.jpg"));

	AddBind(std::make_shared<TransformConstantBuffer>(graphics, *this));


	return false;
}

bool Box::PreRender(Graphics& graphics) noexcept
{
	ResourceBinding(graphics);
	return false;
}

bool Box::Render(Graphics& graphics) noexcept
{
	PreRender(graphics);
	Draw(graphics);
	PostRender(graphics);
	mBoundingBox.Render(graphics);
	return false;
}

bool Box::PostRender(Graphics& graphics) noexcept
{
	return false;
}

void Box::SetScale(DirectX::XMVECTOR scaleVector) noexcept
{
	mScaleVector = scaleVector;
}

const DirectX::XMVECTOR Box::GetScale() const noexcept
{
	return mScaleVector;
}

void Box::SetRotation(DirectX::XMVECTOR rotationVector) noexcept
{
	mRotationVector = rotationVector;
}

const DirectX::XMVECTOR Box::GetRotation() const noexcept
{
	return mRotationVector;
}

void Box::SetTranslation(DirectX::XMVECTOR translationVector) noexcept
{
	mTranslationVector = translationVector;
}

const DirectX::XMVECTOR Box::GetTranslation() const noexcept
{
	return mTranslationVector;
}


const mydx::BoundingBox& Box::GetBoundingBox() const noexcept
{
	return mBoundingBox;
}
