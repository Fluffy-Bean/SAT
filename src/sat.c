#include "sat.h"
#include <raylib.h>
#include <raymath.h>

#include "shape.h"

Collision CheckCollision(Shape *shape1, Shape *shape2)
{
    if (shape1->type == Circle && shape2->type == Circle)
        return CheckCollisionCircleCircle(&shape1->data.circle, &shape2->data.circle);
    if (shape1->type == Polygon && shape2->type == Polygon)
        return CheckCollisionPolyPoly(&shape1->data.polygon, &shape2->data.polygon);
    if (shape1->type == Circle && shape2->type == Polygon)
        return CheckCollisionCirclePoly(&shape1->data.circle, &shape2->data.polygon);
    if (shape1->type == Polygon && shape2->type == Circle)
        return CheckCollisionPolyCircle(&shape1->data.polygon, &shape2->data.circle);
    return (Collision){.collided = false};
}

Collision CheckCollisionCircleCircle(ShapeCircle *shape1, ShapeCircle *shape2)
{
    Vector2 normal = (Vector2){.x = 0, .y = 0};
    float   depth  = 0;

    float radii    = shape1->radius + shape2->radius;
    float distance = Vector2Distance(shape1->center, shape2->center);
    if (distance >= radii) {
        return (Collision){
            .normal   = normal,
            .depth    = depth,
            .collided = false,
        };
    }

    normal = Vector2Normalize(Vector2Subtract(shape2->center, shape1->center));
    depth  = radii - distance;

    return (Collision){
        .normal   = normal,
        .depth    = depth,
        .collided = true,
    };
}

Collision CheckCollisionPolyPoly(ShapePolygon *shape1, ShapePolygon *shape2)
{
    Vector2 normal = (Vector2){.x = 0, .y = 0};
    float   depth  = INFINITY;

    for (int i = 0; i < shape1->v_count; i += 1) {
        Vector2 edge = Vector2Subtract(Vector2Add(shape1->center, shape1->vertices[i]),
                                       Vector2Add(shape1->center, shape1->vertices[(i + 1) % shape1->v_count]));
        Vector2 axis = Vector2Normalize((Vector2){edge.y * -1, edge.x});

        Projection proj1 = ProjectPolygon(axis, shape1);
        Projection proj2 = ProjectPolygon(axis, shape2);

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

    for (int i = 0; i < shape2->v_count; i += 1) {
        Vector2 edge = Vector2Subtract(Vector2Add(shape2->center, shape2->vertices[i]),
                                       Vector2Add(shape2->center, shape2->vertices[(i + 1) % shape2->v_count]));
        Vector2 axis = Vector2Normalize((Vector2){edge.y * -1, edge.x});

        Projection proj1 = ProjectPolygon(axis, shape1);
        Projection proj2 = ProjectPolygon(axis, shape2);

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

    Vector2 center1   = FindArithmetic(shape1);
    Vector2 center2   = FindArithmetic(shape2);
    Vector2 direction = Vector2Subtract(center1, center2);

    if (Vector2DotProduct(direction, normal) > 0)
        normal = Vector2Scale(normal, -1);

    return (Collision){
        .normal   = normal,
        .depth    = depth,
        .collided = true,
    };
}

Collision CheckCollisionPolyCircle(ShapePolygon *shape1, ShapeCircle *shape2)
{
    Vector2 normal = (Vector2){.x = 0, .y = 0};
    float   depth  = INFINITY;

    for (int i = 0; i < shape1->v_count; i += 1) {
        Vector2 edge = Vector2Subtract(Vector2Add(shape1->center, shape1->vertices[i]),
                                       Vector2Add(shape1->center, shape1->vertices[(i + 1) % shape1->v_count]));
        Vector2 axis = Vector2Normalize((Vector2){edge.y * -1, edge.x});

        Projection proj1 = ProjectPolygon(axis, shape1);
        Projection proj2 = ProjectCircle(axis, shape2);

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

    {
        Vector2    axis  = Vector2Normalize(Vector2Subtract(shape2->center, shape1->center));
        Projection proj1 = ProjectPolygon(axis, shape1);
        Projection proj2 = ProjectCircle(axis, shape2);

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

    Vector2 center1   = FindArithmetic(shape1);
    Vector2 center2   = shape2->center;
    Vector2 direction = Vector2Subtract(center1, center2);

    if (Vector2DotProduct(direction, normal) > 0)
        normal = Vector2Scale(normal, -1);

    return (Collision){
        .normal   = normal,
        .depth    = depth,
        .collided = true,
    };
}

Collision CheckCollisionCirclePoly(ShapeCircle *shape1, ShapePolygon *shape2)
{
    Vector2 normal = (Vector2){.x = 0, .y = 0};
    float   depth  = INFINITY;

    {
        Vector2    axis  = Vector2Normalize(Vector2Subtract(shape2->center, shape1->center));
        Projection proj1 = ProjectCircle(axis, shape1);
        Projection proj2 = ProjectPolygon(axis, shape2);

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

    for (int i = 0; i < shape2->v_count; i += 1) {
        Vector2 edge = Vector2Subtract(Vector2Add(shape2->center, shape2->vertices[i]),
                                       Vector2Add(shape2->center, shape2->vertices[(i + 1) % shape2->v_count]));
        Vector2 axis = Vector2Normalize((Vector2){edge.y * -1, edge.x});

        Projection proj1 = ProjectCircle(axis, shape1);
        Projection proj2 = ProjectPolygon(axis, shape2);

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

    Vector2 center1   = shape1->center;
    Vector2 center2   = FindArithmetic(shape2);
    Vector2 direction = Vector2Subtract(center1, center2);

    if (Vector2DotProduct(direction, normal) > 0)
        normal = Vector2Scale(normal, -1);

    return (Collision){
        .normal   = normal,
        .depth    = depth,
        .collided = true,
    };
}

Vector2 FindArithmetic(ShapePolygon *shape)
{
    float sumX = 0;
    float sumY = 0;

    for (int i = 0; i < shape->v_count; i += 1) {
        Vector2 v = Vector2Add(shape->center, shape->vertices[i]);
        sumX += v.x;
        sumY += v.y;
    }

    Vector2 mean = (Vector2){
        .x = sumX / (float)shape->v_count,
        .y = sumY / (float)shape->v_count,
    };

    return mean;
}

Projection ProjectPolygon(Vector2 axis, ShapePolygon *shape)
{
    float min = Vector2DotProduct(axis, Vector2Add(shape->center, shape->vertices[0]));
    float max = min;

    for (int i = 1; i < shape->v_count; i += 1) {
        float point = Vector2DotProduct(axis, Vector2Add(shape->center, shape->vertices[i]));
        if (point < min)
            min = point;
        if (point > max)
            max = point;
    }

    return (Projection){
        .min = min,
        .max = max,
    };
}

Projection ProjectCircle(Vector2 axis, ShapeCircle *shape)
{
    float center = Vector2DotProduct(axis, shape->center);
    float min    = center - shape->radius;
    float max    = center + shape->radius;

    return (Projection){
        .min = min,
        .max = max,
    };
}
