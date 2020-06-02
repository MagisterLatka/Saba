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

		inline static void EnableDepthTest()
		{
			s_API->EnableDepthTest();
		}
		inline static void DisableDepthTest()
		{
			s_API->DisableDepthTest();
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_API->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_API->Clear();
		}
		
		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, RendererAPI::RenderTopology topology = RendererAPI::Triangles)
		{
			s_API->DrawIndexed(vertexArray, topology);
		}
		inline static void DrawIndexed(uint32_t indicesCount, RendererAPI::RenderTopology topology = RendererAPI::Triangles)
		{
			s_API->DrawIndexed(indicesCount, topology);
		}

		inline static void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instancesCount, RendererAPI::RenderTopology topology = RendererAPI::Triangles)
		{
			s_API->DrawIndexedInstanced(vertexArray, instancesCount, topology);
		}
		inline static void DrawIndexedInstanced(uint32_t indicesCount, uint32_t instancesCount, RendererAPI::RenderTopology topology = RendererAPI::Triangles)
		{
			s_API->DrawIndexedInstanced(indicesCount, instancesCount, topology);
		}
	private:
		static Scope<RendererAPI> s_API;
	};

}
