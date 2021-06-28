#include "UI.h"
void mydx::UI::SculptingTerrainUI()
{
    if (ImGui::CollapsingHeader("Sculpting", ImGuiTreeNodeFlags_DefaultOpen))
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
