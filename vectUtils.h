//
// Created by gabri on 03/07/2025.
//

#ifndef VECTUTILS_H
#define VECTUTILS_H
#include "car.h"
#include "raylib.h"


Vector2 vector_from_points(Vector2 origin, Vector2 destination);
Vector2 symmetric_and_rotate(Vector2 vec,float angle);
void draw_vect_at_car_center(car*,Vector2,Color);
#endif //VECTUTILS_H
