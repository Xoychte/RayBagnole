//
// Created by gabri on 03/07/2025.
//

#include "car.h"
#include "vectUtils.h"

void compute_body_positions(car* car) {

}

void display_car(car* car) {
    Vector2 fl = car->body.frontLeft;
    Vector2 fr = car->body.frontRight;
    Vector2 br = car->body.backRight;
    Vector2 bl = car->body.backLeft;
    Vector2 c = car->centerPos;

    DrawLineV(fl,fr,BLACK);
    DrawLineV(br,bl,BLACK);
    DrawLineV(fr,br,BLACK);
    DrawLineV(fl,bl,BLACK);
    DrawPixelV(c,BLACK);
}