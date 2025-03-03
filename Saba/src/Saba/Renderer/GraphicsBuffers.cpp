#include <pch.h>
#include "GraphicsBuffers.h"

#include "Saba/Renderer/RendererAPI.h"
#if defined(SB_PLATFORM_WINDOWS)
#    include "Saba/Platform/DX11/DX11GraphicsBuffers.h"
#endif
#include "Saba/Platform/OpenGL/OpenGLGraphicsBuffers.h"

namespace Saba {

Ref<VertexBuffer> VertexBuffer::Create(BufferLayout layout, const void* data, uint32_t size, BufferUsage usage) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(layout, data, size, usage);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11VertexBuffer>::Create(layout, data, size, usage);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<VertexBuffer> VertexBuffer::Create(BufferLayout layout, const Buffer& buffer, BufferUsage usage) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(layout, buffer, usage);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11VertexBuffer>::Create(layout, buffer, usage);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<VertexBuffer> VertexBuffer::Create(BufferLayout layout, Buffer&& buffer, BufferUsage usage) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(layout, std::move(buffer), usage);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11VertexBuffer>::Create(layout, std::move(buffer), usage);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}



Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* data, uint32_t size, BufferUsage usage) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(data, size, usage);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11IndexBuffer>::Create(data, size, usage);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<IndexBuffer> IndexBuffer::Create(const Buffer& buffer, BufferUsage usage) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(buffer, usage);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11IndexBuffer>::Create(buffer, usage);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<IndexBuffer> IndexBuffer::Create(Buffer&& buffer, BufferUsage usage) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLIndexBuffer>::Create(std::move(buffer), usage);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11IndexBuffer>::Create(std::move(buffer), usage);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}



Ref<ConstantBuffer> ConstantBuffer::Create(BufferShaderBinding binding, const void* data, uint32_t size) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLConstantBuffer>::Create(binding, data, size);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11ConstantBuffer>::Create(binding, data, size);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<ConstantBuffer> ConstantBuffer::Create(BufferShaderBinding binding, const Buffer& buffer) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLConstantBuffer>::Create(binding, buffer);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11ConstantBuffer>::Create(binding, buffer);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<ConstantBuffer> ConstantBuffer::Create(BufferShaderBinding binding, Buffer&& buffer) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLConstantBuffer>::Create(binding, std::move(buffer));
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11ConstantBuffer>::Create(binding, std::move(buffer));
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}
Ref<ConstantBuffer> ConstantBuffer::Create(BufferShaderBinding binding, const UniformBufferBase& buffer) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::None:    SB_CORE_THROW_INFO("None API is not supported"); return nullptr;
        case RendererAPI::API::OpenGL:  return Ref<OpenGLConstantBuffer>::Create(binding, buffer);
#if defined(SB_PLATFORM_WINDOWS)
        case RendererAPI::API::DX11:    return Ref<DX11ConstantBuffer>::Create(binding, buffer);
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DirectX 12 is not supported yet"); return nullptr;
#else
        case RendererAPI::API::DX11:
        case RendererAPI::API::DX12:    SB_CORE_THROW_INFO("DX11 and DX12 are not supported on non-windows systems"); return nullptr;
#endif
        case RendererAPI::API::Vulkan:  SB_CORE_THROW_INFO("Vulkan is not supported yet"); return nullptr;
    }

    SB_CORE_THROW_INFO("Unknown API");
    return nullptr;
}

}
