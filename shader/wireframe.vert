#version 450 core

// THIS DOESN'T WORK
//struct geVertex {
//    vec3 position;
//    vec3 normal;
//};
//layout (location = 0) in geVertex data;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 texCoords;

layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;
layout (location = 3) uniform mat4 scaleBias;
layout (location = 4) uniform mat4 shadowView;
layout (location = 5) uniform mat4 shadowProjection;
layout (location = 6) uniform mat4 shadowScaleBias;

layout (location = 10) uniform bool exemptFromViewTranslation;
layout (location = 12) uniform bool exemptFromViewProjection;

out vec3 GeomNormal;
out vec3 GeomTexCoords;
out vec3 GeomPos;
out vec3 GeomVertexPos;
out vec3 GeomShadowPos;

void main(void) {
    vec3 biasedTexCoords = vec3(scaleBias * vec4(texCoords, 1.0));
    GeomTexCoords = biasedTexCoords;
    GeomVertexPos = position;
    GeomShadowPos = vec3(shadowScaleBias * shadowProjection * shadowView * model * vec4(position, 1.0f));

    // vec4(v, 0) means turning on affine transformations
    // This is a very simple way of cutting off the translation part from the normal
    // This doesn't account for non-uniform scaling (ex: scaling by (1.5, 3, 1))
    GeomNormal = normalize(mat3(model) * normal);
    GeomPos = vec3(model * vec4(position, 1.0f));
    // ORDER MATTERS LOL
    if (exemptFromViewProjection) {
        gl_Position = model * vec4(position, 1.0f);
    } else if (exemptFromViewTranslation) {
        // Same as normal vertex BUT keep ONLY rotation of view matrix (world->view)
        gl_Position = projection * mat4(mat3(view)) * model * vec4(position, 1.0f);
    } else {
        gl_Position = projection * view * model * vec4(position, 1.0f);
    }
}