#include <pch.h>
#include <Saba.h>
#include "HierarchyPanel.h"

#include <ImGui/imgui.h>
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
			ImGui::TextUnformatted(entity.GetComponent<TagComponent>().Tag.c_str());
		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx("Transform component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& tc = entity.GetComponent<TransformComponent>();

				ImGui::Text("[%.3f  %.3f  %.3f  %.3f]", tc.Transform[0][0], tc.Transform[1][0], tc.Transform[2][0], tc.Transform[3][0]);
				ImGui::Text("[%.3f  %.3f  %.3f  %.3f]", tc.Transform[0][1], tc.Transform[1][1], tc.Transform[2][1], tc.Transform[3][1]);
				ImGui::Text("[%.3f  %.3f  %.3f  %.3f]", tc.Transform[0][2], tc.Transform[1][2], tc.Transform[2][2], tc.Transform[3][2]);
				ImGui::Text("[%.3f  %.3f  %.3f  %.3f]", tc.Transform[0][3], tc.Transform[1][3], tc.Transform[2][3], tc.Transform[3][3]);

				ImGui::Separator();

				ImGui::TextUnformatted("Pos              "); ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
				ImGui::DragFloat3("###0", glm::value_ptr(tc.Pos), 0.1f);
				ImGui::PopItemWidth();

				if (entity.HasComponent<CameraComponent>())
				{
					if (entity.HasComponent<NativeScriptComponent>())
					{
						auto ea = tc.EulerAngles;
						ImGui::TextUnformatted("Euler angles     "); ImGui::SameLine();
						ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
						ImGui::DragFloat3("###1", glm::value_ptr(ea), 0.0f);
						ImGui::PopItemWidth();
					}
					else
					{
						ImGui::TextUnformatted("Euler angles     "); ImGui::SameLine();
						ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
						ImGui::DragFloat3("###1", glm::value_ptr(tc.EulerAngles), 0.01f);
						ImGui::PopItemWidth();
					}
					auto scale = tc.Scale;
					ImGui::TextUnformatted("Scale            "); ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
					ImGui::DragFloat3("###2", glm::value_ptr(scale), 0.0f);
					ImGui::PopItemWidth();
				}
				else
				{
					ImGui::TextUnformatted("Euler angles     "); ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
					ImGui::DragFloat3("###1", glm::value_ptr(tc.EulerAngles), 0.01f);
					ImGui::PopItemWidth();
					ImGui::TextUnformatted("Scale            "); ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
					ImGui::DragFloat3("###2", glm::value_ptr(tc.Scale), 0.1f, 0.01f, 10.0f);
					ImGui::PopItemWidth();
				}

				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<SpriteComponent>())
		{
			if (ImGui::TreeNodeEx("Sprite component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& sprc = entity.GetComponent<SpriteComponent>();

				ImGui::Checkbox("Use transform", &sprc.UseTransform);
				ImGui::TextUnformatted("Position         "); ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
				ImGui::DragFloat3("###0", glm::value_ptr(sprc.Pos), 0.1f);
				ImGui::PopItemWidth();
				ImGui::TextUnformatted("Size             "); ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
				ImGui::DragFloat2("###1", glm::value_ptr(sprc.Size), 0.1f, 0.001f, 10.0f);
				ImGui::PopItemWidth();
				ImGui::TextUnformatted("Color            "); ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
				ImGui::ColorEdit4("###2", glm::value_ptr(sprc.Color));
				ImGui::PopItemWidth();
				ImGui::TextUnformatted("Tilling factor   "); ImGui::SameLine();
				ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
				ImGui::DragFloat("###3", &sprc.TillingFactor, 0.1f, 0.01f, 10.0f);
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
		}
		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx("Camera component", ImGuiTreeNodeFlags_DefaultOpen))
			{
				auto& cc = entity.GetComponent<CameraComponent>();

				ImGui::Checkbox("Primary camera", &cc.Primary);
				if (cc.Camera.GetType() == SceneCamera::Type::Orthographic)
				{
					ImGui::TextUnformatted("Type: orthographic camera");
					ImGui::TextUnformatted("Orthographic size"); ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
					ImGui::DragFloat("###0", &cc.Camera.m_Fov, 0.1f, 0.5f, 20.0f);
					ImGui::PopItemWidth();
				}
				else if (cc.Camera.GetType() == SceneCamera::Type::Perspective)
				{
					ImGui::TextUnformatted("Type: perspective camera");
					ImGui::TextUnformatted("FOV              "); ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
					ImGui::DragFloat("###0", &cc.Camera.m_Fov, 0.1f, glm::pi<float>() / 18.0f, glm::quarter_pi<float>() * 3.0f);
					ImGui::PopItemWidth();
					ImGui::TextUnformatted("Near clip plane  "); ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
					ImGui::DragFloat("###1", &cc.Camera.m_NearClip, 0.01f, 0.01f, 0.5f);
					ImGui::PopItemWidth();
					ImGui::TextUnformatted("Far clip plane   "); ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
					ImGui::DragFloat("###2", &cc.Camera.m_FarClip, 0.1f, 1.0f, 100.0f);
					ImGui::PopItemWidth();
				}
				else
				{
					ImGui::TextUnformatted("Type: none");
				}
				ImGui::Checkbox("Fixed aspect ratio", &cc.FixedAspectRatio);
				if (cc.FixedAspectRatio)
				{
					ImGui::TextUnformatted("Aspect ratio     "); ImGui::SameLine();
					ImGui::PushItemWidth(ImGui::GetWindowWidth() - ImGui::GetFontSize() * 12.0f);
					ImGui::DragFloat("###3", &cc.Camera.m_AspectRatio, 0.01f, 0.25f, 4.0f);
					ImGui::PopItemWidth();
				}
				else
					cc.Camera.SetViewportSize(m_Scene->GetViewportSize().x, m_Scene->GetViewportSize().y);

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
