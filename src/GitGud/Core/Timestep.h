#pragma once

namespace GitGud
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : _time(time)
		{
		}

		float GetSeconds() const { return _time; }
		float GetMilliseconds() const { return _time * 1000.0f; }

		operator float() const { return _time; }

	private:
		float _time;
	};
}