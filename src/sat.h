#ifndef SAT_H
#define SAT_H

#include <raylib.h>
#include <stdbool.h>
#include "object.h"

typedef struct Collision Collision;
struct Collision {
    Vector2 normal;
    float   depth;
    bool    collided;
};

typedef struct Projection Projection;
struct Projection {
    float min;
    float max;
};

Collision  CheckCollision(Object *obj1, Object *obj2);
Vector2    FindArithmetic(Object *obj);
Projection ProjectToPlane(Vector2 axis, Object *obj);

#endif // SAT_H
