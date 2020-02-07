#include "pch.h"
#include "Renderer.h"

namespace Saba {

	Scope<Renderer::Scene> Renderer::s_Scene = MakeScope<Renderer::Scene>();

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_Scene->ViewProjMat = camera.GetViewProjectionMat();
	}
	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjMat", s_Scene->ViewProjMat);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
