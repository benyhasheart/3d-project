#pragma once
#include "UI.h"


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
void testfunc()
{

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

    bool isOpen = true;
    ImGui::SetNextWindowPos( ImVec2(0.0f,0.0f));
    ImGui::Begin("Terrain Editor", &isOpen);
    auto nextUiPosX = ImGui::GetWindowPos().x + ImGui::GetWindowWidth();
    mCurrentTerrainEditorUi(this);
    //ImGui::SetWindowSize(ImVec2(200.0f, YonWindow::windowHeight));

    ImGui::End();

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
            ImGui::Button("Save", ImVec2(64.0f, 32.0f));
            ImGui::SameLine();
            ImGui::Button("Load", ImVec2(64.0f, 32.0f));
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
    //ImGui::SetWindowSize(ImVec2(200.0f, YonWindow::windowHeight));

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