//
// Created by Afterwind on 9/19/2017.
//

#include <GL/glew.h>
#include "draw.h"
#include "utils.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "world.h"
#include <math.h>
#include <string.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

geObject* sun;
geObject* sky;
geObject* shadowMap;
geObject* vertexWorldDumb;
geObject* vertexWorldCulled;
geObject* vertexWorldGreedy;
geObject* crosshair;
geObject* highlight;

/* INTERNAL FUNCTIONS */

geObject* geObjectInit() {
    geObject* obj = &objects[numObjects++];
    obj->texture = 0;
    obj->shape = 0;
    obj->pos.x = obj->pos.y = obj->pos.z = 0;
    obj->size.x = obj->size.y = obj->size.z = 1;
    obj->rotation.x = obj->rotation.y = obj->rotation.z = 0;
    obj->exemptFromView = false;
    obj->exemptFromViewTranslation = false;
    obj->exemptFromViewProjection = false;
    obj->glTextureId = GL_TEXTURE0;
    obj->glTextureType = GL_TEXTURE_2D;
    obj->extraBrightness = 0.0f;
    return obj;
}

void initObjects() {
    // Initialize all objects
    memset(objects, 0, sizeof(objects));

    sun = geObjectInit();
    sun->shape = shapes + GE_CUBE;
    sun->texture = tex[GE_TEXTURE_YELLOW];
    sun->size.x = sun->size.y = sun->size.z = 30;
    sun->pos.y = 200;
    sun->exemptFromViewTranslation = true;
    sun->extraBrightness = 1;
//    objects[712].shape = shapes + GE_CUBE;
//    objects[712].texture = tex[4];
//    objects[712].size.x = objects[712].size.y = objects[712].size.z = 30;
//    objects[712].pos = lightPoint;

    sky = geObjectInit();
    sky->shape = shapes + GE_CUBE_INVERTED;
    sky->texture = tex[GE_TEXTURE_SKY];
    sky->size.x = sky->size.y = sky->size.z = 500;
    sky->pos.y = -10;
    sky->glTextureType = GL_TEXTURE_CUBE_MAP;
    sky->glTextureId = GL_TEXTURE1;
    sky->exemptFromViewTranslation = true;
    sky->extraBrightness = 1;

//    shadowMap = geObjectInit();
//    shadowMap->shape = shapes + GE_SQUARE;
//    shadowMap->texture = tex[GE_TEXTURE_SHADOW_MAP_512];
//    shadowMap->size.x = shadowMap->size.y = 4;
//    shadowMap->pos.y = 2;
//    shadowMap->pos.x = -2;
//    shadowMap->pos.z = 5;

    highlight = geObjectInit();
    highlight->shape = shapes + GE_CUBE;
    highlight->size.x = highlight->size.y = highlight->size.z = 1.01f;
    highlight->texture = tex[GE_TEXTURE_GRAY_TRANSPARENT];
    highlight->extraBrightness = 1.0f;

//    vertexWorldDumb = geObjectInit();
//    vertexWorldDumb->pos.x = -200;
//    vertexWorldDumb->pos.y = 0;
//    vertexWorldDumb->pos.z = 200;
//    vertexWorldDumb->rotation.y = 60;
//    vertexWorldDumb->texture = tex[12];
//    vertexWorldDumb->shape = shapes + GE_VERTEX_WORLD_DUMB;

//    vertexWorldGreedy = geObjectInit();
//    vertexWorldGreedy->pos.x = -200;
//    vertexWorldGreedy->pos.y = 0;
//    vertexWorldGreedy->pos.z = 0;
//    vertexWorldGreedy->rotation.y = 60;
//    vertexWorldGreedy->texture = tex[12];
//    vertexWorldGreedy->shape = shapes + GE_VERTEX_WORLD_CULLED;

//    geObject* terrainNoise = geObjectInit();
//    terrainNoise->pos.y = 50;
//    terrainNoise->shape = shapes + GE_TERRAIN_NOISE;
//    terrainNoise->texture = tex[2];
//    terrainNoise->size.x = terrainNoise->size.y = terrainNoise->size.z = 100;


    crosshair = geObjectInit();
    crosshair->texture = tex[GE_TEXTURE_WHITE];
    crosshair->shape = shapes + GE_2D_CROSSHAIR;
    crosshair->exemptFromView = true;
    crosshair->extraBrightness = 1.0f;
    crosshair->size.x = crosshair->size.y = crosshair->size.z = 0.02f;
    crosshair->pos.z = -1;

    worldsSecondary[0] = geWorldInit(GE_ALGORITHM_GREEDY, 100, 32, 100);
    geShapeBuffer(&worldsSecondary[0].shape);
    worldsSecondary[0].object = geObjectInit();
    worldsSecondary[0].object->shape = &worldsSecondary[0].shape;
    worldsSecondary[0].object->texture = tex[GE_TEXTURE_ATLAS];
    worldsSecondary[0].object->glTextureType = GL_TEXTURE_2D_ARRAY;
    worldsSecondary[0].object->glTextureId = GL_TEXTURE2;
    worldsSecondary[0].object->pos.x = 60;
    worldsSecondary[0].object->pos.z = -60;

    worldsSecondary[1] = geWorldInit(GE_ALGORITHM_GREEDY, 50, 32, 50);
    geShapeBuffer(&worldsSecondary[1].shape);
    worldsSecondary[1].object = geObjectInit();
    worldsSecondary[1].object->shape = &worldsSecondary[1].shape;
    worldsSecondary[1].object->texture = tex[GE_TEXTURE_ATLAS];
    worldsSecondary[1].object->glTextureType = GL_TEXTURE_2D_ARRAY;
    worldsSecondary[1].object->glTextureId = GL_TEXTURE2;
    worldsSecondary[1].object->pos.z = 60;

    worldMain = geWorldInit(GE_ALGORITHM_GREEDY, 10, 32, 10);
    geShapeBuffer(&worldMain.shape);
    worldMain.object = geObjectInit();
    worldMain.object->shape = &worldMain.shape;
    worldMain.object->texture = tex[GE_TEXTURE_ATLAS];
    worldMain.object->glTextureType = GL_TEXTURE_2D_ARRAY;
    worldMain.object->glTextureId = GL_TEXTURE2;

//    linePointer = geObjectInit();
//    linePointer->shape = shapes + GE_LINE;
//    linePointer->size.x = linePointer->size.y = linePointer->size.z = 20;
//    linePointer->extraBrightness = 1.0f;
//    linePointer->texture = tex[GE_TEXTURE_RED];

    // <editor-fold> UNUSED USEFUL OBJECTS
//    for (i = 512; i < 612; i++) {
//        objects[i].shape = shapes + GE_LINE;
//        objects[i].texture = tex[1];
//        objects[i].size.x = objects[i].size.y = objects[i].size.z = 100;
//        objects[i].pos.z = i - 562;
//    }
//
//    for (i = 612; i < 712; i++) {
//        objects[i].shape = shapes + GE_LINE;
//        objects[i].texture = tex[1];
//        objects[i].size.x = objects[i].size.y = objects[i].size.z = 100;
//        objects[i].pos.x = i - 662;
//        objects[i].rotation.y = 90;
//    }
//    for (i = 718; i <= 720; i++) {
//        objects[i].shape = shapes + GE_SQUARE;
//        objects[i].texture = tex[8];
//        objects[i].size.x = 0.5f;
//        objects[i].size.y = 0.2f;
//        objects[i].size.z = 1;
//        objects[i].pos.x = 0;
//        objects[i].pos.y = 0.3f * (i - 718);
//        objects[i].pos.z = 0;
//        objects[i].rotation.y = 180;
//    }
//    srand(0);
//    for (i = 722; i < 772; i++) {
//        objects[i].shape = shapes + GE_LINE;
//        objects[i].texture = tex[1];
//        objects[i].size.x = 2;
//        objects[i].size.y = 2;
//        objects[i].size.z = 2;
//
//        float x1 = ((float)rand()/(float)(RAND_MAX)), x2 = ((float)rand()/(float)(RAND_MAX));
//        float v = 1 - fabsf(4 * x1 - 2);
//        float sinT = copysignf(0.5f * (v - sqrtf(2 - v * v)), x1 - 0.5f);
//        float r = fminf(sinT, 0) + x2 * (sqrtf(1 - sinT * sinT) + fabsf(sinT));
//
//        float a = (3 * PI / 2) - asinf(sinT);
//        float sinA = sinf(a);
//        float cosA = cosf(a);
//
//        objects[i].rotation.y = 180.0f * asinf(sinT) / PI;
//        objects[i].pos.x = 0;
//        objects[i].pos.y = 0; //(i - 722) / 50.0f;
//        objects[i].pos.z = r * (cosA * cosA - r * sinA * cosA) / sinA;
//    }

//    objects[772].pos.x = 0;
//    objects[772].pos.y = -0.2f;
//    objects[772].pos.z = 0;
//    objects[772].size.x = 1;
//    objects[772].size.y = 1;
//    objects[772].size.z = 1;
//    objects[772].texture = tex[4];
//    objects[772].shape = shapes + GE_SQUARE;
//    objects[772].rotation.x = 90;
    // </editor-fold>
}

/* EXTERNAL FUNCTIONS */

void geShapeBuffer(geShape* shape) {
    glUseProgram(programs[GE_PROGRAM_MAIN]);

    // If shape already exists just update it
    if (shape->offsetBytesVertex != 0 && shape->offsetBytesIndex != 0) {
        // Buffer shape
        glBindVertexArray(shape->vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferSubData(GL_ARRAY_BUFFER, shape->offsetBytesVertex, sizeof(geVertex) * shape->numVertices, shape->vertices);
        if (shape->numIndices != 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, shape->offsetBytesIndex, sizeof(GLuint) * shape->numIndices, shape->indices);
        }
        return;
    }

    // Initialize shape
    glGenVertexArrays(1, &shape->vao);

    shape->offsetBytesVertex = currentOffsetVertex;
    shape->offsetBytesIndex = currentOffsetIndex;

    currentOffsetVertex += (shape->numVertices + 290000) * sizeof(geVertex);
    currentOffsetIndex += (shape->numIndices + 290000) * sizeof(GLuint);

    // Buffer shape
    glBindVertexArray(shape->vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferSubData(GL_ARRAY_BUFFER, shape->offsetBytesVertex, sizeof(geVertex) * shape->numVertices, shape->vertices);
    if (shape->numIndices != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, shape->offsetBytesIndex, sizeof(GLuint) * shape->numIndices, shape->indices);
    }

    glBindVertexArray(shape->vao);
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(geVertex), (const void*) shape->offsetBytesVertex);
    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(geVertex), (const void*) (shape->offsetBytesVertex + (sizeof(kmVec3))));
    // Texture coords attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(geVertex), (const void*) (shape->offsetBytesVertex + (sizeof(kmVec3) + sizeof(kmVec3))));
}

void addLine(kmVec3* v1, kmVec3* v2) {
    geObject* line = geObjectInit();
    line->shape = shapes + GE_LINE;
    line->size.x = line->size.y = line->size.z = 20;
    line->extraBrightness = 1.0f;
    line->texture = tex[1];

    kmVec3 temp;
    kmMat4 translate;
    kmVec3Subtract(&temp, v2, v1);
    temp.z /= 2;
    kmMat4Translation(&translate, v1->x, v1->y, v1->z);

}

void geObjectAdd(geObject* obj) {
    if (numObjects + 1 > MAX_OBJECTS) {
        fprintf(stderr, "Too many objects to add\n");
        return;
    }
    memcpy(&objects[numObjects++], obj, sizeof(geObject));
}

void geObjectAddAll(geObject* obj, size_t num) {
    if (numObjects + num > MAX_OBJECTS) {
        fprintf(stderr, "Too many objects to add\n");
        return;
    }
    memcpy(&objects[numObjects], obj, sizeof(geObject) * num);
    numObjects += num;
}

void initScene() {
    GLenum err;
    glUseProgram(programs[GE_PROGRAM_MAIN]);
    glGenVertexArrays(INDEX_NUM, vaos);
    glGenFramebuffers(2, fbos);
    glGenBuffers(2, vbos);

    // Initialize shapes
    initShapes();
    unsigned long long vertexBufferSize = 0;
    unsigned long long indexBufferSize = 0;
    for (int i = 0; i < INDEX_NUM; i++) {
        vertexBufferSize += shapes[i].numVertices * sizeof(geVertex);
        indexBufferSize += shapes[i].numIndices * sizeof(GLuint);
        if (i == 0) {
            shapes[i].offsetBytesVertex = 0;
            shapes[i].offsetBytesIndex = 0;
        } else {
            shapes[i].offsetBytesVertex = shapes[i - 1].offsetBytesVertex + shapes[i - 1].numVertices * sizeof(geVertex);
            shapes[i].offsetBytesIndex = shapes[i - 1].offsetBytesIndex + shapes[i - 1].numIndices * sizeof(GLuint);
        }
        shapes[i].vao = vaos[i];
    }

    currentOffsetVertex = shapes[INDEX_NUM - 1].offsetBytesVertex + shapes[INDEX_NUM - 1].numVertices * sizeof(geVertex);
    currentOffsetIndex = shapes[INDEX_NUM - 1].offsetBytesIndex + shapes[INDEX_NUM - 1].numIndices * sizeof(GLuint);

    // Add some extra space for dynamically added shapes
    vertexBufferSize += 10000000 * sizeof(geVertex);
    indexBufferSize += 10000000 * sizeof(GLuint);

    // Initialize buffers
    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_STATIC_DRAW);

    // Buffer all shapes
    for (int i = 0; i < INDEX_NUM; i++) {
        geShape* shape = shapes + i;
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glBufferSubData(GL_ARRAY_BUFFER, shape->offsetBytesVertex, sizeof(geVertex) * shape->numVertices, shape->vertices);
        if (shape->numIndices != 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, shape->offsetBytesIndex, sizeof(GLuint) * shape->numIndices, shape->indices);
        }

        glBindVertexArray(shape->vao);
        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(geVertex), (const void *) shape->offsetBytesVertex);
        // Normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(geVertex), (const void *) (shape->offsetBytesVertex + (sizeof(kmVec3))));
        // Texture coords attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(geVertex), (const void *) (shape->offsetBytesVertex + (sizeof(kmVec3) + sizeof(kmVec3))));
    }

    // Initialize textures
    unsigned char pixelsRed[] = { 153, 0, 0 };
    unsigned char pixelsBlue[] = { 0, 0, 140 };
    unsigned char pixelsWhite[] = { 255, 255, 255 };
    unsigned char pixelsYellow[] = { 255, 200, 0 };
    unsigned char pixelsGrayTransparent[] = { 64, 64, 64, 128 };

    SDL_Surface* grass = getTexture("../res/grass.jpg");
    unsigned char* pixelsGrass = cubifyTexture(grass->pixels, grass->w, grass->h);;
    loadTextureRaw(tex + GE_TEXTURE_GRASS_CUBIC, pixelsGrass, grass->w * 4, grass->h * 3, GL_RGB);
    SDL_FreeSurface(grass);
    free(pixelsGrass);

    loadTextureRaw(tex + GE_TEXTURE_RED, pixelsRed, 1, 1, GL_RGB);
    loadTextureRaw(tex + GE_TEXTURE_BLUE, pixelsBlue, 1, 1, GL_RGB);
    loadTextureRaw(tex + GE_TEXTURE_WHITE, pixelsWhite, 1, 1, GL_RGB);
    loadTextureRaw(tex + GE_TEXTURE_YELLOW, pixelsYellow, 1, 1, GL_RGB);
    loadTextureCubeMap(tex + GE_TEXTURE_SKY, "../res/sky4.png");
    loadTexture(tex + GE_TEXTURE_GRASS, "../res/grass.jpg");
    loadTextureRaw(tex + GE_TEXTURE_GRAY_TRANSPARENT, pixelsGrayTransparent, 1, 1, GL_RGBA);
    loadTexture(tex + GE_TEXTURE_BUTTON, "../res/button.png");

    glGenTextures(1, tex + GE_TEXTURE_SHADOW_MAP_512);
    glBindTexture(GL_TEXTURE_2D, tex[GE_TEXTURE_SHADOW_MAP_512]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, tex + GE_TEXTURE_SHADOW_MAP_16384);
    glBindTexture(GL_TEXTURE_2D, tex[GE_TEXTURE_SHADOW_MAP_16384]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 16384, 16384, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    loadTexture(tex + GE_TEXTURE_COBBLE, "../res/cobble.png");
    loadTexture(tex + GE_TEXTURE_COBBLE_2, "../res/cobble2.png");
    loadTexture(tex + GE_TEXTURE_CONTAINER, "../res/container2.png");

    loadTexture3D(tex + GE_TEXTURE_ATLAS, "../res/atlas.png");

    initObjects();

    kmMat4 rot;
    kmMat4RotationZ(&rot, 15 * PI / 180.0f);
    kmVec3MultiplyMat4(&sun->pos, &sun->pos, &rot);
    sun->rotation.z += 15;
    glUniform3fv(_U(pl[0]), 1, (const GLfloat *) &sun->pos);

    kmVec3 lightDirection = {0, -1, 0};
    kmVec3Subtract(&lightDirection, &lightDirection, &sun->pos);
    kmVec3Normalize(&lightDirection, &lightDirection);
    glUniform3fv(_U(dl), 1, (const GLfloat *) &lightDirection);

//    glGenRenderbuffers(1, rbos);
//    glBindRenderbuffer(GL_RENDERBUFFER, rbos[0]);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, 1, 1);
//    glFramebufferRenderbuffer()

    err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("Error after initScene: %s\n", gluErrorString(err));
    }
}

void update() {
    geCameraUpdate(&cameraMain);

    kmVec3 raycastResult = geCameraRaycast(&cameraMain, &worldMain);
    if (raycastResult.x == -1) {
        raycastResult = geCameraRaycast(&cameraMain, &worldsSecondary[0]);
    }
    if (raycastResult.x == -1) {
        raycastResult = geCameraRaycast(&cameraMain, &worldsSecondary[1]);
    }
    if (raycastResult.x == -1) {
        highlight->size.x = 0;
        highlight->size.y = 0;
        highlight->size.z = 0;
    } else {
        highlight->size.x = 1.01f;
        highlight->size.y = 1.01f;
        highlight->size.z = 1.01f;
    }
    memcpy(&highlight->pos, &raycastResult, sizeof(kmVec3));

    // Update lights
    kmMat4 rot;
//    kmMat4RotationZ(&rot, 0.004f * PI / 180.0f);
//    kmVec3MultiplyMat4(&sun->pos, &sun->pos, &rot);
//    sun->rotation.z += 0.004f;
//    glUniform3fv(_U(pl[0]), 1, (const GLfloat *) &sun->pos);
//
//    float skyDim = cosf(sun->rotation.z * PI / 180.0f);
//    if (skyDim <= 0.1f) {
//        skyDim = 0.1f;
//    }
    glUniform1f(_U(skyDim), 1);
//
    kmVec3 lightDirection = {0, -1, 0};
//    kmVec3Subtract(&lightDirection, &lightDirection, &sun->pos);
//    kmVec3Normalize(&lightDirection, &lightDirection);
//    glUniform3fv(_U(dl), 1, (const GLfloat *) &lightDirection);

    kmMat4Identity(&rot);
    glUniformMatrix4fv(_U(scaleBias), 1, GL_FALSE, rot.mat);

    // Update crosshair
//    memcpy(&crosshair->rotation, &cameraMain.direction, sizeof(kmVec3));
}

void drawScene() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform4f(_U(lightAmbient), 0.3f, 0.3f, 0.3f, 1);

    size_t i;
    for (i = 0; i < numObjects; i++) {
        geObject* obj = objects + i;
        if (obj == highlight && programs[GE_PROGRAM_WIREFRAME] < programs[GE_PROGRAM_MAIN]) {
            continue;
        }

        glUniform1i(_U(exemptFromView), obj->exemptFromView);
        glUniform1i(_U(exemptFromViewTranslation), obj->exemptFromViewTranslation);
        glUniform1i(_U(exemptFromViewProjection), obj->exemptFromViewProjection);
        glUniform1f(_U(extraBrightness), obj->extraBrightness);

        glActiveTexture(obj->glTextureId);
        glBindTexture(obj->glTextureType, obj->texture);
        glUniform1i(_U(useCubeMap), obj->glTextureType == GL_TEXTURE_CUBE_MAP);
        glUniform1i(_U(useAtlas), obj->glTextureType == GL_TEXTURE_2D_ARRAY);

        kmMat4 rot, scale, translation;
        if (i == 721) {
            kmMat4Scaling(&scale, 0.5f, 0.5f, 1);
            kmMat4Translation(&translation, 0.5f, 0.5f, 0);
            kmMat4Multiply(&rot, &scale, &translation);
            glUniformMatrix4fv(_U(scaleBias), 1, GL_FALSE, rot.mat);
        } else {
            kmMat4Identity(&rot);
            glUniformMatrix4fv(_U(scaleBias), 1, GL_FALSE, rot.mat);
        }
        geObjectDraw(obj);
    }
}

void geObjectDraw(geObject* obj) {
    // UPDATE MODEL UNIFORM
    kmMat4 rotX, rotY, rotZ;
    kmMat4 scale;
    kmMat4 translation;
    kmMat4 model;

    kmMat4RotationX(&rotX, obj->rotation.x * (PI / 180));
    kmMat4RotationY(&rotY, obj->rotation.y * (PI / 180));
    kmMat4RotationZ(&rotZ, obj->rotation.z * (PI / 180));
    kmMat4Scaling(&scale, obj->size.x, obj->size.y, obj->size.z);
    kmMat4Translation(&translation, obj->pos.x, obj->pos.y, obj->pos.z);

    kmMat4Identity(&model);
    kmMat4Multiply(&model, &model, &translation);
    kmMat4Multiply(&model, &model, &scale);

    if (obj == linePointer) {
        kmMat4Multiply(&model, &model, &rotY);
        kmMat4Multiply(&model, &model, &rotX);
        kmMat4Multiply(&model, &model, &rotZ);
    } else {
        kmMat4Multiply(&model, &model, &rotX);
        kmMat4Multiply(&model, &model, &rotY);
        kmMat4Multiply(&model, &model, &rotZ);
    }

    glUniformMatrix4fv(_U(model), 1, GL_FALSE, model.mat);

    // DRAW THE OBJECT
    GLenum primitive = obj->shape->numVertices == 2 || obj->shape == shapes + GE_2D_CROSSHAIR || obj->shape == shapes + GE_3D_CROSSHAIR || obj->shape == shapes + GE_CUBE_BORDER ? GL_LINES : GL_TRIANGLES;
    glBindVertexArray(obj->shape->vao);
    if (obj->shape->numIndices == 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glDrawArrays(primitive, (GLint) obj->shape->offsetBytesVertex / sizeof(geVertex), (GLsizei) (obj->shape->numVertices));
    } else {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
        glDrawElements(primitive, (GLsizei) obj->shape->numIndices, GL_UNSIGNED_INT, (const void*) obj->shape->offsetBytesIndex);
    }
    glBindVertexArray(0);
}

void clearScene() {
    for (int i = 0; i < INDEX_NUM; i++) {
        glDeleteFramebuffers(1, fbos);
        glDeleteVertexArrays(INDEX_NUM, vaos);
        glDeleteBuffers(2, vbos);
        glDeleteTextures(sizeof(tex) / sizeof(*tex), tex);
        free(shapes[i].vertices);
        free(shapes[i].indices);
        glDeleteProgram(programs[GE_PROGRAM_MAIN]);
        glDeleteProgram(programs[GE_PROGRAM_TEXTURE]);
    }
}