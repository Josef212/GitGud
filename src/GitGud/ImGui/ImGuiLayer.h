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

		virtual void OnAtach() override;
		virtual void OnDeatach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float _time = 0.0f;
	};
}
