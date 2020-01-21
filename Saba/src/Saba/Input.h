#pragma once

namespace Saba {

	class Input
	{
	public:
		static bool IsKeyPressed(int key);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePos();
		static float GetMouseXPos();
		static float GetMouseXYos();
	};

}
