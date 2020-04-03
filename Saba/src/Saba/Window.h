#pragma once

#include "Events\Event.h"

namespace Saba {

	struct WindowProps
	{
		std::string Title;
		uint Width, Height;

		WindowProps(const std::string& title = "Saba", uint width = 1280, uint height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		using EventCallbackFN = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFN& callback) = 0;
		virtual void SetVSync(bool vsync) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetCursor(bool enabled) = 0;
		virtual bool IsCursorEnabled() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Ref<Window> Create(const WindowProps& props = WindowProps());
	};

}
