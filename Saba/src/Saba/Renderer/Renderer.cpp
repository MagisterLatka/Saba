#include "pch.h"
#include "Renderer.h"

namespace Saba {

	Scope<Renderer::Scene> Renderer::s_Scene = MakeScope<Renderer::Scene>();

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
		Renderer3D::Init();
	}
	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
		Renderer3D::Shutdown();
	}
	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

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
