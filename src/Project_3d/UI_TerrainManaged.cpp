#pragma once
#include "App.h"
#include "CommonDataStruct.h"
#include "UI.h"
#include "Texture.h"
#include "YonWindow.h"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/pointer.h"
#include <rapidjson/writer.h>
#include <rapidjson/reader.h>

#include <cstdio>



void mydx::UI::buildTerrainUI() noexcept
{
	static int value[2] = { 0 };
	static int eType = 0;
	static wchar_t Path[256] = {};
	static float position[4] = {0,0,0,1};
	static float scale[4] = {1,1,1,0};


	if (ImGui::CollapsingHeader("New Terrain"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		ImGui::RadioButton("New Build", &eType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("heightMap", &eType, 1);

		GetCurrentDirectory(256, Path);
		if (ImGui::BeginTable("split1", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
		{

			if (eType == 1)
			{
				ImGui::TableNextColumn();
				ImGui::Text("HeightMap");

				ImGui::TableNextColumn();
				//load HeightMap column
				if (mLoadedHeightMap != nullptr)
				{
					ID3D11ShaderResourceView* resourceView = const_cast<ID3D11ShaderResourceView*>(mLoadedHeightMap->GetShaderResourceView());
					ImGui::Image(reinterpret_cast<void*>(resourceView),
						ImVec2(64, 64));
				}

				ImGui::InputText("", mHeightMapFileName.data(), mHeightMapFileName.size());
				ImGui::SameLine();


				if (ImGui::Button("load##1"))
				{
					auto findFileName = FileOpenDialog();

					if (findFileName.length() > 0)
					{
						size_t size = findFileName.find_last_of(L"\\") + 1;
						//str.
						mHeightMapFileName = std::string(findFileName.begin() + size, findFileName.end());

						mLoadedHeightMap = std::make_shared<Texture>(*g_graphics, findFileName.c_str());
					}
					//디렉토리 경로를 이전 경로로 바꿔줌
					SetCurrentDirectory(Path);
					int a = 1;
				}

			}
			//load Material column
			ImGui::TableNextColumn();
			ImGui::Text("Material");

			ImGui::TableNextColumn();
			if (mLoadedMaterial != nullptr)
			{
				ID3D11ShaderResourceView* resourceView = const_cast<ID3D11ShaderResourceView*>(mLoadedMaterial->GetShaderResourceView());
				ImGui::Image(reinterpret_cast<void*>(resourceView),
					ImVec2(64, 64));
			}

			ImGui::InputText("", mMaterialFileName.data(), mMaterialFileName.size());
			ImGui::SameLine();
			if (ImGui::Button("load##2"))
			{
				auto findFileName = FileOpenDialog();

				if (findFileName.length() > 0)
				{
					size_t size = findFileName.find_last_of(L"\\") + 1;
					//str.
					mMaterialFileName = std::string(findFileName.begin() + size, findFileName.end());

					mLoadedMaterial = std::make_shared<Texture>(*g_graphics, findFileName.c_str());
				}
				SetCurrentDirectory(Path);
			}

			ImGui::EndTable();

		}


		if (eType == 0)
		{
			if (ImGui::BeginTable("split2", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
			{

				ImGui::TableNextColumn();
				ImGui::Text("Size");
				ImGui::TableNextColumn();
				ImGui::InputInt2(" row x col ", mRowCol);

				ImGui::EndTable();

			}
		}

		if (ImGui::BeginTable("split3", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
		{

			ImGui::TableNextColumn();
			ImGui::Text("Position");
			ImGui::Text("scale");
			ImGui::TableNextColumn();
			ImGui::InputFloat3("xyz#1", position, "%.1f");
			ImGui::InputFloat3("xyz#2", scale, "%.1f");

			ImGui::EndTable();

		}


		ImGui::Separator();
		if (ImGui::Button("Build"))
		{
			MapDesc mapDesc = {};
			mapDesc.RowCellCount = mRowCol[0];
			mapDesc.ColCellCount = mRowCol[1];
			mapDesc.position = DirectX::XMVectorSet(position[0], position[1], position[2], position[3]);
			mapDesc.scale = DirectX::XMVectorSet(scale[0], scale[1], scale[2], scale[3]);

			mOwnerApp->BuildTerrain(mapDesc, mLoadedMaterial, mLoadedHeightMap);
			mLoadedMaterial.reset();
			mLoadedHeightMap.reset();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			eType = 0;
			::ZeroMemory(value, sizeof(value));
			::ZeroMemory(position, sizeof(position));
			::ZeroMemory(scale, sizeof(scale));
			mLoadedMaterial.reset();
			mLoadedHeightMap.reset();
		}
	}


}

void mydx::UI::saveTerrain() noexcept
{
	if (mSelectTerrainName.length() <= 0)
		return;

	auto terrain = mOwnerApp->GetCurrentTerrain().get();

	mydx::TerrainInfo info;

	info.name = mSelectTerrainName;

	auto& material = terrain->GetMaterial();
	if (material != nullptr)
		info.textureName = material->GetTextureName();

	auto& heightMap = terrain->GetHeightMap();
	if (heightMap != nullptr)
		info.heightMapName = heightMap->GetTextureName();

	info.mapDesc = terrain->GetMapDesc();
	info.vertexTable = terrain->GetVertexBuffer()->GetVertexTable();
	info.indexTable = terrain->GetIndexBuffer()->GetIndexTable();
	

	save(info);
}

void mydx::UI::loadTerrain() noexcept
{
	mydx::TerrainInfo info;
	load(info);
}

void mydx::UI::save(TerrainInfo& terrain) noexcept
{
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Value o(rapidjson::kObjectType);
	{
		
		rapidjson::Value name;

		name = rapidjson::StringRef(terrain.name.c_str(), terrain.name.length());
		o.AddMember("name", name, document.GetAllocator());

		name = rapidjson::StringRef(terrain.textureName.c_str(), terrain.textureName.length());
		o.AddMember("material", name, document.GetAllocator());

		name = rapidjson::StringRef(terrain.heightMapName.c_str(), terrain.heightMapName.length());
		o.AddMember("heightMap", name, document.GetAllocator());

		rapidjson::Value cellcount;

		cellcount.SetUint(terrain.mapDesc.RowCellCount);
		o.AddMember("rowCellCount", cellcount, document.GetAllocator());

		cellcount.SetUint(terrain.mapDesc.ColCellCount);
		o.AddMember("colCellCount", cellcount, document.GetAllocator());

		DirectX::XMFLOAT4 convert;
		DirectX::XMStoreFloat4(&convert, terrain.mapDesc.position);

		rapidjson::Value position(rapidjson::kArrayType);
		position.PushBack(convert.x, document.GetAllocator());
		position.PushBack(convert.y, document.GetAllocator());
		position.PushBack(convert.z, document.GetAllocator());
		position.PushBack(convert.w, document.GetAllocator());

		o.AddMember("mapPosition", position, document.GetAllocator());

		DirectX::XMStoreFloat4(&convert, terrain.mapDesc.scale);

		
		rapidjson::Value scale(rapidjson::kArrayType);
	
		scale.PushBack(convert.x, document.GetAllocator());
		scale.PushBack(convert.y, document.GetAllocator());
		scale.PushBack(convert.z, document.GetAllocator());
		scale.PushBack(convert.w, document.GetAllocator());

		o.AddMember("mapScale", scale, document.GetAllocator());

		rapidjson::Value vertexTable(rapidjson::kArrayType);

		for (auto& vertex : terrain.vertexTable)
		{
			vertexTable.PushBack(vertex.position.x, document.GetAllocator() );
			vertexTable.PushBack(vertex.position.y, document.GetAllocator());
			vertexTable.PushBack(vertex.position.z, document.GetAllocator());
			vertexTable.PushBack(vertex.position.w, document.GetAllocator());
		}

		o.AddMember("vertexTable", vertexTable, document.GetAllocator());

		rapidjson::Value indexTable(rapidjson::kArrayType);

		for (auto& index : terrain.indexTable)
		{
			rapidjson::Value indexValue;
			indexValue.SetUint64(index);
			indexTable.PushBack(indexValue, document.GetAllocator());
		}

		o.AddMember("indexTable", indexTable, document.GetAllocator());

		document.AddMember("terrain", o, document.GetAllocator());

		FILE* fp;
		fopen_s(&fp,"output.json", "wb"); // non-Windows use "w"

		char writeBuffer[65536];
		rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

		rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
		document.Accept(writer);

		if (fp != nullptr)
			fclose(fp);
	}
	
	
}

void mydx::UI::load(TerrainInfo& terrain) noexcept
{
	static wchar_t Path[256] = {};
	GetCurrentDirectory(256, Path);

	auto findFileName = FileOpenDialog();

	if (findFileName.length() > 0)
	{
		auto convertFileName = std::string(findFileName.begin(), findFileName.end());

		FILE* fp;
		fopen_s(&fp, convertFileName.c_str(), "rb");  // non-Windows use "r"

		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		rapidjson::Document document;
		document.ParseStream(is);

		if (document.HasParseError())
		{
			return;
		}

		assert(document["terrain"].IsObject());
		rapidjson::Value& o = document["terrain"];

		assert(o.IsObject());
		{
			//name
			rapidjson::Value& name = o["name"];

			assert(name.IsString());
			terrain.name = std::string(name.GetString());
			//material
			rapidjson::Value& textureName = o["material"];

			assert(textureName.IsString());
			terrain.textureName = std::string(textureName.GetString());

			//heightMap
			rapidjson::Value& heightMapName = o["heightMap"];

			assert(heightMapName.IsString());
			terrain.heightMapName = std::string(heightMapName.GetString());
			//mapDesc
			rapidjson::Value& rowCellCount = o["rowCellCount"];

			assert(rowCellCount.IsUint());
			terrain.mapDesc.RowCellCount = rowCellCount.GetUint();

			rapidjson::Value& colCellCount = o["colCellCount"];

			assert(colCellCount.IsUint());
			terrain.mapDesc.ColCellCount = colCellCount.GetUint();

			rapidjson::Value& position = o["mapPosition"];

			assert(position.IsArray());

			DirectX::XMFLOAT4 posVector = {};
			for (rapidjson::SizeType i = 0; i < position.Size();)
			{
				posVector.x = position[i].GetFloat();
				posVector.y = position[i + 1].GetFloat();
				posVector.z = position[i + 2].GetFloat();
				posVector.w = position[i + 3].GetFloat();
				i += 4;
			}

			terrain.mapDesc.position = DirectX::XMLoadFloat4(&posVector);


			rapidjson::Value& scale = o["mapScale"];

			assert(scale.IsArray());

			DirectX::XMFLOAT4 scaleVector = {};
			for (rapidjson::SizeType i = 0; i < scale.Size();)
			{
				scaleVector.x = scale[i].GetFloat();
				scaleVector.y = scale[i + 1].GetFloat();
				scaleVector.z = scale[i + 2].GetFloat();
				scaleVector.w = scale[i + 3].GetFloat();
				i += 4;
			}

			terrain.mapDesc.scale = DirectX::XMLoadFloat4(&scaleVector);
			//vertex

			rapidjson::Value& vertexTable = o["vertexTable"];

			assert(vertexTable.IsArray());

			std::size_t size = vertexTable.Size() / 4;
			auto& table = terrain.vertexTable;
			table.reserve(size);

			DirectX::XMFLOAT4 vertex = {};
			for (rapidjson::SizeType i = 0; i < vertexTable.Size();)
			{
				vertex.x = vertexTable[i].GetFloat();
				vertex.y = vertexTable[i + 1].GetFloat();
				vertex.z = vertexTable[i + 2].GetFloat();
				vertex.w = vertexTable[i + 3].GetFloat();

				mydx::VertexData vertexData = {};
				vertexData.position = vertex;
				table.emplace_back(vertexData);
				i += 4;
			}

			//index

			rapidjson::Value& indexTable = o["indexTable"];

			assert(indexTable.IsArray());

			size = indexTable.Size();
			auto& indicesTable = terrain.indexTable;
			table.reserve(size);

			
			rapidjson::Value index;
			for (rapidjson::SizeType i = 0; i < indexTable.Size(); i++)
			{
				index = indexTable[i].GetUint64();
				indicesTable.emplace_back(static_cast<unsigned long long>(index.GetUint64()));
			}
		}

		if (fp != nullptr)
			fclose(fp);
	}

	SetCurrentDirectory(Path);
	
}
