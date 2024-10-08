#include "main.h"
#include <raylib.h>
#include <raymath.h>
#include "sat.h"
#include "shape.h"

// How many circles to put into the scene
Shape *shapes[1000];
// a value of how strongly to push shapes/colliders away
float SQUISH = 10;

int main(int argc, char **argv)
{
    // SetTargetFPS(60);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    shapes[0] = NewRectangle(40, 40);

    for (int i = 1; i < lenof(shapes); i += 1)
        shapes[i] = NewCircle(2);

    for (int i = 0; i < lenof(shapes); i += 1) {
        switch (shapes[i]->type) {
            case Circle:
                shapes[i]->data.circle.center = (Vector2){WINDOW_WIDTH_HALF, WINDOW_HEIGHT_HALF};
                break;
            case Polygon:
                shapes[i]->data.polygon.center = (Vector2){WINDOW_WIDTH_HALF, WINDOW_HEIGHT_HALF};
                break;
        }
    }

    while (!WindowShouldClose()) {
        shapes[0]->data.polygon.center = GetMousePosition();

        for (int i = 1; i < lenof(shapes); i += 1) {
            for (int j = 0; j < lenof(shapes); j += 1) {
                if (i == j)
                    continue;
                Collision collision = CheckCollision(shapes[i], shapes[j]);
                if (collision.collided == true) {
                    switch (shapes[i]->type) {
                        case Circle:
                            shapes[i]->data.circle.center =
                                Vector2Subtract(shapes[i]->data.circle.center,
                                                Vector2Scale(collision.normal, collision.depth * SQUISH * GetFrameTime()));
                            shapes[i]->data.circle.center = Vector2Clamp(shapes[i]->data.circle.center, (Vector2){0, 0},
                                                                         (Vector2){WINDOW_WIDTH, WINDOW_HEIGHT});
                            break;
                        case Polygon:
                            shapes[i]->data.polygon.center =
                                Vector2Subtract(shapes[i]->data.polygon.center,
                                                Vector2Scale(collision.normal, collision.depth * SQUISH * GetFrameTime()));
                            shapes[i]->data.polygon.center =
                                Vector2Clamp(shapes[i]->data.polygon.center, (Vector2){0, 0},
                                             (Vector2){WINDOW_WIDTH, WINDOW_HEIGHT});
                            break;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleLines(10, 10, WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20, LIGHTGRAY);
        DrawLine(WINDOW_WIDTH_HALF, 0, WINDOW_WIDTH_HALF, WINDOW_HEIGHT, LIGHTGRAY);
        DrawLine(0, WINDOW_HEIGHT_HALF, WINDOW_WIDTH, WINDOW_HEIGHT_HALF, LIGHTGRAY);

        for (int i = 0; i < lenof(shapes); i += 1)
            DrawObjectLines(shapes[i]);

        DrawFPS(20, 20);
        EndDrawing();
    }

    for (int i = 0; i < lenof(shapes); i += 1)
        DestroyShape(shapes[i]);

    CloseWindow();

    return 0;
}
