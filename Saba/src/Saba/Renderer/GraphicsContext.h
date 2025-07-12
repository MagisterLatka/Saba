#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class GraphicsContext : public RefCounted {
public:
    virtual ~GraphicsContext() = default;

    static Ref<GraphicsContext> Create();
};

} //namespace Saba
