#ifndef ZFEST_PLATFORM_H
#define ZFEST_PLATFORM_H

struct RenderCommands
{
    uint8 *base;
    MemSize maxSize;
    MemSize size;
};

struct Core
{
    bool32 running;

    // TODO(nox): This is _not_ cross platform! This is using windows virtual
    // keycodes as indices, and would not work on other platforms. We need to have
    // some translation method.
    uint8 keyStates[256];
    uint8 keyDeltas[256];

    // NOTE(nox): Relative to the bottom left corner of the window
    V2 mousePos;

    MemSize persistentMemorySize;
    void *persistentMemory;

    MemSize transientMemorySize;
    void *transientMemory;

    uint32 width;
    uint32 height;
    RenderCommands renderCommands;

    real32 time;
    real32 frameTime;
};

#define LEFT_MOUSE_BUTTON 0x01

#endif // ZFEST_PLATFORM_H
