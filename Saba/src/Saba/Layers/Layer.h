#pragma once

#include "Saba\Events\Event.h"
#include "Saba\Timestep.h"

namespace Saba {

	class Layer
	{
	public:
		Layer(const std::string& name);
		~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
	private:
		std::string m_Name;
	};

}
