#ifndef GAMEENGINE_WORLD_H
#define GAMEENGINE_WORLD_H

#include "types.h"

geWorld worldMain;
geWorld worldsSecondary[2];

typedef enum {
    GE_ALGORITHM_BASIC,
    GE_ALGORITHM_CULLED,
    GE_ALGORITHM_GREEDY
} ALGORITHM;

geWorld geWorldInit(ALGORITHM algorithm, size_t sizeX, size_t sizeY, size_t sizeZ);
void geWorldGenerate(geWorld* world, size_t baseHeight, size_t heightOffsetIntesnsity);
void geWorldDestroy(geWorld* world);
kmVec3 geWorldFind(geWorld* world, kmVec3* v);
void geWorldRemoveBlock(geWorld* world, kmVec3* v);
void geWorldCopyPlanes(geWorld* world);
void geWorldCopyPlanesWithOrientation(geWorld* world, uint8_t orientation);
geWorld geWorldInitCustom(ALGORITHM algorithm, uint8_t orientation);

#endif //GAMEENGINE_WORLD_H
