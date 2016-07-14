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

    uint8 keyStates[256];
    uint8 keyDeltas[256];

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

#endif // ZFEST_PLATFORM_H
