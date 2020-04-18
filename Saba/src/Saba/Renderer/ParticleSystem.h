#pragma once

#include "Saba\Timestep.h"
#include "VertexArray.h"

namespace Saba {

	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(uint32_t maxParticles = 10000);
		ParticleSystem(const ParticleSystem&) = delete;
		ParticleSystem& operator=(const ParticleSystem&) = delete;
		~ParticleSystem();

		void OnUpdate(Timestep ts);
		void OnRender();

		void Emit(ParticleProps& props);
	private:
		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeCounter = 0.0f;

			bool Active = false;
		};
		std::vector<Particle> m_Particles;
		uint32_t m_FirstActive = -1, m_LastActive = -1;
		uint32_t m_Index;

		Ref<VertexArray> m_VAO;
		float* m_Buffer;
	};

}
