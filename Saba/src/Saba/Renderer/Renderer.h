#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Saba {

	class Renderer
	{
	public:
		static void Init();

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
