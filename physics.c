//
// Created by gabri on 07/07/2025.
//
#include <stdio.h>
#include "physics.h"


#include "raylib.h"
#include "raymath.h"
#include "vectUtils.h"

//these constants will need finetuning to work
#define CDRAG  0.4257
#define CRR 12.8
#define CBRAKE 5000
#define PI 3.14159265f
#define G 9.81

float radian_to_degree(float radian) {
    return radian * 180 / PI;
}

float degree_to_radian(float degree) {
    return degree * PI / 180;
}

float atand (float angle) {
    return radian_to_degree((float)atan((double)degree_to_radian(angle)));
}

float sind (float angle) {
    return (float)sin((double)degree_to_radian(angle));
}

/*
Returns drag force as a 2D vector, using a pointer to a car structure and the drag constant Cdrag
Here we are using a quadratic drag
 */
Vector2 compute_drag(car* car, const float Cdrag) {
    Vector2 drag = Vector2Scale(car->mechanics.speed,(-1)*Cdrag * Vector2Length(car->mechanics.speed));
    return drag;
};

/*
Returns all rolling resistances combined as a 2D vector, using pointer to a car structure and the rolling resistance constant Crr
This resistance is considered linear
The resistance is the sum of both axle
Crr should be approximately 30* larger than Cdrag
 */
Vector2 compute_rolling_resistance(car* car, const float Crr) {
    Vector2 facing = get_facing_vector(car);
    //Rear axle
    float longitudinalSpeed = Vector2DotProduct(car->mechanics.speed,facing);
    Vector2 RolResRear = Vector2Scale(facing,(-1)*Crr*get_rear_weight_ratio(car)*longitudinalSpeed);

    //Front axle
    Vector2 FwheelUnit = Vector2Normalize(Vector2Rotate(facing,car->wheels.FwheelAngle));
    Vector2 RolResFront =Vector2Scale(FwheelUnit,(-1)*Crr*get_front_weight_ratio(car)*Vector2DotProduct(FwheelUnit,car->mechanics.speed));


    return Vector2Add(RolResRear,RolResFront);
}

/*
Returns traction force as a 2D vector using a car structure pointer
Currently using a constant as engine power
The car is considered rwd as the traction applies towards the nose of the car
 */
Vector2 compute_traction(car* car) {
    Vector2 traction = Vector2Zero();
    Vector2 breaking = Vector2Zero();
    if (IsKeyDown(KEY_DOWN) && Vector2Length(car->mechanics.speed) > 0.f ) {
        breaking = Vector2Scale(car->mechanics.speed,(-1)*CBRAKE);
        printf("Braking");
    }
    if (IsKeyDown(KEY_UP)) {
        traction = Vector2Scale(get_facing_vector(car),1000000); //TODO use the car engine power instead of a total constant
    }
   return Vector2Add(traction, breaking);
}

Vector2 compute_lateral_force(car* car) {
    Vector2 facing = get_facing_vector(car);
    //Front axle
    Vector2 FwheelUnit = Vector2Normalize(Vector2Rotate(facing,car->wheels.FwheelAngle)); // Unit vector for where the front wheels are pointed at
    float FrontSlipAngle = radian_to_degree(Vector2Angle(car->mechanics.speed,FwheelUnit));
    float FrontLoad = car->wheels.FaxleMass * G;
    float Ff = simplified_magic_formula(10.f,1.3f,1.f,0.97f,FrontSlipAngle,FrontLoad); //Value in Newtons

    Vector2 FwheelOrtho = Vector2Rotate(FwheelUnit,PI/2);
    Vector2 FfVec = Vector2Scale(FwheelOrtho,Ff);

    //Rear axle
    float RearSlipAngle = radian_to_degree(Vector2Angle(car->mechanics.speed,facing));
    float RearLoad = car->wheels.FaxleMass * G;
    float Rf = simplified_magic_formula(10.f,1.3f,1.f,0.97f,RearSlipAngle,RearLoad);

    Vector2 RwheelOrtho = Vector2Rotate(facing,PI/2);
    Vector2 RfVec = Vector2Scale(RwheelOrtho,Rf);

    Vector2 res = Vector2Add(FfVec,RfVec);
    return Vector2Scale(res,50);
}
/*
Computes lateral forces with a simplified "magic" Pacejka formula
 */
float simplified_magic_formula (float stiffness, float shape, float peak, float curvature, float angle,float weightLoad) {
    return weightLoad * peak * sind(shape * atand(stiffness*angle - curvature * (stiffness*angle - atand(stiffness*angle))));
}

/*
Returns acceleration as a 2D vector using Newton's 2nd law
!!! also changes the car's acceleration vector in its mechanics
 */
Vector2 compute_acceleration(car* car) {
    Vector2 acceleration = compute_traction(car);
    acceleration = Vector2Add(acceleration,compute_drag(car,CDRAG));
    acceleration = Vector2Add(acceleration,compute_rolling_resistance(car,CRR));
    acceleration = Vector2Add(acceleration,compute_lateral_force(car));
    acceleration = Vector2Scale(acceleration,1/(car->mechanics.mass)); //Mass shouldn't be zero
    car->mechanics.acceleration = acceleration;
    return acceleration;
}

/*
Updates weight distribution with the car's acceleration
*/
void update_weight_distrib(car* car) {
    float wheelBaseLength = fabsf(car->relativePositions.CtofLw.x - car->relativePositions.CtorLw.x);

    float accDistrib = Vector2DotProduct(car->mechanics.acceleration, get_facing_vector(car)) * car->mechanics.mass * ((car->wheels.RwheelRadius * 2) /wheelBaseLength); //The height of the center of mass is considered equal to the rear tire height
    car->wheels.FaxleMass = fabsf(car->relativePositions.CtorLw.x / wheelBaseLength) * car->mechanics.mass - accDistrib;
    car->wheels.RaxleMass = fabsf(car->relativePositions.CtofLw.x / wheelBaseLength) * car->mechanics.mass + accDistrib;
}

float get_front_weight_ratio(car* car) {
    return car->wheels.FaxleMass / car->mechanics.mass;
}

float get_rear_weight_ratio(car* car) {
    return car->wheels.RaxleMass / car->mechanics.mass;
}

/*
Changes the car's speed vector based on its acceleration and framerate
Also calls update_weight_distrib with said acceleration
 */
void apply_acceleration(car* car, int framerate) {
    car->mechanics.speed = Vector2Add(car->mechanics.speed,
                                    Vector2Scale(car->mechanics.acceleration,(1 / (float)framerate)));
    //update_weight_distrib(car);
}

/*
Changes the car's position based on it's speed
 */
void update_position(car* car, int framerate) {
    car->centerPos = Vector2Add(car->centerPos,
                    Vector2Scale(car->mechanics.speed,(1 / (float)framerate)));
}