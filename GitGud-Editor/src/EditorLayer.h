#pragma once

#include <GitGud.h>

namespace GitGud
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

	private:
		OrthographicCameraController _cameraController;
		Ref<Texture2D> _checkerTexture, _logoTexture;
		Ref<Texture2D> _spriteSheet;
		Ref<SubTexture2D> _stairsSprite, _barrelSprite, _orangeTree;

		Ref<Framebuffer> _frambuffer;

		glm::vec4 _color = { 0.8f, 0.2f, 0.3f, 1.0f };
		glm::vec2 _pos = { 0.0f, 0.0f };
		glm::vec2 _size = { 1.0f, 1.0f };
		float _angle = 0.0f;
	};
}