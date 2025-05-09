#pragma once

#include "Saba/Renderer/Texture.h"

namespace Saba {

inline static const std::filesystem::path g_AssetsPath = "assets";

class ContentBrowserPanel {
public:
    SB_CORE ContentBrowserPanel();
    ~ContentBrowserPanel() = default;

    SB_CORE void Open() noexcept;

    SB_CORE void OnUIRender();
private:
    std::filesystem::path m_CurrentDir;
    Ref<Texture2D> m_DirIcon, m_FileIcon;
    bool m_Open = false;
};

} //namespace Saba
