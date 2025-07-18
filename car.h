//
// Created by gabri on 03/07/2025.
//

#ifndef CAR_H
#define CAR_H
#include "raylib.h"



typedef struct wheels {
    float FwheelRadius;
    float FwheelWidth;
    Vector2 FwheelCenter;
    float FwheelAngle;
    float FaxleMass;

    float RwheelRadius;
    float RwheelWidth;
    Vector2 RwheelCenter;
    float RaxleMass;

} wheels;

typedef struct body {
    Vector2 frontLeft;
    Vector2 frontRight;
    Vector2 rearLeft;
    Vector2 rearRight;
} body;

typedef struct relativePositions { // All of these are constants, describing the car when its angle is null
    Vector2 CtoFl; // Center to Front left
    Vector2 FlToRl; // Front left to Rear left
    Vector2 CtofLw; // Center to front left wheel
    Vector2 CtorLw; // Center to rear left wheel
} relativePositions; // The car is currently symmetric

typedef struct mechanics {
    float mass;
    Vector2 speed;
    Vector2 acceleration;
} mechanics;

typedef struct car {
    body body;
    wheels wheels;
    relativePositions relativePositions;
    Vector2 centerPos; // Also center of mass
    float angle;
    mechanics mechanics;
} car;

Vector2 symmetric_CtoFr(const car* car);
Vector2 symmetric_FrtoBr(const car* car);
Vector2 get_facing_vector(const car* car);
void compute_body_positions(car* car);
void display_wheels(car* car);
void display_body(const car* car);
car* create_le_car(void);
void camera_follow(car* car, Camera2D* camera);

#endif //CAR_H
