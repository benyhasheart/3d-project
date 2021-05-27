#pragma once
#include "Camera.h"
class ModelViewCamera final : public Camera
{
public:
	ModelViewCamera();
	ModelViewCamera(ModelViewCamera& src)= delete;
	ModelViewCamera& operator = (ModelViewCamera& src) = delete;
	virtual ~ModelViewCamera()= default;

public:
	bool Update(Graphics& graphics)  noexcept override final;
	
private:
	void updateViewMatrix(DirectX::FXMVECTOR eye, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up)noexcept;
	
};

