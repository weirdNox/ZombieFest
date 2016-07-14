struct GameState
{
    bool32 isInitialized;
    V2 playerPos;
};

internal void gameUpdateAndRender(Core *core)
{
    GameState *gameState = (GameState *)core->persistentMemory;
    if(!gameState->isInitialized)
    {
        gameState->playerPos = v2(100, 100);

        gameState->isInitialized = true;
    }

    if(core->keyStates['W'])
    {
        gameState->playerPos += v2(0, 10);
    }
    if(core->keyStates['S'])
    {
        gameState->playerPos += v2(0, -10);
    }
    if(core->keyStates['A'])
    {
        gameState->playerPos += v2(-10, 0);
    }
    if(core->keyStates['D'])
    {
        gameState->playerPos += v2(10, 0);
    }

    clear(&core->renderCommands, v4(1, 0, 0, 1));
    pushRect(&core->renderCommands, gameState->playerPos, v2(50, 50), v4(1, 1, 1, 1));
}
