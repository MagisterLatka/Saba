#pragma once

#include "Saba/Core.h"
#include "Saba/Window.h"
#include "Saba/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Saba {

	class WindowGLFW : public Window
	{
	public:
		WindowGLFW(const WindowProps& props);
		virtual ~WindowGLFW();

		virtual void OnUpdate() override;

		virtual unsigned int GetWidth() const override { return m_Data.width; }
		virtual unsigned int GetHeight() const override { return m_Data.height; }

		virtual int GetWindowPosX() const override { return m_Data.windowPosX; };
		virtual int GetWindowPosY() const override { return m_Data.windowPosY; };

		virtual void SetEventCallback(const EventCallbackFN& func) override;
		virtual void SetVSync(bool isVSync) override;
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		void Init(const WindowProps& props);
		void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			int windowPosX, windowPosY;
			bool vSync;

			EventCallbackFN eventCallback;
		};
		WindowData m_Data;
	};

}
