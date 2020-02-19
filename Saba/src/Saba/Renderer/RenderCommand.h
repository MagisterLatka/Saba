#pragma once

#include "RendererAPI.h"

namespace Saba {

	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_API->Init();
		}
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_API->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_API->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_API->Clear();
		}
		
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_API->DrawIndexed(vertexArray);
		}
	private:
		static Scope<RendererAPI> s_API;
	};

}
