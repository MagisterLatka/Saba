#pragma once

#include "Saba/KeyCodes.h"
#include "Saba/MouseCodes.h"

namespace Saba {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePos();
		static float GetMouseXPos();
		static float GetMouseYPos();
	};

}
