#pragma once
#include "App.h"
#include "CommonDataStruct.h"
#include "UI.h"
#include "Texture.h"
#include "YonWindow.h"



void mydx::UI::buildTerrainUI()
{
   static int value[2] = {0};

	if ( ImGui::CollapsingHeader("New Terrain"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        if (ImGui::BeginTable("split1", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
        {
            //material column
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
            wchar_t Path[256] = {};
            GetCurrentDirectory(256, Path);
            if (ImGui::Button("load##1"))
            {
                OPENFILENAME ofn;       // common dialog box structure
                TCHAR szFile[260] = { 0 };       // if using TCHAR macros
                // Initialize OPENFILENAME
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = g_hwnd;
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = _T("..\..\data\map");
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn) == TRUE)
                {
                    std::wstring str = std::wstring(szFile);
                    size_t size = str.find_last_of(L"\\") + 1;
                    //str.
                    mHeightMapFileName = std::string(str.begin() + size, str.end());

                    mLoadedHeightMap = std::make_shared<Texture>(*g_graphics, szFile);
                }
                //디렉토리 경로를 이전 경로로 바꿔줌
                SetCurrentDirectory(Path);
                int a = 1;
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
         
            ImGui::InputText("",mMaterialFileName.data(), mMaterialFileName.size());
            ImGui::SameLine();
            if (ImGui::Button("load##2"))
            {
                OPENFILENAME ofn;       // common dialog box structure
                TCHAR szFile[260] = { 0 };       // if using TCHAR macros
                // Initialize OPENFILENAME
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = g_hwnd;
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn) == TRUE)
                {
                    std::wstring str =  std::wstring(szFile);
                    size_t size = str.find_last_of(L"\\") + 1;
                   //str.
                    mMaterialFileName = std::string(str.begin() + size,str.end());

                    mLoadedMaterial = std::make_shared<Texture>(*g_graphics, szFile);
                }
                SetCurrentDirectory(Path);
            }
            ImGui::EndTable();
            
        }

        if (ImGui::BeginTable("split2", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
        {

            ImGui::TableNextColumn();
            ImGui::Text("Size");
            ImGui::TableNextColumn();
            ImGui::InputInt2(" row x col ", mRowCol);
            
            ImGui::EndTable();
            
        }
        ImGui::Separator();
        if (ImGui::Button("Build"))
        {
            MapDesc mapDesc = {};
            mapDesc.RowCellCount = mRowCol[0];
            mapDesc.ColCellCount = mRowCol[1];
            mapDesc.CellDistance = 1.0f;

            mOwnerApp->BuildTerrain(mapDesc, mLoadedMaterial, mLoadedHeightMap);
        }
        
    }
    

}
