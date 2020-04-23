#include "pch.h"
#include "OpenGLRendererAPI.h"

#include <glad\glad.h>

namespace Saba {

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, RendererAPI::RenderTopology topology)
	{
		unsigned int t = topology == Triangles ? GL_TRIANGLES : topology == TriangleStrip ? GL_TRIANGLE_STRIP : 0;
		glDrawElements(t, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::DrawIndexed(uint32_t indicesCount, RendererAPI::RenderTopology topology)
	{
		unsigned int t = topology == Triangles ? GL_TRIANGLES : topology == TriangleStrip ? GL_TRIANGLE_STRIP : 0;
		glDrawElements(t, indicesCount, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instancesCount, RenderTopology topology)
	{
		unsigned int t = topology == Triangles ? GL_TRIANGLES : topology == TriangleStrip ? GL_TRIANGLE_STRIP : 0;
		glDrawElementsInstanced(t, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, instancesCount);
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(uint32_t indicesCount, uint32_t instancesCount, RenderTopology topology)
	{
		unsigned int t = topology == Triangles ? GL_TRIANGLES : topology == TriangleStrip ? GL_TRIANGLE_STRIP : 0;
		glDrawElementsInstanced(t, indicesCount, GL_UNSIGNED_INT, nullptr, instancesCount);
	}

}
