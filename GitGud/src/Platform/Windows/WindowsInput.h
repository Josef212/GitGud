#pragma once

#include "GitGud/Core/Input.h"

namespace GitGud
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyImpl(int keyCode) override;
		virtual bool IsKeyImplDown(int keyCode) override;
		virtual bool IsKeyImplUp(int keyCode) override;

		virtual bool IsMouseButtonImpl(int button) override;
		virtual bool IsMouseButtonDownImpl(int button) override;
		virtual bool IsMouseButtonUpImpl(int button) override;

		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual std::pair<float, float> GetMousePosImpl() override;
	};
}
