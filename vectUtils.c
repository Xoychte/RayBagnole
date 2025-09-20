//
// Created by gabri on 03/07/2025.
//

#include "vectUtils.h"

#include <stdio.h>


#include "raymath.h"

Vector2 vector_from_points(Vector2 origin, Vector2 destination) {
    return (Vector2){destination.x - origin.x, destination.y - origin.y};
}

Vector2 symmetric_and_rotate (Vector2 vec, float angle) {
    Vector2 res = (Vector2){vec.x, (-1)*vec.y};
    return Vector2Rotate(res, angle);
}

void draw_vect_at_car_center (car* car, Vector2 vec,Color color) {
    DrawLineV(car->centerPos,Vector2Add(car->centerPos,vec),color);
}

float multiply(Vector2 a, Vector2 b) {
    return Vector2Length(a) * Vector2Length(b) * sin(Vector2Angle(a,b));
}

Vector2 symmetric(Vector2 vec) {
    Vector2 res = (Vector2){vec.x, -vec.y};
    return res;
}

void draw_rec_from_center(Vector2 center, float width, float heigth, Color color) {
    Vector2 offset = (Vector2){-width/2, -heigth/2};
    draw_rec_lines_v(Vector2Add(center,offset),(Vector2){width,heigth},color);
}

void draw_rec_lines_v (Vector2 position, Vector2 size, Color color) {
    DrawRectangleLines(position.x, position.y, size.x, size.y, color);
}