#pragma once

#ifdef GG_PLATFORM_WINDOWS
	#ifdef GG_BUILD_DLL
		#define GG_API __declspec(dllexport)
	#else 
		#define GG_API __declspec(dllimport)
	#endif
#else
	#error Only Windows supported
#endif // GG_PLATFORM_WINDOWS

#ifdef GG_ENABLE_ASSERTS
	#define GG_ASSERT(x, ...) { if(!(x)) {GG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GG_CORE_ASSERT(x, ...) { if(!(x)) {GG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GG_ASSERT(x, ...)
	#define GG_CORE_ASSERT(x, ...)
#endif

#define GG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

typedef unsigned int uint;