#ifndef __PERIODIC_H__
#define __PERIODIC_H__

//------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------
#define Assert(x) ASSERT(x)
#define AssertAlways() Assert(false)

#define CountOfArray(a) ( sizeof(a) / sizeof(*a) )

#define PeriodicMemset(destination, value, count) Sifteo::memset8((uint8_t*)destination, (uint8_t)value, count)

//------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------
#define NULL 0

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32

#define LETTER_SPACING 1
#define LETTER_DESCENDER_HEIGHT 2

#define NUM_CUBES 12 // 12 max

#define MAX_REACTIONS ((NUM_CUBES + 1) / 2 + 1)  // Half of the number of cubes, rounded up, plus an extra for processing cubes with no neighbors
#define MAX_COMPOUNDS 5

//------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------
typedef unsigned long size_t; // Sifteo doesn't declare size_t for some reason.

//------------------------------------------------------------------------
// Generic Utility Functions
//------------------------------------------------------------------------
int strlen(const char* str);
int strcmp(const char* a, const char* b);
int abs(int x);

#endif
