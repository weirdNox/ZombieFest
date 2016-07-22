#include <cstdlib>

internal Entity *addEntity(GameState *gameState, EntityType type)
{
    Entity *result = gameState->entities + gameState->entityCount++;
    assert(gameState->entityCount <= arrayCount(gameState->entities));

    result->type = type;

    return result;
}

internal Entity *addBullet(GameState *gameState, V2 playerPos, V2 direction, real32 speed)
{
    Entity *bullet = addEntity(gameState, EntityType_Bullet);

    V2 normalizedDirection = normalize(direction);
    bullet->pos = playerPos + normalizedDirection*0.1f;
    bullet->vel = normalizedDirection*speed;
    bullet->rotationDeg = aTan2(direction.y, direction.x)*180.0f/PI32 - 90;

    return bullet;
}

internal void gameUpdateAndRender(Core *core)
{
    GameState *gameState = (GameState *)core->persistentMemory;
    if(!gameState->isInitialized)
    {
        gameState->playerIndex = gameState->entityCount;
        addEntity(gameState, EntityType_Player);

        // TODO(nox): This random generator is only for testing!
        srand(10912038);
        for(uint32 zombieIndex = 0;
            zombieIndex < 100;
            ++zombieIndex)
        {
            Entity *zombie = addEntity(gameState, EntityType_Zombie);
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

                V2 cameraToPointer = core->mousePos - v2i(core->width, core->height)*0.5f;
                cameraToPointer *= 1.0f/gameState->metersToPixels;
                V2 cameraToPlayer = entity->pos - gameState->cameraPos;
                V2 direction = cameraToPointer - cameraToPlayer;
                entity->rotationDeg = aTan2(direction.y, direction.x)*180.0f/PI32 - 90;

                if(core->keyStates[LEFT_MOUSE_BUTTON] && core->keyDeltas[LEFT_MOUSE_BUTTON] &&
                   (direction.x || direction.y))
                {
                    addBullet(gameState, entity->pos, direction, 70.0f);
                }
            } break;

            case EntityType_Zombie:
            {
                V2 direction = gameState->entities[gameState->playerIndex].pos - entity->pos;
                entity->rotationDeg = aTan2(direction.y, direction.x)*180.0f/PI32 - 90;
                accelDirection = direction;
                accelValue = 4.0f;
            } break;

            case EntityType_Bullet:
            {
                // TODO(nox): REMOVE THEM!
                drag = 1.0f;
            } break;
        }

        V2 accel = normalizeSafe(accelDirection)*accelValue - drag*entity->vel;
        entity->vel += accel*core->frameTime;
        entity->pos += entity->vel*core->frameTime;

        switch(entity->type)
        {
            case EntityType_Player:
            {
                // TODO(nox): Need to implement something like EntityTransform
                // + Offsets for rendering so that we are able to add things
                // (like a gun) to our player
                pushRect(&core->renderCommands, gameState, entity->pos,
                         v2(0.5f, 0.25f), v4(1, 1, 1, 1), entity->rotationDeg);
            } break;

            case EntityType_Zombie:
            {
                pushRect(&core->renderCommands, gameState, entity->pos,
                         v2(0.5f, 0.25f), v4(0, 1, 0, 1), entity->rotationDeg);
            } break;

            case EntityType_Bullet:
            {
                pushRect(&core->renderCommands, gameState, entity->pos,
                         v2(0.1f, 0.1f), v4(1, 0, 0, 1), entity->rotationDeg);
            } break;
        }

    }
}
