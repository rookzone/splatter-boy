// platform.h
#ifndef PLATFORM_H_
#define PLATFORM_H_

#include <stdint.h>

// Platform identifiers
#define PLATFORM_GB     1
#define PLATFORM_GBA    2

#define PLATFORM PLATFORM_GB

// Include the appropriate platform implementation
#if defined(PLATFORM_GB)
    #include "platform/gb/platform_gb.h"
#elif defined(PLATFORM_GBA)
    #include "platform/gba/platform_gba.h"
#else
    // Default to Game Boy if nothing specified
    #define PLATFORM_GB
    #include "platform/gb/platform_gb.h"
#endif

#endif // PLATFORM_H_