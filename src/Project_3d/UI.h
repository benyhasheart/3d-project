#pragma once

#include <functional>
#include <vector>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "IGameLoop.h"
#include "Texture.h"
#include "YonWindow.h"

class Graphics;
class App;
namespace mydx
{
	class UI : public IGameLoop
	{
	
	public:
		UI() = default;
		UI(Graphics& graphics);
		virtual ~UI() ;
 
	public:
		virtual bool Initialize() noexcept final override;
		virtual bool Update()  noexcept final override;
		virtual bool Render()  noexcept final override;
		virtual bool Release() noexcept final override;

	public:
		void SetOwenrApp(App* app) noexcept;
	private:
		void buildTerrainUI();
		void SculptingTerrainUI();
		void PaintTerrainUI();


	private:
		std::function<void(UI*)> mCurrentTerrainEditorUi;

		std::shared_ptr<Texture> mLoadedMaterial;
		std::shared_ptr<Texture> mLoadedHeightMap;
		std::string				mMaterialFileName;
		std::string				mHeightMapFileName;
		int						mRowCol[2];

		App*					mOwnerApp;					
		
	};
}


