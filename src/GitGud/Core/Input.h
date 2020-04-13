#pragma once

#include "Core.h"

namespace GitGud
{
	class GG_API Input
	{
	public:
		inline static bool IsKey(int keyCode) { return s_instance->IsKeyImpl(keyCode); }
		inline static bool IsKeyDown(int keyCode) { return s_instance->IsKeyImplDown(keyCode); }
		inline static bool IsKeyUp(int keyCode) { return s_instance->IsKeyImplUp(keyCode); }

		inline static bool IsMouseButton(int button) { return s_instance->IsMouseButtonImpl(button); }
		inline static bool IsMouseButtonDown(int button) { return s_instance->IsMouseButtonDownImpl(button); }
		inline static bool IsMouseButtonUp(int button) { return s_instance->IsMouseButtonUpImpl(button); }

		inline static float GetMouseX() { return s_instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_instance->GetMouseYImpl(); }
		inline static std::pair<float, float> GetMousePos() { return s_instance->GetMousePosImpl(); }

		static Scope<Input> Create();

	protected:
		virtual bool IsKeyImpl(int keyCode) = 0;
		virtual bool IsKeyImplDown(int keyCode) = 0;
		virtual bool IsKeyImplUp(int keyCode) = 0;

		virtual bool IsMouseButtonImpl(int button) = 0;
		virtual bool IsMouseButtonDownImpl(int button) = 0;
		virtual bool IsMouseButtonUpImpl(int button) = 0;

		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;

	private:
		static Scope<Input> s_instance;
	};
}
