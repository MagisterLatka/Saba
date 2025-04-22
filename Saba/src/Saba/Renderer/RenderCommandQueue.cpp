#include <pch.h>
#include "RenderCommandQueue.h"

namespace Saba {

static constexpr uint64_t c_CommandBufferSize = 10lu * 1024lu * 1024lu;

RenderCommandQueue::RenderCommandQueue()
    : m_CommandBuffer(new uint8_t[c_CommandBufferSize]), m_CommandBufferPtr(m_CommandBuffer) {}
RenderCommandQueue::~RenderCommandQueue() {
    delete[] m_CommandBuffer;
}

void* RenderCommandQueue::Allocate(RenderCommandFn fn, uint64_t size) {
    if (m_CommandBufferPtr + (sizeof(RenderCommandFn) + sizeof(uint64_t) + size) > m_CommandBuffer + c_CommandBufferSize) {
        SB_CORE_WARN("Exceed RenderCommandQueue command buffer");
        Execute();
    }

    *reinterpret_cast<RenderCommandFn*>(m_CommandBufferPtr) = fn;
    m_CommandBufferPtr += sizeof(RenderCommandFn);

    *reinterpret_cast<uint64_t*>(m_CommandBufferPtr) = size;
    m_CommandBufferPtr += sizeof(uint64_t);

    void* memory = reinterpret_cast<void*>(m_CommandBufferPtr);
    m_CommandBufferPtr += size;
    ++m_CommandCount;
    return memory;
}
void RenderCommandQueue::Execute() {
    uint8_t* buffer = m_CommandBuffer;
    for (uint64_t i = 0; i < m_CommandCount; ++i) {
        RenderCommandFn& function = *reinterpret_cast<RenderCommandFn*>(buffer);
        buffer += sizeof(RenderCommandFn);

        uint64_t size = *reinterpret_cast<uint64_t*>(buffer);
        buffer += sizeof(uint64_t);

        function(buffer);
        buffer += size;
    }

    m_CommandBufferPtr = m_CommandBuffer;
    m_CommandCount = 0;
}

} //namespace Saba
