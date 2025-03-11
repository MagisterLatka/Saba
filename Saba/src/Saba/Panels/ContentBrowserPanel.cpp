#include <pch.h>
#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace Saba {

ContentBrowserPanel::ContentBrowserPanel()
    : m_CurrentDir(g_AssetsPath)
{
    Texture2DProps props;
    props.Filepath = g_AssetsPath / "DirectoryIcon.png";
    props.Sampling = TextureSampling::Point;
    m_DirIcon = Texture2D::Create(props);
    props.Filepath = g_AssetsPath / "FileIcon.png";
    m_FileIcon = Texture2D::Create(props);
}

void ContentBrowserPanel::OnUIRender() {
    ImGui::Begin("Content Browser");

    if (m_CurrentDir != g_AssetsPath) {
        if (ImGui::Button("<-"))
            m_CurrentDir = m_CurrentDir.parent_path();
    }

    static float padding = 20.0f, iconSize = 114.0f;
    const float cellSize = iconSize + padding;
    const float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = std::max(static_cast<int>(panelWidth / cellSize), 1);

    ImGui::Columns(columnCount, nullptr, false);

    for (const auto& dir : std::filesystem::directory_iterator(m_CurrentDir)) {
        const auto& path = dir.path();
        const auto relativePath = std::filesystem::relative(path, g_AssetsPath);
        std::string filenameString = relativePath.filename().string();

        const Ref<Texture2D>& icon = dir.is_directory() ? m_DirIcon : m_FileIcon;
        ImGui::PushID(filenameString.c_str());
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::ImageButton(filenameString.c_str(), (uint64_t)icon->GetRawPointer(), { iconSize, iconSize });

        if (ImGui::BeginDragDropSource()) {
            const auto relPathString = relativePath.string();
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", relPathString.c_str(), relPathString.length() + 1);
            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            if (dir.is_directory())
                m_CurrentDir /= path.filename();
        }

        ImGui::TextWrapped("%s", filenameString.c_str());

        ImGui::NextColumn();

        ImGui::PopID();
    }

    ImGui::Columns(1);

    ImGui::SliderFloat("Icon size", &iconSize, 16.0f, 512.0f);
    
    ImGui::End();
}

}
