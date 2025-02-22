#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class GraphicsContext : public RefCounted {
public:
    SB_CORE virtual ~GraphicsContext() = default;

    SB_CORE virtual void Init() = 0;

    SB_CORE static Ref<GraphicsContext> Create(void* data = nullptr);
};

}
