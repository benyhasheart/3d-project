#pragma once
#include "Drawable.h"

class Camera;
namespace mydx
{
	struct Ray
	{
		DirectX::XMVECTOR Origine;
		DirectX::XMVECTOR Direction;
		float Extent;
	};
	class Select : public Drawable
	{
	public:
		Select();
		virtual ~Select() = default;

	public:
		DirectX::XMVECTOR ScreenToWorld(DirectX::XMVECTOR position, float screenWidth, float screenHeight) noexcept;
		bool PointInPolygon(DirectX::XMVECTOR intersectionPoint, DirectX::XMVECTOR vertex0, DirectX::XMVECTOR vertex1, DirectX::XMVECTOR vertex2) noexcept;
		bool CheckInterSection(DirectX::XMVECTOR start, DirectX::XMVECTOR end, DirectX::XMVECTOR vertex0, DirectX::XMVECTOR vertex1, DirectX::XMVECTOR vertex2);
	public:
		virtual bool Initialize(Graphics& graphics) noexcept final;
		virtual bool Update(Graphics& graphics) noexcept final;
		virtual bool PreRender(Graphics& graphics) noexcept final;
		virtual bool Render(Graphics& graphics) noexcept final;
		virtual bool PostRender(Graphics& graphics) noexcept final;
		virtual bool Release() noexcept final;

		virtual DirectX::XMMATRIX GetTransform() const noexcept final;
	public:
		void SetCamera(Camera* camera) noexcept;
		
		Ray& GetRay() noexcept;
		DirectX::XMVECTOR& GetIntersectingLineSegment() noexcept;
	public:

	private:
		Camera* mCamera;
		Ray		mRay;
		DirectX::XMVECTOR mIntersectingLineSegment;
	};


}
