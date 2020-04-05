#pragma once

#include "GitGud/Layer.h"
#include "GitGud/Events/KeyEvent.h"
#include "GitGud/Events/MouseEvent.h"
#include "GitGud/Events/ApplicationEvent.h"

namespace GitGud
{
	class GG_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAtach() override;
		void OnDeatach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleaseEvent& e);
		bool OnMouseMovedEvent(MouseMoveEvent& e);
		bool OnMouseScrolledEvent(MouseScrollEvent& e);
		bool OnKeyPressedEvent(KeyPressEvent& e);
		bool OnKeyReleasedEvent(KeyReleaseEvent& e);
		bool OnKeyTypedEvent(KeyTypeEvent& e);
		bool OnWindowResizedEvent(WindowResizeEvent& e);

	private:
		float _time = 0.0f;
	};
}
