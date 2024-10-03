#include "main.h"
#include <raylib.h>
#include <raymath.h>

int main(int argc, char **argv)
{
    SetTargetFPS(60);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    Object cube = {
        .center = {0, 400},
        .vertices =
            {
                {-50, -50},
                {-50, 50},
                {50, 50},
                {50, -50},
            },
        .length = 4,
    };
    Object poly = {
        .center = {400, 225},
        .vertices =
            {
                {-50, -50},
                {50, -50},
                {80, 50},
                {0, 70},
                {-80, 50},
            },
        .length = 5,
    };

    while (!WindowShouldClose()) {
        cube.center = GetMousePosition();
        Collision c = CheckCollision(poly, cube);
        if (c.collision == true) {
            poly.center = Vector2Subtract(poly.center, Vector2Scale(c.normal, c.depth));
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleLines(10, 10, WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20, DARKGRAY); // Border Lines
        DrawLine(WINDOW_WIDTH_HALF, 0, WINDOW_WIDTH_HALF, WINDOW_HEIGHT, DARKGRAY);  // Vertical Line
        DrawLine(0, WINDOW_HEIGHT_HALF, WINDOW_WIDTH, WINDOW_HEIGHT_HALF, DARKGRAY); // Horizontal Line

        DrawObject(cube);
        DrawObject(poly);

        DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

Collision CheckCollision(Object obj1, Object obj2)
{
    Vector2 normal = {0, 0};
    float   depth  = INFINITY;

    for (int i = 0; i < obj1.length; i += 1) {
        Vector2 edge = Vector2Subtract(Vector2Add(obj1.vertices[i], obj1.center),
                                       Vector2Add(obj1.vertices[(i + 1) % obj1.length], obj1.center));
        Vector2 axis = Vector2Normalize((Vector2){edge.y * -1, edge.x});

        Projection proj1 = ProjectToPlane(axis, obj1);
        Projection proj2 = ProjectToPlane(axis, obj2);

        if (proj1.max < proj2.min || proj2.max < proj1.min)
            return (Collision){normal, depth, false};

        float axisDepth = fminf(proj1.max - proj2.min, proj2.max - proj1.min);

        if (axisDepth < depth) {
            depth  = axisDepth;
            normal = axis;
        }
    }

    for (int i = 0; i < obj2.length; i += 1) {
        Vector2 edge = Vector2Subtract(Vector2Add(obj2.vertices[i], obj2.center),
                                       Vector2Add(obj2.vertices[(i + 1) % obj2.length], obj2.center));
        Vector2 axis = Vector2Normalize((Vector2){edge.y * -1, edge.x});

        Projection proj1 = ProjectToPlane(axis, obj1);
        Projection proj2 = ProjectToPlane(axis, obj2);

        if (proj1.max < proj2.min || proj2.max < proj1.min)
            return (Collision){normal, depth, false};

        float axisDepth = fminf(proj1.max - proj2.min, proj2.max - proj1.min);

        if (axisDepth < depth) {
            depth  = axisDepth;
            normal = axis;
        }
    }

    depth /= Vector2Length(normal);
    normal = Vector2Normalize(normal);

    Vector2 center1   = FindArithmetic(obj1);
    Vector2 center2   = FindArithmetic(obj2);
    Vector2 direction = Vector2Subtract(center1, center2);

    if (Vector2DotProduct(direction, normal) > 0)
        normal = Vector2Scale(normal, -1);

    return (Collision){normal, depth, true};
}

Vector2 FindArithmetic(Object obj)
{
    float sumX = 0;
    float sumY = 0;

    for (int i = 0; i < obj.length; i += 1) {
        Vector2 v = Vector2Add(obj.vertices[i], obj.center);
        sumX += v.x;
        sumY += v.y;
    }

    Vector2 mean = (Vector2){
        .x = sumX / (float)obj.length,
        .y = sumY / (float)obj.length,
    };

    return mean;
}

Projection ProjectToPlane(Vector2 axis, Object obj)
{
    float min = Vector2DotProduct(axis, Vector2Add(obj.vertices[0], obj.center));
    float max = min;

    for (int i = 1; i < obj.length; i += 1) {
        float point = Vector2DotProduct(axis, Vector2Add(obj.vertices[i], obj.center));
        if (point < min)
            min = point;
        if (point > max)
            max = point;
    }

    Projection proj = {
        .min = min,
        .max = max,
    };

    return proj;
}

void DrawObject(Object obj)
{
    for (int i = 0; i < obj.length; i += 1) {
        Vector2 p1 = Vector2Add(obj.vertices[i], obj.center);
        Vector2 p2 = Vector2Add(obj.vertices[(i + 1) % obj.length], obj.center);
        DrawLineEx(p1, p2, 1, ORANGE);
    }
}
