#ifndef ZFEST_H
#define ZFEST_H

enum EntityType
{
    EntityType_Player,
    EntityType_Zombie,
};

struct Entity
{
    EntityType type;
    V2 pos;
    V2 vel;
    int32 life;
};

struct GameState
{
    bool32 isInitialized;

    uint32 entityCount;
    Entity entities[100];

    uint32 playerIndex;

    V2 cameraPos;

    real32 metersToPixels;
};

#endif // ZFEST_H
