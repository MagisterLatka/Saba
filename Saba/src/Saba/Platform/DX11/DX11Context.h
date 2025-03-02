#pragma once

#include "Saba/Renderer/GraphicsContext.h"
#include "Saba/Platform/DX11/DXGIInfoManager.h"
#include "Saba/Core/Application.h"

#include <d3d11.h>

namespace Saba {

class DX11Context : public GraphicsContext {
#if defined(SB_DEBUG)
public:
    SB_CORE static DxgiInfoManager s_DXGIInfoManager;
#endif
public:
    class GraphicsException : public SabaException {
    public:
        SB_CORE GraphicsException(int line, const char* file, HRESULT hresult, std::string infoMessage = {}) noexcept;
        SB_CORE const char* what() const noexcept override;
        SB_CORE const char* GetType() const noexcept override { return "Graphics Exception"; }
        SB_CORE HRESULT GetErrorCode() const noexcept { return m_Result; }
        SB_CORE std::string GetErrorString() const noexcept;
        SB_CORE std::string GetErrorDescription() const noexcept;
        SB_CORE const std::string& GetErrorInfo() const noexcept { return m_Info; }
    private:
        HRESULT m_Result;
        std::string m_Info;
    };
public:
    SB_CORE DX11Context() = default;
    SB_CORE ~DX11Context() = default;

    SB_CORE void Init() override;
    SB_CORE void Shutdown() override {}
    SB_CORE void InitForWindow(Window* window) override;
    SB_CORE void ShutdownForWindow(Window *window) override;

    SB_CORE void SwapBuffers(Window *window) override;
    SB_CORE void RecreateSwapChain(Window *window) override;
    SB_CORE void BindWindow([[maybe_unused]] Window *window) override {}
    SB_CORE void BindToRender(Window *window) override;
    SB_CORE void Clear(Window *window, const glm::vec4 &color) override;

    SB_CORE ComPtr<ID3D11Device> GetDevice() { SB_CORE_ASSERT(m_Device, "No DX11Device created"); return m_Device; }
    SB_CORE ComPtr<ID3D11DeviceContext> GetContext() { SB_CORE_ASSERT(m_Context, "No DX11DeviceContext created"); return m_Context; }

    SB_CORE static Ref<DX11Context> GetContextFromApplication() { return Application::Get().GetGraphicsContext().As<DX11Context>(); }
private:
    ComPtr<ID3D11Device> m_Device;
    ComPtr<ID3D11DeviceContext> m_Context;

    std::unordered_map<HWND, std::pair<ComPtr<IDXGISwapChain>, ComPtr<ID3D11RenderTargetView>>> m_WindowData;
};

#define SB_DX_GRAPHICS_EXCEPT(hr) Saba::DX11Context::GraphicsException(__LINE__, __FILE__, hr)
#define SB_DX_GRAPHICS_CALL(func) { if (FAILED(hr = (func))) throw SB_DX_GRAPHICS_EXCEPT(hr); }

#if defined(SOF_DEBUG)
#   define SB_DX_GRAPHICS_CALL_INFO(func) { Saba::DX11Context::s_DxgiInfoManager.Set(); if (FAILED(hr = (func)))\
        throw Saba::DX11Context::GraphicsException(__LINE__, __FILE__, hr, Saba::DX11Context::s_DxgiInfoManager.GetMessages()); }
#   define SB_DX_GRAPHICS_CALL_INFO_ONLY(func) { Saba::DX11Context::s_DxgiInfoManager.Set(); (func); { auto str = Saba::DX11Context::s_DxgiInfoManager.GetMessages();\
        if (!str.empty()) throw Saba::MessageException(__LINE__, __FILE__, str); } }
#else
#   define SB_DX_GRAPHICS_CALL_INFO(func) { if (hr = (func); FAILED(hr)) throw Saba::DX11Context::GraphicsException(__LINE__, __FILE__, hr); }
#   define SB_DX_GRAPHICS_CALL_INFO_ONLY(func) (func)
#endif

}
