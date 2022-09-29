#include "sifteo.h"

#include <Windows.h>
#include <stdio.h>
#include <malloc.h>

namespace Sifteo
{
    typedef void(*LogCallbackT)(const char* message);
    typedef void(*PaintCallbackT)();
    typedef CubeID(*NeighborHoodCubeAtCallbackT)(CubeID relativeTo, Side side);
    typedef void(*ClearScreenCallbackT)(CubeID cube, unsigned int colorIndex);
    typedef void(*DrawPointCallbackT)(CubeID cube, unsigned int x, unsigned int y, unsigned int colorIndex);
    typedef void(*SetPaletteColorCallbackT)(CubeID cube, unsigned int index, uint8_t r, uint8_t g, uint8_t b);

    LogCallbackT LogCallback;
    PaintCallbackT PaintCallback;
    NeighborHoodCubeAtCallbackT NeighborHoodCubeAtCallback;
    ClearScreenCallbackT ClearScreenCallback;
    DrawPointCallbackT DrawPointCallback;
    SetPaletteColorCallbackT SetPaletteColorCallback;

    PeriodicExport void InstallCallbacks
        (
        LogCallbackT logCallback,
        PaintCallbackT paintCallback,
        NeighborHoodCubeAtCallbackT neighborHoodCubeAtCallback,
        ClearScreenCallbackT clearScreenCallback,
        DrawPointCallbackT drawPointCallback,
        SetPaletteColorCallbackT setPaletteColorCallback
        )
    {
        LogCallback = logCallback;
        PaintCallback = paintCallback;
        NeighborHoodCubeAtCallback = neighborHoodCubeAtCallback;
        ClearScreenCallback = clearScreenCallback;
        DrawPointCallback = drawPointCallback;
        SetPaletteColorCallback = setPaletteColorCallback;
    }

    void __do_log(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        
        // Figure out how long the message will be
        int messageLength = _vscprintf(format, args) + 1; // +1 for the null terminating character
        char* message = (char*)_alloca(messageLength * sizeof(char));
        message[messageLength - 1] = '\0'; // Add terminating null character.

        // Format the message
        ASSERT(vsprintf_s(message, messageLength, format, args) != -1);

        // Print out the message
        if (LogCallback != NULL)
        { LogCallback(message); }
        else
        {
            printf(message);
            OutputDebugStringA(message);
        }

        // Cleanup
        _freea(message);
        va_end(args);
    }

    void System::paint()
    {
        ASSERT(PaintCallback != NULL);
        PaintCallback();
    }

    Neighborhood::Neighborhood(CubeID cube)
    {
        relativeTo = cube;
    }

    CubeID Neighborhood::cubeAt(Side side)
    {
        ASSERT(NeighborHoodCubeAtCallback != NULL);
        return NeighborHoodCubeAtCallback(relativeTo, side);
    }

    bool Neighborhood::hasCubeAt(Side side)
    {
        return cubeAt(side) != (CubeID)-1;
    }

    vec::vec(unsigned int x, unsigned int y)
    {
        this->x = x;
        this->y = y;
    }

    void FB32Drawable::__setVideoBuffer(VideoBuffer* videoBuffer)
    {
        this->videoBuffer = videoBuffer;
    }

    void FB32Drawable::fill(unsigned int colorIndex)
    {
        ASSERT(videoBuffer != NULL);
        ASSERT(ClearScreenCallback != NULL);
        ClearScreenCallback(videoBuffer->__getAttachedTo(), colorIndex);
    }

    void FB32Drawable::plot(vec position, unsigned int colorIndex)
    {
        ASSERT(videoBuffer != NULL);
        ASSERT(DrawPointCallback != NULL);
        DrawPointCallback(videoBuffer->__getAttachedTo(), position.x, position.y, colorIndex);
    }

    void ColormapSlot::__init(VideoBuffer* videoBuffer, unsigned int index)
    {
        this->videoBuffer = videoBuffer;
        this->index = index;
    }

    void ColormapSlot::set(uint8_t r, uint8_t g, uint8_t b)
    {
        ASSERT(videoBuffer != NULL);
        ASSERT(SetPaletteColorCallback != NULL);
        SetPaletteColorCallback(videoBuffer->__getAttachedTo(), index, r, g, b);
    }

    VideoBuffer::VideoBuffer()
    {
        fb32.__setVideoBuffer(this);

        for (unsigned int i = 0; i < NUM_COLORS; i++)
        {
            colormap[i].__init(this, i);
        }
    }

    void VideoBuffer::attach(CubeID cube)
    {
        attachedTo = cube;
    }

    void VideoBuffer::initMode(VideoMode mode)
    {
        ASSERT(mode == FB32);
    }

    CubeID VideoBuffer::__getAttachedTo()
    {
        return attachedTo;
    }
}
