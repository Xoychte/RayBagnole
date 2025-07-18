//
// Created by gabri on 03/07/2025.
//

#include "car.h"

#include <stdio.h>
#include <stdlib.h>

#include "vectUtils.h"
#include "raymath.h"

Vector2 symmetric_CtoFr (const car* car) {
    return symmetric_and_rotate(car->relativePositions.CtoFl,car->angle);
}

Vector2 symmetric_FrtoBr (const car* car) {
    return symmetric_and_rotate(car->relativePositions.FlToRl,car->angle);
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

    car->wheels.FwheelCenter = Vector2Add(car->centerPos,Vector2Rotate(car->relativePositions.CtofLw,car->angle));
    car->wheels.RwheelCenter = Vector2Add(car->centerPos,Vector2Rotate(car->relativePositions.CtorLw,car->angle));
}

void display_wheels (car* car) { //TODO look into the weird interaction when turning
    float FwBis = car->wheels.FwheelWidth / 2;
    float fullAngle = car->angle + car->wheels.FwheelAngle;

    //Drawing the front left tire (awful)

    Vector2 fLwfLc = Vector2Add(car->wheels.FwheelCenter, Vector2Rotate((Vector2){car->wheels.FwheelRadius,FwBis},fullAngle)); //Front left corner of the front left tire
    Vector2 fLwfRc = Vector2Add(car->wheels.FwheelCenter,Vector2Rotate((Vector2){car->wheels.FwheelRadius,(-1)*FwBis},fullAngle)); //Front right corner of the front left tire
    DrawLineV(fLwfLc,fLwfRc,GRAY);

    Vector2 tireSide = Vector2Rotate((Vector2){car->wheels.FwheelRadius * 2,0.f},fullAngle);

    Vector2 fLwrLc = Vector2Subtract(fLwfLc,tireSide);
    Vector2 fLwrRc = Vector2Subtract(fLwfRc,tireSide);
    DrawLineV(fLwrLc,fLwrRc,GRAY);
    DrawLineV(fLwfLc,fLwrLc,GRAY);
    DrawLineV(fLwfRc,fLwrRc,GRAY);

    //Drawing the front right tire by applying a simple translation to the left
    Vector2 translationL = Vector2Rotate((Vector2){0,2 * car->relativePositions.CtofLw.y},car->angle);

    Vector2 fRwfLc = Vector2Subtract(fLwfLc,translationL);
    Vector2 fRwfRc = Vector2Subtract(fLwfRc,translationL);
    Vector2 fRwrLc = Vector2Subtract(fLwrLc,translationL);
    Vector2 fRwrRc = Vector2Subtract(fLwrRc,translationL);
    DrawLineV(fRwfLc,fRwfRc,GRAY);
    DrawLineV(fRwrLc,fRwrRc,GRAY);
    DrawLineV(fRwfLc,fRwrLc,GRAY);
    DrawLineV(fRwfRc,fRwrRc,GRAY);

    //Drawing the rear left tire
    float RwBis = car->wheels.RwheelWidth / 2;
    float RwRadius = car->wheels.RwheelRadius;

    Vector2 rLwfLc = Vector2Add(car->wheels.RwheelCenter,Vector2Rotate((Vector2){RwRadius,RwBis},car->angle));
    Vector2 rLwfRc = Vector2Add(car->wheels.RwheelCenter,Vector2Rotate((Vector2){RwRadius,(-1)*RwBis},car->angle));
    Vector2 rLwrLc = Vector2Subtract(car->wheels.RwheelCenter,Vector2Rotate((Vector2){RwRadius,(-1)*RwBis},car->angle));
    Vector2 rLwrRc = Vector2Subtract(car->wheels.RwheelCenter,Vector2Rotate((Vector2){RwRadius,RwBis},car->angle));

    DrawLineV(rLwfLc,rLwfRc,GRAY);
    DrawLineV(rLwfLc,rLwrLc,GRAY);
    DrawLineV(rLwrLc,rLwrRc,GRAY);
    DrawLineV(rLwfRc,rLwrRc,GRAY);

    //Drawing the rear right tire by translating the left one
    Vector2 translationR = Vector2Rotate((Vector2){0,2 * car->relativePositions.CtorLw.y},car->angle);

    Vector2 rRwfLc = Vector2Subtract(rLwfLc,translationR);
    Vector2 rRwfRc = Vector2Subtract(rLwfRc,translationR);
    Vector2 rRwrLc = Vector2Subtract(rLwrLc,translationR);
    Vector2 rRwrRc = Vector2Subtract(rLwrRc,translationR);

    DrawLineV(rRwfLc,rRwfRc,GRAY);
    DrawLineV(rRwrLc,rRwrRc,GRAY);
    DrawLineV(rRwrLc,rRwfLc,GRAY);
    DrawLineV(rRwrRc,rRwfRc,GRAY);


}

void display_body(const car* car) {
    const Vector2 fl = car->body.frontLeft;
    const Vector2 fr = car->body.frontRight;
    const Vector2 br = car->body.rearRight;
    const Vector2 bl = car->body.rearLeft;
    const Vector2 c = car->centerPos;


    DrawLineV(fl,fr,BLACK);
    DrawLineV(br,bl,BLACK);
    DrawLineV(fr,br,BLACK);

    DrawLineV(fl,bl,BLACK);
    //DrawLineV(c,fl,RED);

    DrawPixelV(c,MAGENTA);


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
    car->mechanics.mass = 1473; //kg

    car->wheels.FwheelWidth = 13;
    car->wheels.FwheelAngle = 0;
    car->wheels.FwheelRadius = 13;
    car->wheels.FwheelCenter = (Vector2){0,0};
    car->relativePositions.CtofLw = (Vector2){34,-28};

    car->wheels.RwheelWidth = 13;
    car->wheels.RwheelRadius = 13;
    car->relativePositions.CtorLw = (Vector2){-31,-35};

    float wheelBaseLength = (float)fabsf(car->relativePositions.CtofLw.x - car->relativePositions.CtorLw.x);
    car->wheels.FaxleMass = (fabsf(car->relativePositions.CtorLw.x) / wheelBaseLength) * car->mechanics.mass;
    car->wheels.RaxleMass = (fabsf(car->relativePositions.CtofLw.x) / wheelBaseLength) * car->mechanics.mass;

    return car;
}

void camera_follow(car* car,Camera2D* camera) {
    Vector2 position = Vector2Add(car->centerPos,Vector2Scale(car->mechanics.speed,0.01f));
    camera->target = position;
}