#pragma once

#include "Saba/Renderer/RenderCommand.h"
#include "Saba/Renderer/Renderer2D.h"

namespace Saba {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
	};

}
