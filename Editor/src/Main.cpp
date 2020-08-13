#include <pch.h>
#include <Saba.h>
#include <Saba\EntryPoint.h>

#include "EditorLayer.h"

class Editor : public Saba::Application
{
public:
	Editor()
		:Application("Editor")
	{
		PushLayer(new Saba::EditorLayer);
	}

	~Editor()
	{

	}
};
Saba::Application* Saba::CreateApplication()
{
	return new Editor();
}
