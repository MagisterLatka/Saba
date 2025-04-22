#include <pch.h>
#include "OpenGLRenderPass.h"

#include "Saba/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Saba {

OpenGLRenderPass::OpenGLRenderPass() {
    Create();
}
void OpenGLRenderPass::Create() {
    Ref<OpenGLRenderPass> instance = this;
    Renderer::Submit([instance]() mutable {
        glCreateFramebuffers(1, &instance->m_ID);
    });
}
OpenGLRenderPass::~OpenGLRenderPass() {
    glDeleteFramebuffers(1, &m_ID);
}

void OpenGLRenderPass::SetRenderTarget(uint32_t index, Ref<RenderTarget> renderTarget) {
    SB_CORE_ASSERT(index < 8, "Exceeding available number of simultaneous render targets");
    m_RenderTargets[index] = renderTarget.As<OpenGLRenderTarget>();

    if (m_RenderTargets[index]) {
        SB_CORE_ASSERT(!(static_cast<uint32_t>(m_RenderTargets[index]->m_Format) & 0x30u), "Render target cannot have depth or depth stencil format");

        if (m_Width == 0 || m_Height == 0) {
            m_Width = m_RenderTargets[index]->m_Width;
            m_Height = m_RenderTargets[index]->m_Height;
        }
    }
}
void OpenGLRenderPass::SetDepthStencilTarget(Ref<RenderTarget> depthStencilTarget) {
    m_DepthStencilTarget = depthStencilTarget.As<OpenGLRenderTarget>();
    if (m_DepthStencilTarget)
        SB_CORE_ASSERT(static_cast<uint32_t>(m_DepthStencilTarget->m_Format) & 0x30u, "Depth stencil target must have depth or depth stencil format");
}
void OpenGLRenderPass::Bind() const noexcept {
    Ref<const OpenGLRenderPass> instance = this;
    Renderer::Submit([instance]() {
        for (const auto& [index, target] : instance->m_RenderTargets | std::views::enumerate) {
            if (target)
                glNamedFramebufferTexture(instance->m_ID, GL_COLOR_ATTACHMENT0 + static_cast<uint32_t>(index), target->m_ID, 0);
        }

        if (instance->m_DepthStencilTarget) {
            if (instance->m_DepthStencilTarget->m_Format == RenderTargetFormat::Depth32F)
                glNamedFramebufferRenderbuffer(instance->m_ID, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, instance->m_DepthStencilTarget->m_ID);
            else
                glNamedFramebufferRenderbuffer(instance->m_ID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, instance->m_DepthStencilTarget->m_ID);
        }

        static auto buffers = std::to_array<GLenum>({
            GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
            GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7
        });
        glNamedFramebufferDrawBuffers(instance->m_ID, 8u, buffers.data());
        glBindFramebuffer(GL_FRAMEBUFFER, instance->m_ID);
        glViewport(0, 0, static_cast<int>(instance->m_Width), static_cast<int>(instance->m_Height));
    });
}
void OpenGLRenderPass::Clear() noexcept {
    Ref<OpenGLRenderPass> instance = this;
    Renderer::Submit([instance]() {
        for (const auto& [index, renderTarget] : instance->m_RenderTargets | std::views::enumerate) {
            if (renderTarget) {
                if (renderTarget->m_Format == RenderTargetFormat::R32_UINT || renderTarget->m_Format == RenderTargetFormat::RG32_UINT ||
                    renderTarget->m_Format == RenderTargetFormat::RGB32_UINT || renderTarget->m_Format == RenderTargetFormat::RGBA32_UINT)
                {
                    const glm::uvec4 clear = renderTarget->m_ClearValue;
                    glClearNamedFramebufferuiv(instance->m_ID, GL_COLOR, static_cast<int>(index), glm::value_ptr(clear));
                }
                else
                    glClearNamedFramebufferfv(instance->m_ID, GL_COLOR, static_cast<int>(index), glm::value_ptr(renderTarget->m_ClearValue));
            }
        }
        if (instance->m_DepthStencilTarget) {
            const float depth = instance->m_DepthStencilTarget->m_DepthClearValue;
            const uint8_t stencil = instance->m_DepthStencilTarget->m_StencilClearValue;
            if (instance->m_DepthStencilTarget->m_Format == RenderTargetFormat::Depth32F)
                glClearNamedFramebufferfv(instance->m_ID, GL_DEPTH, 0, &depth);
            else
                glClearNamedFramebufferfi(instance->m_ID, GL_DEPTH_STENCIL, 0, depth, stencil);
        }
    });
}
void OpenGLRenderPass::Resize(uint32_t width, uint32_t height) {
    m_Width = width;
    m_Height = height;
    for (auto& renderTarget : m_RenderTargets) {
        if (renderTarget)
            renderTarget->Resize(m_Width, m_Height);
    }
    if (m_DepthStencilTarget)
        m_DepthStencilTarget->Resize(m_Width, m_Height);
}

} //namespace Saba
