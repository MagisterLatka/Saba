#pragma once

#include "Saba/Core.h"
#include "Saba/Renderer/RendererAPI.h"

namespace Saba {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_API->Init();
		}
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_API->SetViewport(x, y, width, height);
		}

		static void SetClearColor(glm::vec4 color)
		{
			s_API->SetClearColor(color);
		}
		static void Clear()
		{
			s_API->Clear();
		}

		static void EnableDepthTest(bool enable = true)
		{
			s_API->EnableDepthTest(enable);
		}
		
		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indicesCount = 0)
		{
			s_API->DrawIndexed(vertexArray, indicesCount);
		}
	private:
		static Scope<RendererAPI> s_API;
	};

}
