internal void renderCommands(RenderCommands *commands, uint32 width, uint32 height)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    real32 aspectRatio = 16.0f / 9.0f;

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

    glOrtho(-(int32)width/2, width/2, -(int32)height/2, height/2, -1, 1);

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

                glPushMatrix();

                glTranslatef(entry->pos.x, entry->pos.y, 0);
                glRotatef(entry->rotationDeg, 0, 0, 1);

                V2 halfDim = entry->dim * 0.5f;
                glBegin(GL_TRIANGLES);
                glColor4f(entry->color.r, entry->color.g, entry->color.b, entry->color.a);

                glVertex2f(-halfDim.x, -halfDim.y);
                glVertex2f(halfDim.x, -halfDim.y);
                glVertex2f(-halfDim.x, halfDim.y);

                glVertex2f(halfDim.x, -halfDim.y);
                glVertex2f(halfDim.x, halfDim.y);
                glVertex2f(-halfDim.x, halfDim.y);

                glEnd();

                glPopMatrix();

                next += sizeof(*entry);
            } break;

            invalidDefaultCase;
        }
    }
}
