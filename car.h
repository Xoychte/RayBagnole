//
// Created by gabri on 03/07/2025.
//

#ifndef CAR_H
#define CAR_H
#include "raylib.h"

typedef struct body {
    Vector2 frontLeft;
    Vector2 frontRight;
    Vector2 rearLeft;
    Vector2 rearRight;
} body;

typedef struct relativePositions { // All of these are constants, describing the car when its angle is null
    Vector2 CtoFl; // Center to Front left
    Vector2 FlToRl; // Front left to Rear left
} relativePositions; // The car is currently symmetric

typedef struct car {
    body body;
    relativePositions relativePositions;
    Vector2 centerPos;
    float angle;
} car;

Vector2 symmetric_CtoFr(const car* car);
Vector2 symmetric_FrtoBr(const car* car);
void compute_body_positions(car* car);
void display_car(const car* car);
car* create_le_car(void);

#endif //CAR_H
