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
		enum RenderTopology
		{
			None = 0, Triangles, TriangleStrip
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, RenderTopology topology) = 0;
		virtual void DrawIndexed(uint32_t indicesCount, RenderTopology topology) = 0;

		virtual void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instancesCount, RenderTopology topology) = 0;
		virtual void DrawIndexedInstanced(uint32_t indicesCount, uint32_t instancesCount, RenderTopology topology) = 0;

		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};

}
