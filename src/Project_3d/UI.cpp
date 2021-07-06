#pragma once
#include "UI.h"
#include "App.h"

using namespace mydx;

mydx::UI::UI(Graphics& graphics)
    :
    mCurrentTerrainEditorUi(nullptr),
    mLoadedMaterial(nullptr),
    mOwnerApp(nullptr)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(g_hwnd);
    ImGui_ImplDX11_Init(graphics.GetDevice(), graphics.GetDeviceContext());
    Initialize();
}

mydx::UI::~UI()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}


bool mydx::UI::Initialize() noexcept
{
    mCurrentTerrainEditorUi = &UI::buildTerrainUI;
    //urrentTerrainEditorUi(this);
    return false;
}

bool mydx::UI::Update() noexcept
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //Terrain Editor
    bool isOpen = true;
    ImGui::SetNextWindowPos( ImVec2(0.0f,0.0f));
    ImGui::Begin("Terrain Editor", &isOpen);
    auto nextUiPosX = ImGui::GetWindowPos().x + ImGui::GetWindowWidth();
    mCurrentTerrainEditorUi(this);
    //ImGui::SetWindowSize(ImVec2(200.0f, YonWindow::windowHeight));

    ImGui::End();
    //Mode Tollbar
    ImGui::SetNextWindowPos(ImVec2(nextUiPosX, 0.0f));
    ImGui::Begin("Mode Tollbar", &isOpen);
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Manage"))
        {
            mCurrentTerrainEditorUi = &UI::buildTerrainUI;
            ImGui::Button("  New \n"
                            "Terrain", ImVec2(64.0f,32.0f));
            ImGui::SameLine();
            if (ImGui::Button("Save", ImVec2(64.0f, 32.0f)))
            {
                saveTerrain();
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Load", ImVec2(64.0f, 32.0f)))
            {
                loadTerrain();
            }

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Sculpting"))
        {
            mCurrentTerrainEditorUi = &UI::SculptingTerrainUI;
            ImGui::Button("Sculpting");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Paint"))
        {
            mCurrentTerrainEditorUi = &UI::PaintTerrainUI;
            ImGui::Button("Paint");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    ImGui::End();

    //World Actor
    ImGui::SetNextWindowPos(ImVec2(YonWindow::windowWidth - ImGui::GetWindowWidth(), 0));
    ImGui::Begin("World Actor", &isOpen);

    auto& terrainIter = mOwnerApp->GetTerrainList();
    static std::vector<std::string> terrainList;
    if (mTerrainList.size() != terrainIter.size())
    {
        TerrainListUpdate();
    }
    
    static int item_current_idx = 0;
    static int item_before_idx = 0;
    ImGui::Text("Actor List");
    if (ImGui::BeginListBox("##Actor List"))
    {
        for (int n = 0; n < mTerrainList.size(); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(mTerrainList[n].c_str(), is_selected))
            {
                item_before_idx = item_current_idx;
                item_current_idx = n;
            }


            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
                mSelectTerrainName = mTerrainList[item_current_idx];
            }
                
        }

        /*if (item_before_idx != item_current_idx)
        {
            mSelectTerrainName = mTerrainList[item_current_idx];
            mOwnerApp->SelectTerrainActor(mSelectTerrainName);
        }*/
            
        ImGui::EndListBox();
    }
    ImGui::Separator();
    
    if ( ImGui::Button("Delete") )
    {
        mSelectTerrainName = mTerrainList[item_current_idx];
        mOwnerApp->RemoveTerrain(mSelectTerrainName);
    }

    ImGui::End();
    return false;
}

bool mydx::UI::Render() noexcept
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return true;
}

bool mydx::UI::Release() noexcept
{
    return false;
}

void mydx::UI::SetOwenrApp(App* app) noexcept
{
    mOwnerApp = app;
}

bool mydx::UI::TerrainListUpdate() noexcept
{
    if (mOwnerApp == nullptr)
        return false;

    auto& terrainIter = mOwnerApp->GetTerrainList();
    mTerrainList.clear();

    for (auto& iter : terrainIter)
    {
        mTerrainList.push_back(iter.first);
    }

    return true;
}

std::wstring mydx::UI::FileOpenDialog()
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
        return std::wstring(szFile);;
    }
    return std::wstring(szFile);
}