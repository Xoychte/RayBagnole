//
// Created by gabri on 07/07/2025.
//

#include "physics.h"

#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

//these constants will need finetuning to work
#define CDRAG  0.4257
#define CRR 12.8
/*
Returns drag force as a 2D vector, using a pointer to a car structure and the drag constant Cdrag
Here we are using a quadratic drag
 */
Vector2 compute_drag(car* car, const float Cdrag) {
    Vector2 drag = Vector2Scale(car->mechanics.speed,(-1)*Cdrag * Vector2Length(car->mechanics.speed));
    return drag;
};

/*
Returns all rolling resistances combined as a 2D vector, using pointer to a car structure and the rolling resistance function Crr
This resistance is considered linear
Crr should be approximately 30* larger than Cdrag
 */
Vector2 compute_rolling_resistance(car* car, const float Crr) {
    Vector2 RolRes = Vector2Scale(car->mechanics.speed,(-1)*Crr);
    return RolRes;
}

/*
Returns traction force as a 2D vector using a car structure pointer
Currently using a constant as engine power
The car is considered rwd as the traction applies towards the nose of the car
 */
Vector2 compute_traction(car* car) {
    Vector2 traction;
    if (IsKeyDown(KEY_UP)) {
        traction = Vector2Scale(get_facing_vector(car),100000); //TODO use the car engine power instead of a total constant
    } else {
        traction = Vector2Zero();
    }
   return traction;
}

/*
Returns acceleration as a 2D vector using Newton's 2nd law
!!! also changes the car's acceleration vector in its mechanics
 */
Vector2 compute_acceleration(car* car) {
    Vector2 acceleration = compute_traction(car);
    acceleration = Vector2Add(acceleration,compute_drag(car,CDRAG));
    acceleration = Vector2Add(acceleration,compute_rolling_resistance(car,CRR));
    acceleration = Vector2Scale(acceleration,1/(car->mechanics.mass)); //Mass shouldn't be zero
    car->mechanics.acceleration = acceleration;
    return acceleration;
}

/*
Changes the car's speed vector based on its acceleration and framerate
 */
void apply_acceleration(car* car, int framerate) {
    car->mechanics.speed = Vector2Add(car->mechanics.speed,
                                    Vector2Scale(car->mechanics.acceleration,(1 / (float)framerate)));
}

/*
Changes the car's position based on it's speed
 */
void update_position(car* car, int framerate) {
    car->centerPos = Vector2Add(car->centerPos,
                    Vector2Scale(car->mechanics.speed,(1 / (float)framerate)));
}