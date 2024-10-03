#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH_HALF (WINDOW_WIDTH / 2)
#define WINDOW_HEIGHT_HALF (WINDOW_HEIGHT / 2)
#define WINDOW_TITLE "SAT Demo"


typedef struct Object Object;
struct Object {
    Vector2 center;
    Vector2 vertices[10]; // Easier than doing linked lists
    int     length;       // how many vertices the shape has
};

typedef struct Collision Collision;
struct Collision {
    Vector2 normal;
    float   depth;
    bool    collision;
};

typedef struct Projection Projection;
struct Projection {
    float min;
    float max;
};


Collision  CheckCollision(Object obj1, Object obj2);
Vector2    FindArithmetic(Object obj);
Projection ProjectToPlane(Vector2 axis, Object obj);
Vector2    ToWorldPosition(Object obj);

void DrawObject(Object obj);

#endif // MAIN_H
