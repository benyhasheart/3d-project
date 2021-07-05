#pragma once
#include <DirectXMath.h>
#include "IGameLoop.h"
#include "YonWindow.h"
#include "YonTimer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "GeometryShader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "Texture.h"
#include "Vector.h"
#include "Matrix.h"
#include "Box.h"
#include "Camera.h"
#include "BoundingBox.h"
#include "HeightMap.h"
#include "QuardTreeVertexIndex.h"
#include "Select.h"
#include "UI.h"
#include "Terrain.h"





//constant buffer test
struct ConstantData
{
	float x,y,z,w;
};

struct TransformConstantData
{
	dxmath::Matrix matrixWorld;
	dxmath::Matrix matrixView;
	dxmath::Matrix matrixProjection;
	float x, y;
};

struct TransformConstantData2
{
	DirectX::XMMATRIX matrixWorld;
};

struct ViewPerspectiveConstantData
{
	DirectX::XMMATRIX matrixView;
	DirectX::XMMATRIX matrixProjection;
};


class App : public IGameLoop
{
public:
	App();
	App(App& src) = delete;
	App& operator=(App& src) = delete;
	~App() =default;

public:
	int Excute();

public:
	void AddTerrain(std::shared_ptr<mydx::Terrain>& map) noexcept;
	void RemoveTerrain(std::string& terrainName) noexcept;

	std::unordered_map<std::string, std::shared_ptr<mydx::Terrain>>::iterator
	FindTerrain(std::string& terrainName) noexcept;

	void SelectTerrainActor(std::string& terrainName) noexcept;
	bool BuildTerrain(mydx::MapDesc& mapDesc, std::shared_ptr<Texture> material = nullptr, std::shared_ptr<Texture> heightMap = nullptr);

public:
	std::unordered_map<std::string, std::shared_ptr<mydx::Terrain>>& GetTerrainList() noexcept;
	std::shared_ptr<mydx::Terrain>& GetCurrentTerrain() noexcept;

public:
	virtual bool Initialize() override;
	virtual bool Update() override;
	virtual bool Render() override;
	virtual bool Release() override;

// --test 
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerStateWireFrame;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterizerStateSolid;
	//std::unique_ptr<VertexBuffer<VerTex>> mVertexBufferClass;
	std::unique_ptr<IndexBuffer> mIndexBufferClass;


	std::unique_ptr<VertexShader> mVertexShaderClass;
	std::unique_ptr<PixelShader> mPixelShaderClass;
	std::unique_ptr<GeometryShader> mGeometryShaderClass;

	std::unique_ptr<Bindable> mVertexConstantBuffer;
	std::unique_ptr<InputLayout> mInputLayoutClass;
	std::unique_ptr<Topology> mTopologyClass;
	std::unique_ptr<Texture> mTexture;

	std::unique_ptr<Box> mBoxShape;
	std::shared_ptr<Camera> mCamera;
	std::unique_ptr<Camera> mTopViewCamera;
	std::unique_ptr<mydx::BoundingBox> mBoundingBox;
	std::shared_ptr<mydx::Map> mMap;
	std::shared_ptr<mydx::HeightMap> mHeightMap;
	std::shared_ptr<mydx::Map> mTerrain;
	std::unique_ptr<mydx::QuardTree> mQuardTree;
	std::shared_ptr<mydx::QuardTreeVertexIndex> mQuardTreeVertexIndex;
	std::shared_ptr<mydx::Select> mSelect;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture1;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture2;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> mSamplerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mStencilState1;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mStencilState2;
	D3D11_TEXTURE2D_DESC		mDescTexture1;
	D3D11_TEXTURE2D_DESC		mDescTexture2;
	D3D11_TEXTURE2D_DESC		mDescBackBuffer;
	D3D11_SAMPLER_DESC			mSamplerDesc;
	D3D11_DEPTH_STENCIL_DESC    mDepthStencilDesc;
	D3D11_VIEWPORT				mViewPort[4];
	TransformConstantData		mTransformData;
	TransformConstantData2		mTransformData2;
	ViewPerspectiveConstantData mViewPerspectiveData;

	std::vector< std::shared_ptr<Box>> mBoxes;
	//std::vector<std::shared_ptr<mydx::Map>> mTerrainList;
	
	//UINT mStride;
// --
private:
	YonWindow mWindow;
	YonTimer mTimer;

	std::unique_ptr<mydx::UI> mUi;
	std::shared_ptr<mydx::Terrain> mCurrentSelectTerrain;

	std::unordered_map<std::string, std::shared_ptr<mydx::Terrain>> mTerrainList;
};

