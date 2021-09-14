/* date = July 29th 2021 2:19 pm */

#ifndef PLATFORM_INCLUDES_H
#define PLATFORM_INCLUDES_H

#include "defines.h"
#include "math.h"
#include "string.h"
#include "memory_arena.h"
#include "platform.h"

#include "math.cpp"
#include "string.cpp"

#if defined(PLATFORM_WIN32)
#include "win32_platform.h"
#include "win32_events.h"
#include "win32_platform.cpp"
#endif

#include "memory_arena.cpp"

#endif //PLATFORM_INCLUDES_H
