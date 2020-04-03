#pragma once

#include "Texture.h"

namespace Saba {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const glm::mat4& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec3 pos, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec2 pos, glm::vec2 size, Ref<Texture2D> texture);
		static void DrawQuad(glm::vec3 pos, glm::vec2 size, Ref<Texture2D> texture);

		struct Stats
		{
			uint32_t quadCount = 0;
			uint32_t drawCalls = 0;
		};

		static void ResetStats();
		static const Stats& GetStats();
	};

}
