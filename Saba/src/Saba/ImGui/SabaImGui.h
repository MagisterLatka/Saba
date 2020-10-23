#pragma once

#include <imgui.h>
#include <imfilebrowser.h>

namespace Saba {

	class ImFileBrowser
	{
	public:
		explicit ImFileBrowser(ImGuiFileBrowserFlags flags = 0, const std::vector<const char*>& typeFilters = {}, const std::string& title = {});
		ImFileBrowser(const ImFileBrowser&) = default;
		ImFileBrowser& operator=(const ImFileBrowser&) = default;

		void SetTitle(std::string title);
		void Open();
		void Close();
		bool IsOpened() const noexcept;
		void Display();
		bool HasSelected() const noexcept;
		bool SetPwd(const std::filesystem::path& pwd = std::filesystem::current_path());
		const std::filesystem::path& GetPwd() const noexcept;
		std::filesystem::path GetSelected() const;
		std::vector<std::filesystem::path> GetMultiSelected() const;
		void ClearSelected();
		void SetTypeFilters(const std::vector<const char*>& typeFilters);
	private:
		ImGui::FileBrowser m_FileBrowser;
	};

	bool DragFloat(const std::string& label, float& value, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragFloat2(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragFloat3(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f, float columnWidth = 100.0f);
	bool DragFloat4(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float minVal = 0.0f, float maxVal = 0.0f, float speed = 0.0f, float columnWidth = 100.0f);

	bool ColorEdit3(const std::string& label, glm::vec3& values, float columnWidth = 100.0f);
	bool ColorEdit4(const std::string& label, glm::vec4& values, float columnWidth = 100.0f);

}
