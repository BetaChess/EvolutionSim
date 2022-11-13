#ifndef PCH_H
#define PCH_H

#ifdef CXXGT20
#define EVOLIKELY [[likely]]
#define EVOUNLIKELY [[unlikely]]
#else
#define EVOLIKELY
#define EVOUNLIKELY
#endif

#include "logger.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>


#endif
