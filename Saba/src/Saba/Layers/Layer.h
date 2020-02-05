#pragma once

#include "Saba\Events\Event.h"
#include "Saba\Timestep.h"

namespace Saba {

	class Layer
	{
	public:
		Layer(const char* name);
		~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
	private:
		const char* m_Name;
	};

}
