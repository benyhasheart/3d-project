#include "UI.h"
void mydx::UI::PaintTerrainUI()
{
    if (ImGui::CollapsingHeader("Paint", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginTable("split1", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings))
        {

            ImGui::TableNextColumn();
            ImGui::Text("asd");
            ImGui::TableNextColumn();
            ImGui::Text("asdasd");
            ImGui::EndTable();
        }


    }


}
