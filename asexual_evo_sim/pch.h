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


#endif
