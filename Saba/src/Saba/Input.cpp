#include "pch.h"
#include "Input.h"

#include "Platform\GLFW\InputGLFW.h"

namespace Saba {

#if defined(SB_USE_GLFW)
	Input* Input::s_Instance = new InputGLFW;
#endif

}
