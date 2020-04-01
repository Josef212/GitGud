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
