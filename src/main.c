#include "main.h"
#include <raylib.h>
#include <raymath.h>
#include "object.h"
#include "sat.h"

int main(int argc, char **argv)
{
    // SetTargetFPS(60);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    Object *cube = NewSquare(90, 90);
    Object *poly = NewPolygon(60, 7);
    poly->center = (Vector2){
        .x = WINDOW_WIDTH_HALF,
        .y = WINDOW_HEIGHT_HALF,
    };

    while (!WindowShouldClose()) {
        cube->center = GetMousePosition();
        Collision c  = CheckCollision(poly, cube);
        if (c.collided == true) {
            poly->center = Vector2Subtract(poly->center, Vector2Scale(c.normal, c.depth));
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangleLines(10, 10, WINDOW_WIDTH - 20, WINDOW_HEIGHT - 20, LIGHTGRAY);
        DrawLine(WINDOW_WIDTH_HALF, 0, WINDOW_WIDTH_HALF, WINDOW_HEIGHT, LIGHTGRAY);
        DrawLine(0, WINDOW_HEIGHT_HALF, WINDOW_WIDTH, WINDOW_HEIGHT_HALF, LIGHTGRAY);

        DrawObjectLines(cube);
        DrawObjectLines(poly);

        DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();

    DestroyObject(cube);
    DestroyObject(poly);

    return 0;
}
