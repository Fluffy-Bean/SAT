#ifndef SHAPE_H
#define SHAPE_H

#include <raylib.h>

typedef struct ShapeCircle {
    Vector2 center;
    float   radius;
} ShapeCircle;

typedef struct ShapePolygon {
    Vector2  center;
    Vector2 *vertices;
    int      v_count;
} ShapePolygon;

typedef union ShapeUnion {
    ShapeCircle  circle;
    ShapePolygon polygon;
} ShapeUnion;

typedef enum ShapeType {
    Circle,
    Polygon,
} ShapeType;

typedef struct Shape {
    ShapeType  type;
    ShapeUnion data;
} Shape;

Shape *NewCircle(float radius);
Shape *NewPolygon(float radius, int faces);
Shape *NewRectangle(float width, float height);

void DrawObjectLines(Shape *shape);
void DestroyShape(Shape *shape);

#endif // SHAPE_H
