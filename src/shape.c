#include "shape.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

Shape *NewCircle(float radius)
{
    Shape *shape = malloc(sizeof(Shape));

    shape->type               = Circle;
    shape->data.circle.radius = radius;
    shape->data.circle.center = (Vector2){0, 0};

    return shape;
}

Shape *NewPolygon(float radius, int faces)
{
    Vector2 *vertices = malloc(sizeof(Vector2) * faces);
    Shape   *shape    = malloc(sizeof(Shape));

    // Set vertices
    for (int i = 0; i < faces; i += 1) {
        float   angle = 2 * PI * (float)i / (float)faces;
        Vector2 point = (Vector2){
            .x = cosf(angle) * radius,
            .y = sinf(angle) * radius,
        };
        vertices[i] = point;
    }

    // construct shape
    shape->type                  = Polygon;
    shape->data.polygon.center   = (Vector2){0, 0};
    shape->data.polygon.vertices = vertices;
    shape->data.polygon.v_count  = faces;

    return shape;
}

Shape *NewRectangle(float width, float height)
{
    // Allocate memory
    Vector2 *vertices = malloc(sizeof(Vector2) * 4);
    Shape   *shape    = malloc(sizeof(Shape));

    float h_width  = width / 2.0f;
    float h_height = height / 2.0f;

    // Set vertices
    vertices[0] = (Vector2){-h_width, -h_height};
    vertices[1] = (Vector2){-h_width, h_height};
    vertices[2] = (Vector2){h_width, h_height};
    vertices[3] = (Vector2){h_width, -h_height};

    // construct shape
    shape->type                  = Polygon;
    shape->data.polygon.center   = (Vector2){0, 0};
    shape->data.polygon.vertices = vertices;
    shape->data.polygon.v_count  = 4;

    return shape;
}

void DrawObjectLines(Shape *shape)
{

    switch (shape->type) {
        case Circle:
            DrawCircleLinesV(shape->data.circle.center, shape->data.circle.radius, ORANGE);
            // DrawCircleV(shape->data.circle.center, shape->data.circle.radius, BLUE);
            break;
        case Polygon:
            for (int i = 0; i < shape->data.polygon.v_count; i += 1) {
                Vector2 p1 = Vector2Add(shape->data.polygon.vertices[i], shape->data.polygon.center);
                Vector2 p2 = Vector2Add(shape->data.polygon.vertices[(i + 1) % shape->data.polygon.v_count],
                                        shape->data.polygon.center);
                DrawLineEx(p1, p2, 1, ORANGE);
            }
            break;
    }
}

void DestroyShape(Shape *shape)
{
    switch (shape->type) {
        case Circle:
            free(shape);
            break;
        case Polygon:
            // Vertices need to be freed as it's a pointer to the array
            free(shape->data.polygon.vertices);
            free(shape);
            break;
    }
}
