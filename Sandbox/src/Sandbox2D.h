#pragma once

#include <GitGud.h>

class Sandbox2D : public GitGud::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(GitGud::Timestep ts) override;
	virtual void OnEvent(GitGud::Event& e) override;
	virtual void OnImGuiRender() override;

private:
	// TMP
	GitGud::OrthographicCameraController _cameraController;
	GitGud::Ref<GitGud::Shader> _shader;
	GitGud::Ref<GitGud::VertexArray> _va;

	glm::vec4 _color = { 0.8f, 0.2f, 0.6f, 1.0f };
};