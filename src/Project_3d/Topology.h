#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology() = default;
	Topology( D3D_PRIMITIVE_TOPOLOGY topology);

public:
	virtual void Bind(Graphics& graphics)noexcept override final;
	virtual bool Update(Graphics& graphics) noexcept final { return false; }
private:
	D3D_PRIMITIVE_TOPOLOGY mTopology;
};

