//
// Created by gabri on 07/07/2025.
//

#ifndef PHYSICS_H
#define PHYSICS_H
#include "car.h"
float radian_to_degree(float radian);
float degree_to_radian(float degree);
Vector2 compute_drag(const car*, const float);
Vector2 compute_rolling_resistance(car*, const float);
void rotation_resistance(car*);
Vector2 compute_traction(const car*);
Vector2 compute_traction_v2 (const car*,int);
float get_torque(const car*);
float get_gear_ratio(const car*,int);
Vector2 compute_lateral_force(car*);
float simplified_magic_formula(float,float,float,float,float,float);
void compute_acceleration(car*);
void update_weight_distrib(car*);
float get_front_weight_ratio(car*);
float get_rear_weight_ratio(car*);
void apply_acceleration(car*, int);
void update_position(car*,int);
int get_rpm_from_speed(car*);
float compute_moment(const car*,Vector2,Vector2);

#endif //PHYSICS_H
