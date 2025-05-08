#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class SwapChain : public RefCounted {
public:
    virtual ~SwapChain() = default;

    virtual uint32_t GetWidth() const noexcept = 0;
    virtual uint32_t GetHeight() const noexcept = 0;
};

}
