#include "sat.h"
#include <raylib.h>
#include <raymath.h>

Collision CheckCollision(Object *obj1, Object *obj2)
{
    Vector2 normal = {0, 0};
    float   depth  = INFINITY;

    for (int i = 0; i < obj1->v_length; i += 1) {
        Vector2 edge = Vector2Subtract(VertexToWorldSpace(obj1, i), VertexToWorldSpace(obj1, (i + 1) % obj1->v_length));
        Vector2 axis = Vector2Normalize((Vector2){edge.y * -1, edge.x});

        Projection proj1 = ProjectToPlane(axis, obj1);
        Projection proj2 = ProjectToPlane(axis, obj2);

        if (proj1.max < proj2.min || proj2.max < proj1.min)
            return (Collision){
                .normal   = normal,
                .depth    = depth,
                .collided = false,
            };

        float axisDepth = fminf(proj1.max - proj2.min, proj2.max - proj1.min);

        if (axisDepth < depth) {
            depth  = axisDepth;
            normal = axis;
        }
    }

    for (int i = 0; i < obj2->v_length; i += 1) {
        Vector2 edge = Vector2Subtract(VertexToWorldSpace(obj2, i), VertexToWorldSpace(obj2, (i + 1) % obj2->v_length));
        Vector2 axis = Vector2Normalize((Vector2){edge.y * -1, edge.x});

        Projection proj1 = ProjectToPlane(axis, obj1);
        Projection proj2 = ProjectToPlane(axis, obj2);

        if (proj1.max < proj2.min || proj2.max < proj1.min)
            return (Collision){
                .normal   = normal,
                .depth    = depth,
                .collided = false,
            };

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

    return (Collision){
        .normal   = normal,
        .depth    = depth,
        .collided = true,
    };
}

Vector2 FindArithmetic(Object *obj)
{
    float sumX = 0;
    float sumY = 0;

    for (int i = 0; i < obj->v_length; i += 1) {
        Vector2 v = VertexToWorldSpace(obj, i);
        sumX += v.x;
        sumY += v.y;
    }

    Vector2 mean = (Vector2){
        .x = sumX / (float)obj->v_length,
        .y = sumY / (float)obj->v_length,
    };

    return mean;
}

Projection ProjectToPlane(Vector2 axis, Object *obj)
{
    float min = Vector2DotProduct(axis, VertexToWorldSpace(obj, 0));
    float max = min;

    for (int i = 1; i < obj->v_length; i += 1) {
        float point = Vector2DotProduct(axis, VertexToWorldSpace(obj, i));
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
