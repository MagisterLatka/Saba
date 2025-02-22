#pragma once

#include "Saba/Core/Core.h"
#include <dxgidebug.h>

class DxgiInfoManager
{
public:
    SB_CORE DxgiInfoManager() = default;
    SB_CORE ~DxgiInfoManager() = default;
    DxgiInfoManager(const DxgiInfoManager&) = delete;
    DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;

    SB_CORE void Init();
    SB_CORE void Set() noexcept;
    SB_CORE std::string GetMessages() const;
private:
    uint64_t m_Next = 0u;
    ComPtr<IDXGIInfoQueue> m_DxgiInfoQueue;
};
