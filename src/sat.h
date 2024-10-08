#ifndef SAT_H
#define SAT_H

#include <raylib.h>
#include <stdbool.h>
#include "shape.h"

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

Collision CheckCollision(Shape *shape1, Shape *shape2);
Collision CheckCollisionCircleCircle(ShapeCircle *shape1, ShapeCircle *shape2);
Collision CheckCollisionPolyPoly(ShapePolygon *shape1, ShapePolygon *shape2);
Collision CheckCollisionPolyCircle(ShapePolygon *shape1, ShapeCircle *shape2);
Collision CheckCollisionCirclePoly(ShapeCircle *shape1, ShapePolygon *shape2);

Vector2    FindArithmetic(ShapePolygon *shape);
Projection ProjectPolygon(Vector2 axis, ShapePolygon *shape);
Projection ProjectCircle(Vector2 axis, ShapeCircle *shape);

#endif // SAT_H
