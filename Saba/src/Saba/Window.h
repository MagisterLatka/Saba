#pragma once

#include "Saba/Core.h"
#include "Saba/Events/Event.h"

namespace Saba {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width, Height;

		WindowProps(const std::string& title = "Saba", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		using EventCallbackFN = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual int GetWindowPosX() const = 0;
		virtual int GetWindowPosY() const = 0;

		virtual void SetEventCallback(const EventCallbackFN& callback) = 0;
		virtual void SetVSync(bool vsync) = 0;
		virtual bool IsVSync() const = 0;

		virtual void EnableCursor(bool enable = true) = 0;
		virtual bool IsCursorEnabled() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}
