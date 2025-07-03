//
// Created by gabri on 03/07/2025.
//

#ifndef CAR_H
#define CAR_H
#include "raylib.h"

typedef struct body {
    Vector2 frontLeft;
    Vector2 frontRight;
    Vector2 backLeft;
    Vector2 backRight;
} body;

typedef struct relativePositions {
    Vector2 fLtoBl;
} relativePositions;

typedef struct car {
    body body;
    relativePositions relativePositions;
    Vector2 centerPos;
    double distCFL; //distance between center point and front left corner
    double angle;
} car;

void compute_body_positions(car* car);
void display_car(car* car);
#endif //CAR_H
