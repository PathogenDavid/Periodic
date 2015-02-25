#ifndef __PERIODIC_H__
#define __PERIODIC_H__

//------------------------------------------------------------------------
// Macros
//------------------------------------------------------------------------
#define Assert(x) ASSERT(x)
#define AssertAlways() Assert(false)

#define CountOfArray(a) ( sizeof(a) / sizeof(*a) )

//------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------
#define NULL 0

#define SCREEN_WIDTH 32
#define SCREEN_HEIGHT 32

#define LETTER_SPACING 1
#define LETTER_DESCENDER_HEIGHT 2

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
