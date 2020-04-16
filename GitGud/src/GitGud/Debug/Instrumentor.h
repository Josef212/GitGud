#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>

#include "GitGud/Core/Log.h"
#include "GitGud/Core/Core.h"

namespace GitGud
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor() : _currentSession(nullptr) { }
		~Instrumentor() = default;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			std::lock_guard lock(_mutex);
			if (_currentSession)
			{
				if (Log::GetCoreLogger())
				{
					GG_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, _currentSession->Name);
				}

				InternalEndSession();
			}

			_outputStream.open(filepath);

			if (_outputStream.is_open())
			{
				_currentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger())
				{
					GG_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(_mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(_mutex);
			if (_currentSession)
			{
				_outputStream << json.str();
				_outputStream.flush();
			}
		}

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}

	private:
		void WriteHeader()
		{
			_outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			_outputStream.flush();
		}

		void WriteFooter()
		{
			_outputStream << "]}";
			_outputStream.flush();
		}

		void InternalEndSession()
		{
			if (_currentSession)
			{
				WriteFooter();
				_outputStream.close();
				delete _currentSession;
				_currentSession = nullptr;
			}
		}

	private:
		std::mutex _mutex;
		InstrumentationSession* _currentSession;
		std::ofstream _outputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name) : _name(name), _stopped(false)
	{
		_startTimepoint = std::chrono::high_resolution_clock::now();
	}

		  ~InstrumentationTimer()
		  {
			  if (!_stopped)
			  {
				  Stop();
			  }
		  }

		  void Stop()
		  {
			  auto endTimepoint = std::chrono::steady_clock::now();
			  auto highResStart = FloatingPointMicroseconds{ _startTimepoint.time_since_epoch() };
			  auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch();

			  Instrumentor::Get().WriteProfile({ _name, highResStart, elapsedTime, std::this_thread::get_id() });

			  _stopped = true;
		  }

	private:
		const char* _name;
		bool _stopped;
		std::chrono::time_point<std::chrono::steady_clock>_startTimepoint;
	};
}

#if GG_PROFILE
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define GG_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define GG_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define GG_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define GG_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define GG_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define GG_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define GG_FUNC_SIG __func__
#else
#define GG_FUNC_SIG "GG_FUNC_SIG unknown!"
#endif

#define GG_PROFILE_BEGIN_SESSION(name, filepath) ::GitGud::Instrumentor::Get().BeginSession(name, filepath)
#define GG_PROFILE_END_SESSION() ::GitGud::Instrumentor::Get().EndSession()
#define GG_PROFILE_SCOPE(name) ::GitGud::InstrumentationTimer timer##__LINE__(name);
#define GG_PROFILE_FUNCTION() GG_PROFILE_SCOPE(GG_FUNC_SIG)
#else
#define GG_PROFILE_BEGIN_SESSION(name, filepath)
#define GG_PROFILE_END_SESSION()
#define GG_PROFILE_SCOPE(name)
#define GG_PROFILE_FUNCTION()
#endif