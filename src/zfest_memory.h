#ifndef ZFEST_MEMORY_H
#define ZFEST_MEMORY_H

#define zeroStruct(instance) zeroSize(sizeof(instance), &(instance))
#define zeroArray(array) zeroSize(sizeof(array), (array))
inline void zeroSize(MemSize size, void *ptr)
{
    // TODO(nox): Profile this
    uint8 *byte = (uint8 *)ptr;
    while(size--)
    {
        *byte++ = 0;
    }
}

struct MemoryArena
{
    uint8 *base;
    MemSize size;
    MemSize used;
};

struct TemporaryMemory
{
    MemoryArena *arena;
    MemSize prevUsed;
};

enum ArenaPushFlag
{
    ArenaFlag_ClearToZero = 1<<0,
};

internal MemoryArena initializeArena(MemoryArena *arena, void *base, MemSize size)
{
    MemoryArena result;

    result.base = (uint8 *)base;
    result.size = size;
    result.used = 0;

    return result;
}

#define pushStruct(arena, Type, ...) pushSize_((arena), sizeof(Type), ##__VA_ARGS__)
#define pushArray(arena, count, Type) pushSize_((arena), (count)*sizeof(Type), ##__VA_ARGS__)
internal void *pushSize_(MemoryArena *arena, MemSize size, uint32 flags = ArenaFlag_ClearToZero)
{
    assert(arena->used + size < arena->size);

    void *result = arena->base + arena->used;
    arena->used += size;

    if(flags & ArenaFlag_ClearToZero)
    {
        zeroSize(size, result);
    }

    return result;
}

#endif // ZFEST_MEMORY_H
