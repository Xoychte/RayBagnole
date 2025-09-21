//
// Created by gabri on 29/08/2025.
//

#ifndef BUILD_H
#define BUILD_H
#include "car.h"

typedef struct positionFromCenter {
    Vector2 Fl;
    Vector2 Rl;
    Vector2 fT; //front tyre
    Vector2 rT;
} positionFromCenter;

typedef struct build_wheels {
    float FwheelRadius;
    float FwheelWidth;
    Vector2 Fcenter;
    Vector2 FcenterSym;

    float RwheelRadius;
    float RwheelWidth;
    Vector2 Rcenter;
    Vector2 RcenterSym;

} build_wheels;

typedef struct carBuild {
    bool updated;
    body body;
    build_wheels wheels;
    int selection;

    positionFromCenter positionFromCenter;
} carBuild;



void draw_body(carBuild *build);
void draw_wheels(carBuild *build);
void init_bodypos(carBuild *build,Vector2 center);
void prevent_illegal(carBuild *build,Vector2 center);
void check_buttons(carBuild *build);


#endif //BUILD_H
