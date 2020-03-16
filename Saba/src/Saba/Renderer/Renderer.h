#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

#include "Renderer2D.h"

namespace Saba {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);
	private:
		struct Scene
		{
			glm::mat4 ViewProjMat;
		};
		static Scope<Scene> s_Scene;
	};

}
