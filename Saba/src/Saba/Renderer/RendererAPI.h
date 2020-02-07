#pragma once

#include "VertexArray.h"

#include <glm\glm.hpp>

namespace Saba {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL
		};
	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};

}
