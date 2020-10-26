#pragma once

#include "Saba/Layers/Layer.h"

namespace Saba {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvent(bool block) { m_BlockEvents = block; }
		bool IsBlockingEvents() const { return m_BlockEvents; }

		void EnableMouseEvents(bool enable = true);

		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
	};

}
