#ifndef ZFEST_H
#define ZFEST_H

enum EntityType
{
    EntityType_Player,
    EntityType_Zombie,
    EntityType_Bullet,
};

struct Entity
{
    EntityType type;
    V2 pos;
    V2 vel;
    real32 rotationDeg; // NOTE(nox): 0º means pointing up
    int32 life;
};

struct GameState
{
    bool32 isInitialized;

    uint32 entityCount;
    Entity entities[256];

    uint32 playerIndex;

    V2 cameraPos;

    real32 metersToPixels;
};

#endif // ZFEST_H
