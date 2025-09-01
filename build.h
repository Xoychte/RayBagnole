//
// Created by gabri on 29/08/2025.
//

#ifndef BUILD_H
#define BUILD_H
#include "car.h"

typedef struct positionFromCenter {
    Vector2 Fl;
    Vector2 Rl;
} positionFromCenter;

typedef struct carBuild {
    body body;
    bool isBodySelected;
    positionFromCenter positionFromCenter;
} carBuild;



void draw_body(carBuild *build);
void init_bodypos(carBuild *build,Vector2 center);
void prevent_illegal(carBuild *build,Vector2 center);
void check_buttons(carBuild *build);


#endif //BUILD_H
