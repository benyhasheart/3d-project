#pragma once
#include "Camera.h"

class FirstPersonViewCamera final : public Camera
{
public:
	FirstPersonViewCamera() = default;
	virtual ~FirstPersonViewCamera()= default;
	FirstPersonViewCamera(FirstPersonViewCamera& src) = delete;
	FirstPersonViewCamera& operator = (FirstPersonViewCamera& src) = delete;

public:
	bool Update(Graphics& graphics) noexcept override final;

};

