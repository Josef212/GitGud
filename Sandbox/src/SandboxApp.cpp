#include <GitGud.h>

class ExampleLayer : public GitGud::Layer
{
public:
	ExampleLayer() : Layer("Example") {}

	void OnUpdate() override
	{
		GG_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(GitGud::Event& event) override
	{
		GG_TRACE("{0}", event);
	}
};

class SandboxApp : public GitGud::Application
{
public:
	SandboxApp() 
	{
		PushLayer(new ExampleLayer());
	}

	~SandboxApp() {}
};

GitGud::Application* GitGud::CreateApplication()
{
	return new SandboxApp();
}
