#pragma once

#include "Texture.h"

namespace Saba {

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();
		static void Flush();

		static void BeginSceneTriangle();
		static void EndSceneTriangle();
		static void FlushQuad();

		static void BeginSceneQuad();
		static void EndSceneQuad();
		static void FlushTriangle();

		static void DrawTriangle(const std::array<std::pair<glm::vec3, glm::vec2>, 3> & posUV, glm::vec4 color);
		static void DrawTriangle(const std::array<std::pair<glm::vec3, glm::vec2>, 3> & posUV, Ref<Texture2D> texture);
		static void DrawQuad(const std::array<glm::vec3, 4> & pos, glm::vec4 color);
		static void DrawQuad(const std::array<glm::vec3, 4> & pos, Ref<Texture2D> texture);

		struct Stats
		{
			uint32_t triangleCount = 0;
			uint32_t quadCount = 0;
			uint32_t drawCallsOnTriangles = 0;
			uint32_t drawCallsOnQuads = 0;
		};

		static void ResetStats();
		static const Stats& GetStats();
	};
}
