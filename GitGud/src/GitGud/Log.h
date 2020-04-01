#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace GitGud
{
	class GG_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#define GG_CORE_TRACE(...)     ::GitGud::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GG_CORE_INFO(...)      ::GitGud::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GG_CORE_WARN(...)      ::GitGud::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GG_CORE_ERROR(...)     ::GitGud::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GG_CORE_FATAL(...)     ::GitGud::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define GG_TRACE(...)          ::GitGud::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GG_INFO(...)           ::GitGud::Log::GetClientLogger()->info(__VA_ARGS__)
#define GG_WARN(...)           ::GitGud::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GG_ERROR(...)          ::GitGud::Log::GetClientLogger()->error(__VA_ARGS__)
#define GG_FATAL(...)          ::GitGud::Log::GetClientLogger()->fatal(__VA_ARGS__)
