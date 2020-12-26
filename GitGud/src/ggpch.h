#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <typeindex>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "GitGud/Core/Log.h"
#include "GitGud/Debug/Instrumentor.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifdef GG_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // GG_PLATFORM_WINDOWS
