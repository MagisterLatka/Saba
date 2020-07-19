#include "pch.h"
#include "Scene3D.h"

#include "Lights\DirectionalLight.h"
#include "Lights\PointLight.h"
#include "Lights\SpotLight.h"

#include "Saba\Renderer\3D\Renderer3D.h"
#include "Saba\Renderer\RenderCommand.h"

namespace Saba {

	Scene3D::Scene3D()
	{
		m_Lights.fill(nullptr);

		m_DirShadowFramebuffer = Saba::Framebuffer::Create();
		m_DirShadowTexture = Saba::Texture2D::Create((int)sqrt(c_MaxDirLights) * c_ShadowTexResolution, (int)sqrt(c_MaxDirLights) * c_ShadowTexResolution, Saba::Texture::Format::Depth16);
		m_DirShadowFramebuffer->Bind();
		m_DirShadowFramebuffer->AttachTexture(m_DirShadowTexture, Saba::Framebuffer::Attachment::Depth);
		m_DirShadowFramebuffer->DrawMode(Saba::Framebuffer::Attachment::None);
		m_DirShadowFramebuffer->ReadMode(Saba::Framebuffer::Attachment::None);
	
		m_PointShadowFramebuffer = Saba::Framebuffer::Create();
		m_PointShadowTexture = Saba::Texture2D::Create(2 * c_ShadowTexResolution, c_MaxPointLights * c_ShadowTexResolution, Saba::Texture::Format::Depth16);
		m_PointShadowFramebuffer->Bind();
		m_PointShadowFramebuffer->AttachTexture(m_PointShadowTexture, Saba::Framebuffer::Attachment::Depth);
		m_PointShadowFramebuffer->DrawMode(Saba::Framebuffer::Attachment::None);
		m_PointShadowFramebuffer->ReadMode(Saba::Framebuffer::Attachment::None);

		m_SpotShadowFramebuffer = Saba::Framebuffer::Create();
		m_SpotShadowTexture = Saba::Texture2D::Create((int)sqrt(c_MaxSpotLights) * c_ShadowTexResolution, (int)sqrt(c_MaxSpotLights) * c_ShadowTexResolution, Saba::Texture::Format::Depth16);
		m_SpotShadowFramebuffer->Bind();
		m_SpotShadowFramebuffer->AttachTexture(m_SpotShadowTexture, Saba::Framebuffer::Attachment::Depth);
		m_SpotShadowFramebuffer->DrawMode(Saba::Framebuffer::Attachment::None);
		m_SpotShadowFramebuffer->ReadMode(Saba::Framebuffer::Attachment::None);
		m_SpotShadowFramebuffer->Unbind();
	}
	Scene3D::~Scene3D()
	{}

	Scene3DObject* Scene3D::Add(Scene3DObject * object)
	{
		object->m_ID = (uint32_t)m_Objects.size();
		m_Objects.push_back(object);
		return object;
	}
	Scene3DObject* Scene3D::Delete(Scene3DObject* object)
	{
		auto i = std::find(m_Objects.begin(), m_Objects.end(), object);
		if (!*i)
		{
			SB_CORE_ASSERT(false, "There is no such object");
		}
		Scene3DObject* a = *i;
		m_Objects.erase(i);
		return a;
	}

	Scene3DObject* Scene3D::Delete(uint32_t id)
	{
		SB_CORE_ASSERT((m_Objects.size() > id), "There is no object with id {0}", id);
		Scene3DObject* a = m_Objects[id];
		m_Objects.erase(m_Objects.begin() + id);
		return a;
	}

	uint32_t Scene3D::GetID(Scene3DObject* object)
	{
		auto i = std::find(m_Objects.begin(), m_Objects.end(), object);
		if (!*i)
		{
			SB_CORE_ASSERT(false, "There is no such object");
		}
		return (uint32_t)(i - m_Objects.begin());
	}

	void Scene3D::Draw(uint32_t id)
	{
		SB_CORE_ASSERT((m_Objects.size() > id), "There is no object with id {0}", id);
		m_Objects[id]->Draw();
	}
	void Scene3D::DrawAll()
	{
		for (auto& o : m_Objects)
		{
			o->Draw();
		}
	}

	Light* Scene3D::AddLight(Light* light)
	{
		bool hasSpace = false;
		uint8_t emptySlot = 255;
		for (uint8_t i = 0; i < c_MaxLights; i++)
		{
			if (!m_Lights[i])
			{
				hasSpace = true;
				emptySlot = i;
				break;
			}
		}
		SB_CORE_ASSERT(hasSpace, "Max lights number extended");
		m_Lights[emptySlot] = light;
		return light;
	}
	Light* Scene3D::DeleteLight(Light* light)
	{
		auto i = std::find(m_Lights.begin(), m_Lights.end(), light);
		if (!*i)
		{
			SB_CORE_ASSERT(false, "There is no such light");
		}
		Light* a = *i;
		*i = nullptr;
		return a;
	}
	Light* Scene3D::DeleteLight(uint8_t id)
	{
		Light* light = nullptr;
		if (id < c_MaxLights)
		{
			light = m_Lights[id];
			m_Lights[id] = nullptr;
		}
		return light;
	}
	uint32_t Scene3D::GetLightID(Light* light)
	{
		auto i = std::find(m_Lights.begin(), m_Lights.end(), light);
		if (!*i)
		{
			SB_CORE_ASSERT(false, "There is no such light");
		}
		return (uint32_t)(i - m_Lights.begin());
	}
	Light::LightData* Scene3D::GetLightsData()
	{
		uint32_t size = 0;
		for (uint8_t i = 0; i < c_MaxLights; i++)
		{
			if (m_Lights[i])
				size += sizeof(Light::LightData);
		}

		if (size == 0) return nullptr;

		Light::LightData* data = (Light::LightData*)malloc(size);
		for (uint8_t i = 0, a = 0; i < c_MaxLights; i++)
		{
			if (m_Lights[i])
				m_Lights[i]->GetData(&data[i]);
		}
		return data;
	}
	Light::LightData* Scene3D::GetLightsData(Light::LightData* bufferPtr)
	{
		uint32_t size = 0;
		for (uint8_t i = 0; i < c_MaxLights; i++)
		{
			if (m_Lights[i])
				size += sizeof(Light::LightData);
		}

		if (size == 0) return nullptr;

		for (uint8_t i = 0, a = 0; i < c_MaxLights; i++)
		{
			if (m_Lights[i])
				m_Lights[i]->GetData(&bufferPtr[i]);
		}

		return bufferPtr;
	}

	void Scene3D::DrawShadows(Ref<Shader> dirShadowShader, Ref<Shader> pointShadowShader, Ref<Shader> spotShadowShader)
	{
		for (int i = 0, directionalLights = 0; i < c_MaxLights; i++)
		{
			if (auto light = dynamic_cast<DirectionalLight*>(m_Lights[i]))
			{
				if (directionalLights == 0)
				{
					dirShadowShader->Bind();
					m_DirShadowFramebuffer->Bind();
					RenderCommand::Clear();
				}
				if (directionalLights < c_MaxDirLights)
				{
					const int x = directionalLights % (int)sqrt(c_MaxDirLights), y = directionalLights / (int)sqrt(c_MaxDirLights);
					RenderCommand::SetViewport(x * c_ShadowTexResolution, y * c_ShadowTexResolution, c_ShadowTexResolution, c_ShadowTexResolution);
					auto lightSpace = light->SetShadowData({ {(float)x / sqrtf(c_MaxDirLights), (float)y / sqrtf(c_MaxDirLights)},
															 {(float)x / sqrtf(c_MaxDirLights) + 1.0f / sqrtf(c_MaxDirLights),
															 (float)y / sqrtf(c_MaxDirLights) + 1.0f / sqrtf(c_MaxDirLights)} });
					dirShadowShader->SetUniformMat4("u_LightSpace", *lightSpace);
					DrawAll();
					Renderer3D::Flush();
				}
				directionalLights++;
			}
		}
		for (int i = 0, pointLights = 0; i < c_MaxLights; i++)
		{
			if (auto light = dynamic_cast<PointLight*>(m_Lights[i]))
			{
				if (pointLights == 0)
				{
					pointShadowShader->Bind();
					m_PointShadowFramebuffer->Bind();
					RenderCommand::Clear();
				}
				if (pointLights < c_MaxPointLights)
				{
					RenderCommand::SetViewport(0, pointLights * c_ShadowTexResolution, 2 * c_ShadowTexResolution, c_ShadowTexResolution);
					auto lightSpace = light->SetShadowData({ {0.0f, (float)pointLights / (float)c_MaxPointLights}, {1.0f, (float)pointLights / (float)c_MaxPointLights + 1.0f / (float)c_MaxPointLights} });
					for (int j = 0; j < 2; j++)
						pointShadowShader->SetUniformMat4("u_LightSpace[" + std::to_string(j) + "]", lightSpace[j]);
					pointShadowShader->SetUniformFloat1("u_FarPlane", light->GetFarPlane());

					for (uint32_t j = 0; j < GetCount() && j != light->GetObjectID(); j++)
						Draw(j);
					Renderer3D::Flush();
				}
				pointLights++;
			}
		}
		for (int i = 0, spotLights = 0; i < c_MaxLights; i++)
		{
			if (auto light = dynamic_cast<SpotLight*>(m_Lights[i]))
			{
				if (spotLights == 0)
				{
					spotShadowShader->Bind();
					m_SpotShadowFramebuffer->Bind();
					RenderCommand::Clear();
				}
				if (spotLights < c_MaxSpotLights)
				{
					const int x = spotLights % (int)sqrt(c_MaxSpotLights), y = spotLights / (int)sqrt(c_MaxSpotLights);
					RenderCommand::SetViewport(x * c_ShadowTexResolution, y * c_ShadowTexResolution, c_ShadowTexResolution, c_ShadowTexResolution);
					auto lightSpace = light->SetShadowData({ {(float)x / sqrtf((float)c_MaxSpotLights), (float)y / sqrtf((float)c_MaxSpotLights)},
													   {(float)x / sqrtf((float)c_MaxSpotLights) + 1.0f / sqrtf((float)c_MaxSpotLights),
													   (float)y / sqrtf((float)c_MaxSpotLights) + 1.0f / sqrtf((float)c_MaxSpotLights)} });
					spotShadowShader->SetUniformMat4("u_LightSpace", *lightSpace);
					spotShadowShader->SetUniformFloat1("u_CutOff", light->GetOuterCutOff());
					spotShadowShader->SetUniformFloat1("u_FarPlane", light->GetFarPlane());

					for (uint32_t j = 0; j < GetCount() && j != light->GetObjectID(); j++)
						Draw(j);
					Renderer3D::Flush();
				}
				spotLights++;
			}
		}
	}
	void Scene3D::BindShadowTextures(uint8_t dirShadowTexIndex, uint8_t pointShadowTexIndex, uint8_t spotShadowTexIndex)
	{
		m_DirShadowTexture->Bind(dirShadowTexIndex);
		m_PointShadowTexture->Bind(pointShadowTexIndex);
		m_SpotShadowTexture->Bind(spotShadowTexIndex);
	}
	void Scene3D::BindShadowTextures(glm::u8vec3 shadowTexIndexes)
	{
		m_DirShadowTexture->Bind(shadowTexIndexes.x);
		m_PointShadowTexture->Bind(shadowTexIndexes.y);
		m_SpotShadowTexture->Bind(shadowTexIndexes.z);
	}
}
