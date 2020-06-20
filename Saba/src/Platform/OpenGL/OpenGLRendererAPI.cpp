#include "pch.h"
#include "OpenGLRendererAPI.h"
#include "OpenGLError.h"

#include <glad\glad.h>

namespace Saba {

	void OpenGLRendererAPI::Init()
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		GLCall(glViewport(x, y, width, height));
	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		GLCall(glDepthMask(GL_TRUE));
	}
	void OpenGLRendererAPI::DisableDepthTest()
	{
		GLCall(glDepthMask(GL_FALSE));
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		GLCall(glClearColor(color.r, color.g, color.b, color.a));
	}
	void OpenGLRendererAPI::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, RendererAPI::RenderTopology topology)
	{
		unsigned int t = topology == Triangles ? GL_TRIANGLES : topology == TriangleStrip ? GL_TRIANGLE_STRIP : 0;
		GLCall(glDrawElements(t, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr));
	}
	void OpenGLRendererAPI::DrawIndexed(uint32_t indicesCount, RendererAPI::RenderTopology topology)
	{
		unsigned int t = topology == Triangles ? GL_TRIANGLES : topology == TriangleStrip ? GL_TRIANGLE_STRIP : 0;
		GLCall(glDrawElements(t, indicesCount, GL_UNSIGNED_INT, nullptr));
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instancesCount, RenderTopology topology)
	{
		unsigned int t = topology == Triangles ? GL_TRIANGLES : topology == TriangleStrip ? GL_TRIANGLE_STRIP : 0;
		GLCall(glDrawElementsInstanced(t, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, instancesCount));
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(uint32_t indicesCount, uint32_t instancesCount, RenderTopology topology)
	{
		unsigned int t = topology == Triangles ? GL_TRIANGLES : topology == TriangleStrip ? GL_TRIANGLE_STRIP : 0;
		GLCall(glDrawElementsInstanced(t, indicesCount, GL_UNSIGNED_INT, nullptr, instancesCount));
	}

}
