#include <pch.h>
#include "DX11Context.h"

#include "DXErr.h"
#include <dxgi1_3.h>

#include "Saba/Platform/Windows/WindowsWindow.h"
#include "Saba/Renderer/Renderer.h"

namespace Saba {

void DX11Context::Init() {
#if defined(SB_DEBUG)
    s_DXGIInfoManager.Init();
#endif

    uint32_t deviceCreateFlags = 0u;
#if defined(SB_DEBUG)
    deviceCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr;
    SB_DX_GRAPHICS_CALL_INFO(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceCreateFlags, nullptr, 0, D3D11_SDK_VERSION,
        &m_Device, nullptr, &m_Context));
}

void DX11Context::InitForWindow(Window* window) {
    WindowsWindow* wind = reinterpret_cast<WindowsWindow*>(window);
    auto& [swapChain, buffer] = m_WindowData[wind->m_Window];

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.Width = wind->m_Data.width;
    swapChainDesc.BufferDesc.Height = wind->m_Data.height;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0u;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 0u;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1u;
    swapChainDesc.SampleDesc.Quality = 0u;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2u;
    swapChainDesc.OutputWindow = wind->m_Window;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

    ComPtr<IDXGIDevice3> dxgiDevice;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<IDXGIFactory> factory;
    m_Device.As(&dxgiDevice);
    HRESULT hr = dxgiDevice->GetAdapter(&adapter);
    if (SUCCEEDED(hr)) {
        adapter->GetParent(IID_PPV_ARGS(&factory));
        SB_DX_GRAPHICS_CALL_INFO(factory->CreateSwapChain(m_Device.Get(), &swapChainDesc, &swapChain));
    }

    ComPtr<ID3D11Texture2D> backBuffer;
    SB_DX_GRAPHICS_CALL_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
    SB_DX_GRAPHICS_CALL_INFO(m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, &buffer));
}

void DX11Context::ShutdownForWindow(Window* window) {
    WindowsWindow* wind = reinterpret_cast<WindowsWindow*>(window);
    auto& [swapChain, buffer] = m_WindowData[wind->m_Window];
    buffer.Reset();
    swapChain.Reset();
}

void DX11Context::SwapBuffers(Window* window) {
    WindowsWindow* wind = reinterpret_cast<WindowsWindow*>(window);
    auto& [swapChain, buffer] = m_WindowData[wind->m_Window];
    HRESULT hr;
    if (wind->m_Data.vSync)
        SB_DX_GRAPHICS_CALL_INFO(swapChain->Present(1u, 0u))
    else
        SB_DX_GRAPHICS_CALL_INFO(swapChain->Present(0u, DXGI_PRESENT_ALLOW_TEARING));
}

void DX11Context::RecreateSwapChain(Window* window) {
    WindowsWindow* wnd = (WindowsWindow*)window;
    auto& [swapChain, targetView] = m_WindowData[wnd->m_Window];

    if (swapChain && wnd->m_Data.width != 0 && wnd->m_Data.height != 0) {
        m_Context->OMSetRenderTargets(0u, nullptr, nullptr);
        targetView.Reset();
        HRESULT hr;
        SB_DX_GRAPHICS_CALL_INFO(swapChain->ResizeBuffers(0u, 0u, 0u, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING));

        ComPtr<ID3D11Texture2D> backBuffer;
        SB_DX_GRAPHICS_CALL_INFO(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer));
        SB_DX_GRAPHICS_CALL_INFO(DX11Context::GetContextFromApplication()->GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, &targetView));
    }
}
void DX11Context::BindToRender(Window* window) {
    Ref<DX11Context> instance = this;
    Renderer::Submit([instance, window]() mutable {
        WindowsWindow* wind = reinterpret_cast<WindowsWindow*>(window);
        auto& [swapChain, buffer] = instance->m_WindowData[wind->m_Window];

        instance->m_Context->OMSetRenderTargets(1u, buffer.GetAddressOf(), nullptr);
        D3D11_VIEWPORT viewport;
        viewport.Width = static_cast<float>(wind->m_Data.width);
        viewport.Height = static_cast<float>(wind->m_Data.height);
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        instance->m_Context->RSSetViewports(1u, &viewport);
    });
}
void DX11Context::Clear(Window* window, const glm::vec4& color) {
    Ref<DX11Context> instance = this;
    Renderer::Submit([instance, window, color]() mutable {
        WindowsWindow* wind = reinterpret_cast<WindowsWindow*>(window);
        auto& [swapChain, buffer] = instance->m_WindowData[wind->m_Window];

        instance->m_Context->ClearRenderTargetView(buffer.Get(), glm::value_ptr(color));
    });
}


#if defined(SB_DEBUG)
DxgiInfoManager DX11Context::s_DXGIInfoManager;
#endif

DX11Context::GraphicsException::GraphicsException(int line, const char* file, HRESULT hresult, std::string infoMessage) noexcept
    : SabaException(line, file, true), m_Result(hresult), m_Info(std::move(infoMessage)) {}
const char* DX11Context::GraphicsException::what() const noexcept {
    std::ostringstream oss;
    oss << GetType() << std::endl;
    oss << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode() << std::dec << " (" << (uint32_t)m_Result << ")" << std::endl;
    oss << "[Error String] " << GetErrorString() << std::endl;
    oss << "[Description] " << GetErrorDescription() << std::endl;

    if (!m_Info.empty())
        oss << std::endl << "[Error Info]" << std::endl << m_Info << std::endl << std::endl;

    oss << GetOriginString();
    m_Buffer = oss.str();
    return m_Buffer.c_str();
}
std::string DX11Context::GraphicsException::GetErrorString() const noexcept {
    return DXGetErrorStringA(m_Result);
}
std::string DX11Context::GraphicsException::GetErrorDescription() const noexcept {
    return {};
}

}
