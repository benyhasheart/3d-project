#pragma once

#include "Graphics.h"

class Bindable
{
public:
	Bindable() = default;
	virtual ~Bindable() = default;

public:
	virtual bool Update(Graphics& graphics) noexcept = 0;
	virtual void Bind(Graphics& graphics) noexcept  = 0;
};

