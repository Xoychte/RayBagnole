//
// Created by gabri on 03/07/2025.
//

#include "car.h"

#include <stdio.h>
#include <stdlib.h>

#include "vectUtils.h"
#include "raymath.h"

Vector2 symmetric_CtoFr (const car* car) {
    Vector2 symmetric = (Vector2){car->relativePositions.CtoFl.x,-car->relativePositions.CtoFl.y};
    return Vector2Rotate(symmetric,car->angle);
}

Vector2 symmetric_FrtoBr (const car* car) {
    Vector2 symmetric = (Vector2){car->relativePositions.FlToRl.x,-car->relativePositions.FlToRl.y};
    return Vector2Rotate(symmetric,car->angle);
}

Vector2 get_facing_vector (const car* car) {
    return Vector2Rotate((Vector2){1,0},car->angle);
}


void compute_body_positions(car* car) {
    const Vector2 c = car->centerPos;
    Vector2 c2Fl = Vector2Rotate(car->relativePositions.CtoFl,car->angle);

    car->body.frontLeft = Vector2Add(c, c2Fl);

    Vector2 Fl2Rl = Vector2Rotate(car->relativePositions.FlToRl,car->angle);
    car->body.rearLeft = Vector2Add(Fl2Rl, car->body.frontLeft);

    car->body.frontRight = Vector2Add(c, symmetric_CtoFr(car));
    car->body.rearRight = Vector2Add(symmetric_FrtoBr(car), car->body.frontRight);
}

void display_car(const car* car) {
    const Vector2 fl = car->body.frontLeft;
    const Vector2 fr = car->body.frontRight;
    const Vector2 br = car->body.rearRight;
    const Vector2 bl = car->body.rearLeft;
    const Vector2 c = car->centerPos;


    DrawLineV(fl,fr,BLACK);
    DrawLineV(br,bl,BLACK);
    DrawLineV(fr,br,BLACK);

    DrawLineV(fl,bl,BLACK);
    DrawLineV(c,fl,RED);

    DrawPixelV(c,MAGENTA);

    DrawPixelV(fl,BLUE);
    DrawPixelV(bl,DARKBLUE);
    DrawPixelV(fr,GREEN);
    DrawPixelV(br,DARKBLUE);

}

car* create_le_car(void) {
    car* car = malloc(sizeof(struct car));
    if (car == NULL) {
        printf("Error allocating memory for car\n");
        exit(EXIT_FAILURE);
    }

    car->centerPos = (Vector2){200,200};
    car->relativePositions.CtoFl = (Vector2){50,-30};
    car->relativePositions.FlToRl = (Vector2){-100,-5};


    car->body.frontLeft = Vector2Zero();
    car->body.frontRight = Vector2Zero();
    car->body.rearRight = Vector2Zero();
    car->body.rearLeft = Vector2Zero();

    compute_body_positions(car);

    car->mechanics.acceleration = (Vector2){0,0};
    car->mechanics.speed = (Vector2){0,0};
    car->mechanics.mass = 10;

    return car;
}