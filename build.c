//
// Created by gabri on 29/08/2025.
//

#include "build.h"

#include <stdio.h>
#include <stdlib.h>

#include "raymath.h"
#include "vectUtils.h"

void draw_body(carBuild* build) {
    DrawLineV(build->body.frontLeft,build->body.frontRight,BLACK);
    DrawLineV(build->body.rearLeft,build->body.frontLeft,BLACK);
    DrawLineV(build->body.rearLeft,build->body.rearRight,BLACK);
    DrawLineV(build->body.rearRight,build->body.frontRight,BLACK);

    if (build->selection == 1) {
        DrawCircleV(build->body.frontLeft,10,GREEN);
        DrawCircleV(build->body.rearLeft,10,GREEN);

    }
}

void draw_wheels(carBuild* build) {
    draw_rec_from_center(build->wheels.Fcenter,build->wheels.FwheelRadius*2,build->wheels.FwheelWidth,GRAY);
    draw_rec_from_center(build->wheels.Rcenter,build->wheels.RwheelRadius*2,build->wheels.RwheelWidth,GRAY);

    draw_rec_from_center(build->wheels.FcenterSym,build->wheels.FwheelRadius*2,build->wheels.FwheelWidth,GRAY);
    draw_rec_from_center(build->wheels.RcenterSym,build->wheels.RwheelRadius*2,build->wheels.RwheelWidth,GRAY);

    if(build->selection == 2) {
        DrawCircleV(build->wheels.Fcenter,10,GREEN);
        DrawCircleV(build->wheels.Rcenter,10,GREEN);
    }
}

//TODO only call init_bodypos when a change is made
void  init_bodypos(carBuild* build,Vector2 center) {
    build->body.frontLeft =  Vector2Add(build->positionFromCenter.Fl,center);
    build->body.frontRight =  Vector2Add(symmetric(build->positionFromCenter.Fl),center);

    build->body.rearLeft =  Vector2Add(build->positionFromCenter.Rl,center);
    build->body.rearRight = Vector2Add(symmetric(build->positionFromCenter.Rl),center);

    build->wheels.Fcenter =  Vector2Add(build->positionFromCenter.fT,center);
    build->wheels.Rcenter =  Vector2Add(build->positionFromCenter.rT,center);

    build->wheels.FcenterSym = Vector2Add(symmetric(build->positionFromCenter.fT),center);
    build->wheels.RcenterSym = Vector2Add(symmetric(build->positionFromCenter.rT),center);
}

void prevent_illegal(carBuild* build,Vector2 center) {
    if (build->positionFromCenter.Fl.y >= -1) {
        build->positionFromCenter.Fl.y = -1;
    }
    if (build->positionFromCenter.Rl.y >= -1) {
        build->positionFromCenter.Rl.y = -1;
    }

    if (build->positionFromCenter.Fl.x <= build->positionFromCenter.Rl.x) {
        build->positionFromCenter.Fl.x = build->positionFromCenter.Rl.x + 1.f;
    }

    if (build->positionFromCenter.fT.y >= -1) {
        build->positionFromCenter.fT.y = -1;
    }
    if (build->positionFromCenter.rT.y >= -1) {
        build->positionFromCenter.rT.y = -1;
    }
}

void check_buttons(carBuild* build) {
    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos,(Rectangle){30,100,100,40})) { //Body button
        if (build->selection == 1) {
            build ->selection = 0;
        } else {
            build->selection = 1;
        }
    }
    else if (CheckCollisionPointRec(mousePos,(Rectangle){30,160,140,40})) { //Body button
        if (build->selection == 2) {
            build ->selection = 0;
        } else {
            build->selection = 2;
        }
    }
}

car* convert_to_car(carBuild* build,car* car) {

    car->relativePositions.CtoFl = Vector2Scale(build->positionFromCenter.Fl,0.5f);
    car->relativePositions.FlToRl = Vector2Subtract(Vector2Scale(build->positionFromCenter.Rl,0.5f),Vector2Scale(build->positionFromCenter.Fl,0.5f));
    car->relativePositions.CtofLw = Vector2Scale(build->positionFromCenter.fT,0.5f);
    car->relativePositions.CtorLw = Vector2Scale(build->positionFromCenter.rT,0.5f);

    car->body.frontLeft = Vector2Zero();
    car->body.frontRight = Vector2Zero();
    car->body.rearRight = Vector2Zero();
    car->body.rearLeft = Vector2Zero();

    compute_body_positions(car);

    car->wheels.FwheelRadius = build->wheels.FwheelRadius / 2;
    car->wheels.RwheelRadius = build->wheels.RwheelRadius / 2;

    car->wheels.FwheelWidth = build->wheels.FwheelWidth / 2;
    car->wheels.RwheelWidth = build->wheels.RwheelWidth / 2;

    car->wheels.FwheelAngle = 0.f;
    car->wheels.drifting = false;

    car->mechanics.mass = 1200.f;
    car->mechanics.speed = Vector2Zero();
    car->mechanics.acceleration = Vector2Zero();
    car->mechanics.maxRPM = 6300;
    car->centerPos = Vector2Zero();
    car->mechanics.rotationAcceleration = 0.f;
    car->mechanics.rotationSpeed = 0.f;

    car->wheels.prevLeftDrift = (Vector2){-1,-1};
    car->wheels.prevRightDrift = (Vector2){-1,-1};

    float wheelBaseLength = (float)fabsf(car->relativePositions.CtofLw.x - car->relativePositions.CtorLw.x);
    printf("Car length %f m \n",wheelBaseLength/20.f); //20 pxl equals a meter currently
    car->wheels.FaxleMass = (fabsf(car->relativePositions.CtorLw.x) / wheelBaseLength) * car->mechanics.mass;
    car->wheels.RaxleMass = (fabsf(car->relativePositions.CtofLw.x) / wheelBaseLength) * car->mechanics.mass;


    return car;

}