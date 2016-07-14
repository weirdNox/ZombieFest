internal void renderCommands(RenderCommands *commands, uint32 width, uint32 height)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    real32 aspectRatio = 16.0f / 9.0f;
    glOrtho(0, aspectRatio*576.0f, 0, 576, -1, 1);

    if(width > height*aspectRatio)
    {
        uint32 newWidth = (uint32)((real32)height*aspectRatio+0.5f);
        uint32 margin = (width - newWidth) / 2;
        width = newWidth;
        glViewport(margin, 0, width, height);
        glScissor(margin, 0, width, height);
    }
    else
    {
        uint32 newHeight = (uint32)((real32)width/aspectRatio+0.5f);
        uint32 margin = (height - newHeight) / 2;
        height = newHeight;
        glViewport(0, margin, width, height);
        glScissor(0, margin, width, height);
    }

    glDisable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);

    for(uint8 *next = commands->base;
        next < commands->base + commands->size;
        )
    {
        RenderCommandType type = (RenderCommandType)(*next);
        next += sizeof(type);
        void *data = (void *)(next);
        switch(type)
        {
            case RenderType_RenderCommandClear:
            {
                RenderCommandClear *entry = (RenderCommandClear *)data;

                glClearColor(entry->color.r, entry->color.g, entry->color.b, entry->color.a);
                glClear(GL_COLOR_BUFFER_BIT);

                next += sizeof(*entry);
            } break;

            case RenderType_RenderCommandRect:
            {
                RenderCommandRect *entry = (RenderCommandRect *)data;

                glBegin(GL_TRIANGLES);
                glColor4f(entry->color.r, entry->color.g, entry->color.b, entry->color.a);

                glVertex2f(entry->minPos.x, entry->minPos.y);
                glVertex2f(entry->maxPos.x, entry->minPos.y);
                glVertex2f(entry->minPos.x, entry->maxPos.y);

                glVertex2f(entry->maxPos.x, entry->minPos.y);
                glVertex2f(entry->maxPos.x, entry->maxPos.y);
                glVertex2f(entry->minPos.x, entry->maxPos.y);

                glEnd();

                next += sizeof(*entry);
            } break;

            invalidDefaultCase;
        }
    }
}
