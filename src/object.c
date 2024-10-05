#include "object.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

Object *NewSquare(float width, float height)
{
    float h_width  = width / 2.0f;
    float h_height = height / 2.0f;

    Vector2 *vertices = malloc(sizeof(Vector2) * 4);
    vertices[0]       = (Vector2){-h_width, -h_height};
    vertices[1]       = (Vector2){-h_width, h_height};
    vertices[2]       = (Vector2){h_width, h_height};
    vertices[3]       = (Vector2){h_width, -h_height};

    Object *object   = malloc(sizeof(Object));
    object->center   = (Vector2){0, 0};
    object->vertices = vertices;
    object->v_length = 4;

    return object;
}

Object *NewPolygon(float radius, int sides)
{
    Vector2 *vertices = malloc(sizeof(Vector2) * sides);
    for (int i = 0; i < sides; i += 1) {
        float   angle = 2 * PI * (float)i / (float)sides;
        Vector2 point = (Vector2){
            .x = cosf(angle) * radius,
            .y = sinf(angle) * radius,
        };
        vertices[i] = point;
    }

    Object *object   = malloc(sizeof(Object));
    object->center   = (Vector2){0, 0};
    object->vertices = vertices;
    object->v_length = sides;

    return object;
}

void DestroyObject(Object *object)
{
    free(object->vertices);
    free(object);
}

Vector2 VertexToWorldSpace(Object *obj, int vertex)
{
    Vector2 ws = Vector2Add(obj->vertices[vertex], obj->center);
    return ws;
}

void DrawObjectLines(Object *obj)
{
    for (int i = 0; i < obj->v_length; i += 1) {
        Vector2 p1 = Vector2Add(obj->vertices[i], obj->center);
        Vector2 p2 = Vector2Add(obj->vertices[(i + 1) % obj->v_length], obj->center);
        DrawLineEx(p1, p2, 1, ORANGE);
    }
}
