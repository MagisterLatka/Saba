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

		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		void Init(const WindowProps& props);
		void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		struct WindowData
		{
			const char* title;
			uint width, height;
			bool vSync;

			EventCallbackFN eventCallback;
		};
		WindowData m_Data;
	};

}
