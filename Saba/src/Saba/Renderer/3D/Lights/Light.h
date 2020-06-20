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

		virtual glm::mat4 SetShadowData(const std::pair<glm::vec2, glm::vec2>& shadowTextureSpace) = 0;
		
		struct LightData
		{
			glm::vec4 pos;
			glm::vec4 dir;
			glm::vec4 diffuseColor;
			glm::vec4 specularColor;
			glm::vec4 cutsoff;
			glm::vec4 attenuation;
			glm::vec4 shadowTextureSpace;
			glm::mat4 dirLightSpace;
		};
		virtual LightData* GetData() = 0;
		virtual LightData* GetData(LightData* bufferPtr) = 0;
	protected:
		glm::vec3 m_Pos;
		glm::vec3 m_Dir;
		glm::vec3 m_DiffuseColor;
		glm::vec3 m_SpecularColor;
		float m_CutOff, m_OuterCutOff;

		std::pair<glm::vec2, glm::vec2> m_ShadowTextureSpace;

		float m_AttConstant, m_AttLinear, m_AttQuadratic;
	};
}
