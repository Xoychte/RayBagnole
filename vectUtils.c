//
// Created by gabri on 03/07/2025.
//

#include "vectUtils.h"

#include <math.h>

#include "raymath.h"

Vector2 vector_from_points(Vector2 origin, Vector2 destination) {
    return (Vector2){destination.x - origin.x, destination.y - origin.y};
}

Vector2 symmetric_and_rotate (Vector2 vec, float angle) {
    Vector2 res = (Vector2){vec.x, (-1)*vec.y};
    return Vector2Rotate(res, angle);
}