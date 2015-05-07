#ifndef __PERIODIC_H__
#define __PERIODIC_H__

//------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------
#define Assert(x) ASSERT(x)
#define AssertAlways() Assert(false)
#define CompilerAssert(x) struct compilerAssertType ## __COUNTER__ { int : !!(x); }

#define CountOfArray(a) ( sizeof(a) / sizeof(*a) )

#ifndef STANDALONE_APP
//! Sifteo's memset uses a non-standard signature, use this one if you want the default memset signature's behavior.
#define PeriodicMemset(destination, value, count) Sifteo::memset8((uint8_t*)destination, (uint8_t)value, count)

#define PeriodicExport
#else
#define PeriodicMemset(destination, value, count) memset(destination, value, count)
#endif

//------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------
#define NULL 0

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32

#define LETTER_SPACING 1
#define LETTER_DESCENDER_HEIGHT 2

#define NUM_CUBES 12 // 12 max
// Note: Each cube takes about 1 KB of memory because of the frame buffer. Setting this too high leaves us with no room to allocate stack.
// Current safe max seems to be 8 cubes

#define MAX_REACTIONS ((NUM_CUBES + 1) / 2 + 1)  // Half of the number of cubes, rounded up, plus an extra for processing cubes with no neighbors
#define MAX_COMPOUNDS (MAX_REACTIONS * 2)

//------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------
#ifndef STANDALONE_APP
typedef unsigned long size_t; // Sifteo doesn't declare size_t for some reason.
#endif

typedef unsigned int uint32;
CompilerAssert(sizeof(uint32) == 4);

//------------------------------------------------------------------------
// Generic Utility Functions
//------------------------------------------------------------------------
int strlen(const char* str);
int strcmp(const char* a, const char* b);
int abs(int x);

#endif
