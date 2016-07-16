#include <cstdlib>

internal Entity *addEntity(GameState *gameState)
{
    Entity *result = gameState->entities + gameState->entityCount++;

    return result;
}

internal void gameUpdateAndRender(Core *core)
{
    GameState *gameState = (GameState *)core->persistentMemory;
    if(!gameState->isInitialized)
    {
        gameState->playerIndex = gameState->entityCount;
        Entity *player = addEntity(gameState);
        player->type = EntityType_Player;

        // TODO(nox): This random generator is only for testing!
        srand(10912038);
        for(uint32 zombieIndex = 0;
            zombieIndex < 10;
            ++zombieIndex)
        {
            Entity *zombie = addEntity(gameState);
            zombie->type = EntityType_Zombie;
            zombie->pos = v2(rand() % 24 - 12.0f, rand() % 13 - 6.5f);
        }

        gameState->cameraPos = v2(0, 0);

        gameState->metersToPixels = 40;

        gameState->isInitialized = true;
    }



    clear(&core->renderCommands, v4(0, 0, 0, 1));

    for(uint32 entityIndex = 0;
        entityIndex < gameState->entityCount;
        ++entityIndex)
    {
        Entity *entity = gameState->entities + entityIndex;
        V2 accelDirection = {};
        real32 accelValue = 0.0f;
        real32 drag = 10.0f;

        switch(entity->type)
        {
            case EntityType_Player:
            {
                accelValue = 50.0f;
                if(core->keyStates['W'])
                {
                    accelDirection += v2(0, 1);
                }
                if(core->keyStates['S'])
                {
                    accelDirection -= v2(0, 1);
                }
                if(core->keyStates['A'])
                {
                    accelDirection -= v2(1, 0);
                }
                if(core->keyStates['D'])
                {
                    accelDirection += v2(1, 0);
                }
            } break;
        }

        V2 accel = normalizeSafe(accelDirection)*accelValue - drag*entity->vel;
        entity->vel += accel*core->frameTime;
        entity->pos += entity->vel*core->frameTime;

        switch(entity->type)
        {
            case EntityType_Player:
            {
                V2 cameraToPointer = core->mousePos - v2i(core->width, core->height)*0.5f;
                cameraToPointer *= 1.0f/gameState->metersToPixels;
                V2 cameraToPlayer = entity->pos - gameState->cameraPos;
                V2 direction = cameraToPointer - cameraToPlayer;
                real32 rotationDeg = aTan2(direction.y, direction.x)*180.0f/PI32 - 90;

                // TODO(nox): Need to implement something like EntityTransform
                // + Offsets for rendering so that we are able to add things
                // (like a gun) to our player
                pushRect(&core->renderCommands, gameState, entity->pos,
                         v2(0.5f, 0.25f), v4(1, 1, 1, 1), rotationDeg);
            } break;

            case EntityType_Zombie:
            {
                V2 direction = gameState->entities[gameState->playerIndex].pos - entity->pos;
                real32 rotationDeg = aTan2(direction.y, direction.x)*180.0f/PI32 - 90;
                pushRect(&core->renderCommands, gameState, entity->pos,
                         v2(0.5f, 0.25f), v4(0, 1, 0, 1), rotationDeg);
            } break;
        }

    }
}
