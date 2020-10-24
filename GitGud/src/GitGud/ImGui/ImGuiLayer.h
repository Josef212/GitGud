#pragma once

#include "GitGud/Core/Layer.h"
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

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { _blockEvents = block; }

		void SetDarkTheme();

	private:
		bool _blockEvents = true;
		float _time = 0.0f;
	};
}
