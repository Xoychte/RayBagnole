//
// Created by gabri on 03/07/2025.
//

#include "vectUtils.h"
Vector2 vector_from_points(Vector2 origin, Vector2 destination) {
    return (Vector2){destination.x - origin.x, destination.y - origin.y};
}