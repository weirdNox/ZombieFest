enum RenderCommandType
{
    RenderType_RenderCommandClear,
    RenderType_RenderCommandRect,
};

struct RenderCommandClear
{
    V4 color;
};

struct RenderCommandRect
{
    V2 minPos;
    V2 maxPos;
    V4 color;
};

#define pushRenderCommand(commands, type) (type *)pushRenderCommand_(commands, RenderType_##type, sizeof(type))
internal void *pushRenderCommand_(RenderCommands *commands, RenderCommandType type, MemSize size)
{
    void *result = 0;
    MemSize totalSize = sizeof(RenderCommandType) + size;
    if(commands->size + totalSize <= commands->maxSize)
    {
        *(RenderCommandType *)(commands->base + commands->size) = type;
        result = commands->base + (commands->size + sizeof(RenderCommandType));

        commands->size += totalSize;
    }
    else
    {
        invalidCodePath;
    }

    return result;
}

internal void clear(RenderCommands *commands, V4 color)
{
    RenderCommandClear *entry = pushRenderCommand(commands, RenderCommandClear);
    if(entry)
    {
        entry->color = color;
    }
}

internal void pushRect(RenderCommands *commands, V2 pos, V2 dim, V4 color)
{
    RenderCommandRect *entry = pushRenderCommand(commands, RenderCommandRect);
    if(entry)
    {
        entry->color = color;
        entry->minPos = pos;
        entry->maxPos = pos + dim;
    }
}
