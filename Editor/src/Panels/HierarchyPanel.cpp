#include <pch.h>
#include <Saba.h>
#include "HierarchyPanel.h"

#include <imgui/imgui.h>
#include "Saba/ImGui/SabaImGui.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Saba {

	Saba::SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		: m_Scene(scene) {}

	void Saba::SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
	}

	void Saba::SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene hierarchy");

		m_Scene->m_Registry.each([&](auto id)
		{
			Entity entity(id, m_Scene.get());
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_Selected = {};

		ImGui::End();

		ImGui::Begin("Properties");

		DrawComponents(m_Selected);

		ImGui::End();
	}

	void Saba::SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		if (ImGui::Selectable(tag.c_str(), m_Selected == entity))
			m_Selected = entity;
	}
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (!entity) return;
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			static char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
				tag = buffer;
		}
		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx("Transform component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& tc = entity.GetComponent<TransformComponent>();

				Saba::DragFloat3("Pos", tc.Pos, 0.0f, 0.0f, 0.0f, 0.1f);
				Saba::DragFloat3("Orientation", tc.Orientation, 0.0f, 0.0f, 0.0f, 0.01f);
				Saba::DragFloat3("Scale", tc.Scale, 1.0f, 0.01f, 10.0f, 0.01f);

				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<SpriteComponent>())
		{
			if (ImGui::TreeNodeEx("Sprite component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& sprc = entity.GetComponent<SpriteComponent>();

				Saba::ColorEdit4("Color", sprc.Color);
				Saba::DragFloat("Tilling factor", sprc.TillingFactor, 1.0f, 0.01f, 10.0f, 0.01f);
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx("Camera component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& cc = entity.GetComponent<CameraComponent>();

				ImGui::Checkbox("Primary camera", &cc.Primary);
				
				static constexpr char* typeStrings[] = { "Perspective", "Orthographic" };
				const char* currentType = typeStrings[(int)cc.Camera.m_Type];
				if (ImGui::BeginCombo("Projection", currentType))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentType == typeStrings[i];
						if (ImGui::Selectable(typeStrings[i], isSelected))
						{
							currentType = typeStrings[i];
							cc.Camera.m_Type = (SceneCamera::Type)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (cc.Camera.m_Type == SceneCamera::Type::Perspective)
				{
					Saba::DragFloat("FOV", cc.Camera.m_PerspectiveFov, glm::half_pi<float>(), glm::pi<float>() / 18.0f, glm::quarter_pi<float>() * 3.0f, 0.1f);
					Saba::DragFloat("Near clip", cc.Camera.m_PerspectiveNear, 0.01f, 0.01f, 0.5f, 0.01f);
					Saba::DragFloat("Far clip", cc.Camera.m_PerspectiveFar, 10.0f, 1.0f, 100.0f, 1.0f);
				}
				else
				{
					Saba::DragFloat("Size", cc.Camera.m_OrthographicSize, 10.0f, 0.5f, 20.0f, 0.1f);
					Saba::DragFloat("Near clip", cc.Camera.m_OrthographicNear, -1.0f, -10.0f, -1.0f, 0.1f);
					Saba::DragFloat("Far clip", cc.Camera.m_OrthographicFar, 1.0f, 1.0f, 10.0f, 0.1f);
				}

				ImGui::Checkbox("Fixed aspect ratio", &cc.FixedAspectRatio);
				if (cc.FixedAspectRatio)
					Saba::DragFloat("Aspect ratio", cc.Camera.m_AspectRatio, (float)m_Scene->m_ViewportSize.x / (float)m_Scene->m_ViewportSize.y, 0.25f, 4.0f, 0.01f);
				else cc.Camera.m_AspectRatio = (float)m_Scene->m_ViewportSize.x / (float)m_Scene->m_ViewportSize.y;

				cc.Camera.Recalculate();
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<NativeScriptComponent>())
		{
			if (ImGui::TreeNodeEx("Native script component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::TreePop();
			}
		}
	}
}
