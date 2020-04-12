#pragma once

#include "Core.h"
#include "GitGud/Events/Event.h"
#include "GitGud/Core/TimeStep.h"

namespace GitGud
{
	class GG_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAtach() {}
		virtual void OnDeatach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return _debugName; }

	protected:
		std::string _debugName;
	};
}
