//
// Created by Afterwind on 9/23/2017.
//

#ifndef GAMEENGINE_TYPES_H
#define GAMEENGINE_TYPES_H

#include <GL/glew.h>
#include <kazmath/kazmath.h>
#include <stdbool.h>

typedef struct geVertex {
    kmVec3 pos;
    kmVec3 normal;
    kmVec3 texCoords;
} geVertex;

typedef struct geShape {
    geVertex* vertices;
    GLuint* indices;

    unsigned long long numVertices;
    unsigned long long numIndices;

    unsigned long long offsetBytesVertex;
    unsigned long long offsetBytesIndex;

    GLuint vao;
} geShape;

typedef struct geObject {
    GLuint texture;
    geShape* shape;

    kmVec3 pos;
    kmVec3 size;
    kmVec3 rotation;

    bool exemptFromView;
    bool exemptFromViewTranslation;
    bool exemptFromViewProjection;

    GLushort glTextureId;
    GLushort glTextureType;
    GLfloat extraBrightness;
} geObject;

typedef struct geCamera {
    kmVec3 pos;
    kmVec3 direction;
    kmVec3 up;

    float aspectRatio;
    kmVec3 rotation;
} geCamera;

typedef struct gePlane {
    geVertex* vertices;
    unsigned long long numVertices;
} gePlane;

typedef struct geWorld {
    long long ***map;
    size_t sizeX, sizeY, sizeZ;
    size_t numBlocks;
    unsigned char algorithm;

    // For rendering
    geObject* object;
    geShape shape;

    gePlane* planesUncompressed[6];
    gePlane* planes[6];
    size_t numPlanes[6];
} geWorld;

#endif //GAMEENGINE_TYPES_H
