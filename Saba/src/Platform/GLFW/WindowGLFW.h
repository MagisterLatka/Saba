#pragma once

#include "Saba\Window.h"
#include "Saba\Renderer\GraphicsContext.h"

#include <GLFW\glfw3.h>

namespace Saba {

	class WindowGLFW : public Window
	{
	public:
		WindowGLFW(const WindowProps& props);
		virtual ~WindowGLFW();

		void OnUpdate() override;

		inline uint GetWidth() const override { return m_Data.width; }
		inline uint GetHeight() const override { return m_Data.height; }

		inline void SetEventCallback(const EventCallbackFN& func) override;
		void SetVSync(bool isVSync) override;
		bool IsVSync() const override;

		virtual void SetCursor(bool enabled) override;
		inline virtual bool IsCursorEnabled() const override { return m_IsCursorEnabled; }

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		void Init(const WindowProps& props);
		void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;
		bool m_IsCursorEnabled = false;

		struct WindowData
		{
			std::string title;
			uint width, height;
			bool vSync;

			EventCallbackFN eventCallback;
		};
		WindowData m_Data;
	};

}
