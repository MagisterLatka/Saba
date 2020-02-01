#include "pch.h"
#include "Renderer.h"

namespace Saba {

	Renderer::Scene* Renderer::s_Scene = new Renderer::Scene();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_Scene->ViewProjMat = camera.GetViewProjectionMat();
	}
	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->SetUniformMat4("u_ViewProjMat", s_Scene->ViewProjMat);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}
