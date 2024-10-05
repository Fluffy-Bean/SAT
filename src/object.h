#ifndef OBJECT_H
#define OBJECT_H

#include <raylib.h>

typedef struct Object Object;
struct Object {
    Vector2  center;
    Vector2 *vertices;
    int      v_length;
};

Object *NewSquare(float width, float height);
Object *NewPolygon(float radius, int sides);
void    DestroyObject(Object *object);
Vector2 VertexToWorldSpace(Object *obj, int vertex);
void    DrawObjectLines(Object *obj);

#endif // OBJECT_H
