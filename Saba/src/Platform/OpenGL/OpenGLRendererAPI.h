#pragma once

#include "Saba\Renderer\RendererAPI.h"

namespace Saba {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void EnableDepthTest() override;
		virtual void DisableDepthTest() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, RendererAPI::RenderTopology topology) override;
		virtual void DrawIndexed(uint32_t indicesCount, RendererAPI::RenderTopology topology) override;

		virtual void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instancesCount, RenderTopology topology) override;
		virtual void DrawIndexedInstanced(uint32_t indicesCount, uint32_t instancesCount, RenderTopology topology) override;
	};

}
