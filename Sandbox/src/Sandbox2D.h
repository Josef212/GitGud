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
	GitGud::OrthographicCameraController _cameraController;
	GitGud::Ref<GitGud::Texture2D> _checkerTexture, _logoTexture;
	GitGud::Ref<GitGud::Texture2D> _spriteSheet;
	GitGud::Ref<GitGud::SubTexture2D> _stairsSprite, _barrelSprite, _orangeTree;

	glm::vec4 _color = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec2 _pos = { 0.0f, 0.0f };
	glm::vec2 _size = { 1.0f, 1.0f };
	float _angle = 0.0f;
};