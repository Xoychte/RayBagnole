//
// Created by gabri on 07/07/2025.
//

#ifndef PHYSICS_H
#define PHYSICS_H
#include "car.h"

Vector2 compute_drag(car*, const float);
Vector2 compute_rolling_resistance(car*, const float);
Vector2 compute_traction(car*);
Vector2 compute_acceleration(car*);
void update_weight_distrib(car*);
void apply_acceleration(car*, int);
void update_position(car*,int);

#endif //PHYSICS_H
