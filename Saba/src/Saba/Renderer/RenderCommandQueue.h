#pragma once

#include "Saba/Core/Core.h"

namespace Saba {

class RenderCommandQueue {
public:
    using RenderCommandFn = void(*)(void*);

    SB_CORE RenderCommandQueue();
    SB_CORE ~RenderCommandQueue();

    SB_CORE void* Allocate(RenderCommandFn fn, uint64_t size);
    SB_CORE void Execute();
private:
    uint8_t* m_CommandBuffer, *m_CommandBufferPtr;
    uint64_t m_CommandCount = 0;
};

} //namespace Saba
