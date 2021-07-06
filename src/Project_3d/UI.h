#pragma once

#include <functional>
#include <vector>

#include "CommonDataStruct.h"
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
		bool TerrainListUpdate() noexcept;
		//temporory func
		std::wstring FileOpenDialog();
	public:
		virtual bool Initialize() noexcept final override;
		virtual bool Update()  noexcept final override;
		virtual bool Render()  noexcept final override;
		virtual bool Release() noexcept final override;

	public:
		void SetOwenrApp(App* app) noexcept;
	private:

		void buildTerrainUI() noexcept;
		void saveTerrain() noexcept;
		void loadTerrain() noexcept;
		void save(TerrainInfo& terrain) noexcept;
		void load(TerrainInfo& terrain) noexcept;

		void SculptingTerrainUI();


		void PaintTerrainUI();


	private:
		std::vector<std::string> mTerrainList;

		std::function<void(UI*)> mCurrentTerrainEditorUi;

		std::shared_ptr<Texture> mLoadedMaterial;
		std::shared_ptr<Texture> mLoadedHeightMap;

		std::string				mSelectTerrainName;
		std::string				mMaterialFileName;
		std::string				mHeightMapFileName;
		int						mRowCol[2];

		App*					mOwnerApp;					
		
	};
}


