#pragma once

#include "Saba\Input.h"

namespace Saba {

	class InputGLFW : public Input
	{
	public:
		bool IsKeyPressedImpl(int key) override;
		bool IsMouseButtonPressedImpl(int button) override;
		std::pair<float, float> GetMousePosImpl() override;
		float GetMouseXPosImpl() override;
		float GetMouseYPosImpl() override;
	};

}
