#pragma once

#include "Saba\Events\Event.h"

namespace Saba {

	class Layer
	{
	public:
		Layer(const char* name);
		~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnEvent(Event& event) {}
		virtual void OnUpdate() {}
	private:
		const char* m_Name;
	};

}
