#pragma once

namespace Saba {

	class Input
	{
	public:
		static bool IsKeyPressed(int key) { return s_Instance->IsKeyPressedImpl(key); }
		static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImpl(); }
		static float GetMouseXPos() { return s_Instance->GetMouseXPosImpl(); }
		static float GetMouseYPos() { return s_Instance->GetMouseYPosImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int key) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		virtual float GetMouseXPosImpl() = 0;
		virtual float GetMouseYPosImpl() = 0;
	private:
		static Input* s_Instance;
	};

}
