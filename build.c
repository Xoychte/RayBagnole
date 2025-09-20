//
// Created by gabri on 29/08/2025.
//

#include "build.h"

#include "raymath.h"
#include "vectUtils.h"

void draw_body(carBuild* build) {
    DrawLineV(build->body.frontLeft,build->body.frontRight,GRAY);
    DrawLineV(build->body.rearLeft,build->body.frontLeft,GRAY);
    DrawLineV(build->body.rearLeft,build->body.rearRight,GRAY);
    DrawLineV(build->body.rearRight,build->body.frontRight,GRAY);

    if (build->selection == 1) {
        DrawCircleV(build->body.frontLeft,10,GREEN);
        DrawCircleV(build->body.rearLeft,10,GREEN);

    }
}

void draw_wheels(carBuild* build) {
    draw_rec_from_center(build->wheels.Fcenter,build->wheels.FwheelRadius*2,build->wheels.FwheelWidth,GRAY);
    draw_rec_from_center(build->wheels.Rcenter,build->wheels.RwheelRadius*2,build->wheels.RwheelWidth,GRAY);

}

void  init_bodypos(carBuild* build,Vector2 center) {
    build->body.frontLeft =  Vector2Add(build->positionFromCenter.Fl,center);
    build->body.frontRight =  Vector2Add(symmetric(build->positionFromCenter.Fl),center);

    build->body.rearLeft =  Vector2Add(build->positionFromCenter.Rl,center);
    build->body.rearRight = Vector2Add(symmetric(build->positionFromCenter.Rl),center);

    build->wheels.Fcenter =  Vector2Add(build->positionFromCenter.fT,center);
    build->wheels.Rcenter =  Vector2Add(build->positionFromCenter.rT,center);
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
}