#pragma once

#include "Saba/Renderer/Shader.h"
#include "Saba/Renderer/Camera.h"
#include "Saba/Renderer/Mesh.h"
#include "Saba/Renderer/Texture.h"

namespace Saba {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static Ref<Shader> GetCurrentShader();
		static void SetShader(Ref<Shader> shader = {});

		static void BeginScene(const Camera& camera, const glm::mat4& tranform = glm::mat4(1.0f));
		static void BeginScene(const glm::mat4& viewProjectionMatrix);

		static void DrawMesh(Ref<Mesh> mesh, glm::mat4 transform = glm::mat4(1.0f));

		struct Stats
		{
			uint32_t verticesCount = 0;
			uint32_t indicesCount = 0;
			uint32_t drawCalls = 0;
		};
		static void ResetStats();
		static const Stats& GetStats();
	};
}
