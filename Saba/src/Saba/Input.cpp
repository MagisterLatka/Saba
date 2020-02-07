#include "pch.h"
#include "Input.h"

#include "Platform\GLFW\InputGLFW.h"

namespace Saba {

#if defined(SB_USE_GLFW)
	Scope<Input> Input::s_Instance = MakeScope<InputGLFW>();
#else
	Scope<Input> Input::s_Instance = nullptr;
#endif

}
