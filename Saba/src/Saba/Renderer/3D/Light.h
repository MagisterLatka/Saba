#pragma once

namespace Saba {

	class Light
	{
		friend class Scene3D;
	public:
		Light() = default;
		virtual ~Light() = default;

		virtual void SetPos(glm::vec3 pos) = 0;
		inline virtual glm::vec3 GetPos() const { return m_Pos; }
		virtual void SetDir(glm::vec3 dir) = 0;
		inline virtual glm::vec3 GetDir() const { return m_Dir; }
		virtual void SetDiffuseColor(glm::vec3 color) = 0;
		inline virtual glm::vec3 GetDiffuseColor() const { return m_DiffuseColor; }
		virtual void SetSpecularColor(glm::vec3 color) = 0;
		inline virtual glm::vec3 GetSpecularColor() const { return m_SpecularColor; }

		struct LightData
		{
			glm::vec4 pos;
			glm::vec4 dir;
			alignas(16) glm::vec3 diffuseColor;
			alignas(16) glm::vec3 speculatColor;
			alignas(16) glm::vec2 cutsoff;
		};
		virtual LightData* GetData() = 0;
		virtual LightData* GetData(LightData* bufferPtr) = 0;
	protected:
		glm::vec3 m_Pos;
		glm::vec3 m_Dir;
		glm::vec3 m_DiffuseColor;
		glm::vec3 m_SpecularColor;
		float m_CutOff, m_OuterCutOff;
	};
}
