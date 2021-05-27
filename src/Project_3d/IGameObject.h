#pragma once
#include <DirectXMath.h>
#include "Graphics.h"

class IGameObject
{
public:
	IGameObject() = default;
	virtual ~IGameObject() = default;

public:
	virtual bool Initialize(Graphics& graphics) noexcept = 0;
	virtual bool Update(Graphics& graphics) noexcept = 0;
	virtual bool PreRender(Graphics& graphics) noexcept = 0;
	virtual bool Render(Graphics& graphics) noexcept = 0;
	virtual bool PostRender(Graphics& graphics) noexcept = 0;
	virtual bool Release() noexcept = 0;

	virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
};

