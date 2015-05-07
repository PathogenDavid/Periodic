// This file contains compatibility glue to make the Sifteo-specific code build on non-Sifteo platforms.
// It should be replaced with a more formalized compatibility layer if both code bases are to be separately maintained in the future.
#ifndef __STANDALONE_APP_SIFTEO_H__
#define __STANDALONE_APP_SIFTEO_H__

#ifndef STANDALONE_APP
#error "This header should only be used in the standalone app!"
#endif

#include <assert.h>
#include <memory.h> // For memset

#ifndef NULL
#define __STANDALONE_APP_SIFTEO_H__DEFINED_NULL
#define NULL 0
#endif

namespace Sifteo
{
    typedef void(*LogCallbackT)(const char* format, ...);
    extern LogCallbackT LogCallback;

    typedef unsigned char uint8_t;

    typedef uint8_t CubeID;
    #define INVALID_CUBE_ID ((CubeID)-1)

    enum Side
    {
        TOP = 0,
        LEFT,
        BOTTOM,
        RIGHT,
        NUM_SIDES,
        NO_SIDE = -1
    };

    enum VideoMode
    {
        FB32
    };

    class System
    {
    public:
        static void paint();
    };

    class Neighborhood
    {
    private:
        CubeID relativeTo;
    public:
        Neighborhood(CubeID cube);
        CubeID cubeAt(Side side);
        bool hasCubeAt(Side side);
    };

    struct vec
    {
        unsigned int x;
        unsigned int y;

        vec(unsigned int x, unsigned int y);
    };

    struct VideoBuffer;

    struct FB32Drawable
    {
    private:
        VideoBuffer* videoBuffer = NULL;
    public:
        void __setVideoBuffer(VideoBuffer* videoBuffer);
        void fill(unsigned int colorIndex);
        void plot(vec position, unsigned int colorIndex);
    };

    struct ColormapSlot
    {
    private:
        VideoBuffer* videoBuffer = NULL;
        unsigned int index = -1;
    public:
        void __init(VideoBuffer* videoBuffer, unsigned int index);
        void set(uint8_t r, uint8_t g, uint8_t b);
    };

    struct VideoBuffer
    {
    private:
        CubeID attachedTo = INVALID_CUBE_ID;
        static const unsigned NUM_COLORS = 16;
    public:
        VideoBuffer();
        void attach(CubeID cube);
        void initMode(VideoMode mode);
        CubeID __getAttachedTo();

        FB32Drawable fb32;
        ColormapSlot colormap[NUM_COLORS];
    };
}

#define ASSERT(x) assert(x)

#define LOG(...) Sifteo::LogCallback(__VA_ARGS__);

#define PeriodicExport extern "C" __declspec(dllexport)

#ifdef __STANDALONE_APP_SIFTEO_H__DEFINED_NULL
#undef NULL
#undef __STANDALONE_APP_SIFTEO_H__DEFINED_NULL
#endif

#endif
