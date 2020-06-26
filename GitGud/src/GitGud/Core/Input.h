#pragma once

#include "Core.h"

namespace GitGud
{
	class GG_API Input
	{
	public:
		static bool IsKey(int keyCode);
		static bool IsKeyDown(int keyCode);
		static bool IsKeyUp(int keyCode);

		static bool IsMouseButton(int button);
		static bool IsMouseButtonDown(int button);
		static bool IsMouseButtonUp(int button);

		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePos();
	};
}
