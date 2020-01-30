#pragma once

#include "RendererAPI.h"

namespace Saba {

	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			s_API->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_API->Clear();
		}
		
		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_API->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_API;
	};

}
