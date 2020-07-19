#pragma once

class ExampleLayer : public Saba::Layer
{
public:
	ExampleLayer();
	~ExampleLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(Saba::Event& event) override;
	virtual void OnUpdate(Saba::Timestep ts) override;
	virtual void OnImGuiRender() override;
private:
	bool OnKeyPress(Saba::KeyPressedEvent& e);
private:
	Saba::PerspectiveCameraControler m_CameraControler;

	Saba::Scene3D m_Scene;
	Saba::Ref<Saba::Framebuffer> m_SceneFramebuffer;
	Saba::Ref<Saba::Texture2D> m_SceneFramebufferTexture;
	Saba::Ref<Saba::Renderbuffer> m_SceneFramebufferRenderbuffer;
	float m_Exposure = 1.0f;

	static constexpr int m_MaxDirLights = 4;
	Saba::Ref<Saba::Framebuffer> m_DirShadowFramebuffer;
	Saba::Ref<Saba::Texture2D> m_DirShadowTexture;

	static constexpr int m_MaxPointLights = 4;
	Saba::Ref<Saba::Framebuffer> m_PointShadowFramebuffer;
	Saba::Ref<Saba::Texture2D> m_PointShadowTexture;

	static constexpr int m_MaxSpotLights = 4;
	Saba::Ref<Saba::Framebuffer> m_SpotShadowFramebuffer;
	Saba::Ref<Saba::Texture2D> m_SpotShadowTexture;
};
