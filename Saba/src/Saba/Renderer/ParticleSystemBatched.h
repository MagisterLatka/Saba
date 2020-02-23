#pragma once

#include "ParticleSystem.h"
#include "OrthographicCamera.h"

namespace Saba {

	class ParticleSystemBatched : public ParticleSystem
	{
	public:
		ParticleSystemBatched();
		virtual ~ParticleSystemBatched();

		virtual void OnRender() override;
	private:
		float* m_Buffer;
	};

}
