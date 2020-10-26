#include "pch.h"
#include "Saba/ImGui/SabaImGui.h"

#include <imgui_internal.h>

namespace Saba {

	ImFileBrowser::ImFileBrowser(ImGuiFileBrowserFlags flags, const std::vector<const char*>& typeFilters, const std::string& title)
		: m_FileBrowser(flags)
	{
		if (!typeFilters.empty()) m_FileBrowser.SetTypeFilters(typeFilters);
		if (!title.empty()) m_FileBrowser.SetTitle(title);
	}

	void ImFileBrowser::SetTitle(std::string title)
	{
		m_FileBrowser.SetTitle(title);
	}
	void ImFileBrowser::Open()
	{
		m_FileBrowser.Open();
	}
	void ImFileBrowser::Close()
	{
		m_FileBrowser.Close();
	}
	bool ImFileBrowser::IsOpened() const noexcept
	{
		return m_FileBrowser.IsOpened();
	}
	void ImFileBrowser::Display()
	{
		m_FileBrowser.Display();
	}
	bool ImFileBrowser::HasSelected() const noexcept
	{
		return m_FileBrowser.HasSelected();
	}
	bool ImFileBrowser::SetPwd(const std::filesystem::path& pwd)
	{
		return m_FileBrowser.SetPwd(pwd);
	}
	const std::filesystem::path& ImFileBrowser::GetPwd() const noexcept
	{
		return m_FileBrowser.GetPwd();
	}
	std::filesystem::path ImFileBrowser::GetSelected() const
	{
		return m_FileBrowser.GetSelected();
	}
	std::vector<std::filesystem::path> ImFileBrowser::GetMultiSelected() const
	{
		return m_FileBrowser.GetMultiSelected();
	}
	void ImFileBrowser::ClearSelected()
	{
		m_FileBrowser.ClearSelected();
	}
	void ImFileBrowser::SetTypeFilters(const std::vector<const char*>& typeFilters)
	{
		m_FileBrowser.SetTypeFilters(typeFilters);
	}



	bool DragFloat(const std::string& label, float& value, float resetValue, float minVal, float maxVal, float speed, float columnWidth)
	{
		auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		ImGui::NextColumn();

		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		bool changed = ImGui::DragFloat("###X", &value, speed, minVal, maxVal);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
		return changed;
	}
	bool DragFloat2(const std::string& label, glm::vec2& values, float resetValue, float minVal, float maxVal, float speed, float columnWidth)
	{
		auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		bool changed = ImGui::DragFloat("###X", &values.x, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		changed |= ImGui::DragFloat("###Y", &values.y, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
		return changed;
	}
	bool DragFloat3(const std::string& label, glm::vec3& values, float resetValue, float minVal, float maxVal, float speed, float columnWidth)
	{
		auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		bool changed = ImGui::DragFloat("###X", &values.x, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		changed |= ImGui::DragFloat("###Y", &values.y, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		changed |= ImGui::DragFloat("###Z", &values.z, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
		return changed;
	}
	bool DragFloat4(const std::string& label, glm::vec4& values, float resetValue, float minVal, float maxVal, float speed, float columnWidth)
	{
		auto& boldFont = ImGui::GetIO().Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		bool changed = ImGui::DragFloat("###X", &values.x, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		changed |= ImGui::DragFloat("###Y", &values.y, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		changed |= ImGui::DragFloat("###Z", &values.z, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.8f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.9f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.8f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("W", buttonSize))
			values.w = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		changed |= ImGui::DragFloat("###W", &values.w, speed, minVal, maxVal);
		ImGui::PopItemWidth();


		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
		return changed;
	}

	bool ColorEdit3(const std::string& label, glm::vec3& values, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		ImGui::NextColumn();
		
		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		bool changed = ImGui::ColorEdit3("###C", &values.x);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::PopID();
		return changed;
	}
	bool ColorEdit4(const std::string& label, glm::vec4& values, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::TextUnformatted(label.c_str());
		ImGui::NextColumn();

		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
		bool changed = ImGui::ColorEdit4("###C", &values.x);
		ImGui::PopStyleVar();
		ImGui::PopItemWidth();
		ImGui::Columns(1);
		ImGui::PopID();
		return changed;
	}

	void HelpMarker(const char* desc) //From imgui_demo.cpp
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}
