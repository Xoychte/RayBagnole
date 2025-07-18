//
// Created by gabri on 07/07/2025.
//

#ifndef PHYSICS_H
#define PHYSICS_H
#include "car.h"
float radian_to_degree(float radian);
float degree_to_radian(float degree);
Vector2 compute_drag(car*, const float);
Vector2 compute_rolling_resistance(car*, const float);
Vector2 compute_traction(car*);
Vector2 compute_lateral_force(car*);
float simplified_magic_formula(float,float,float,float,float,float);
Vector2 compute_acceleration(car*);
void update_weight_distrib(car*);
float get_front_weight_ratio(car*);
float get_rear_weight_ratio(car*);
void apply_acceleration(car*, int);
void update_position(car*,int);

#endif //PHYSICS_H
