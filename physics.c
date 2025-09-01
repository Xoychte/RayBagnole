//
// Created by gabri on 07/07/2025.
//

#include "physics.h"

#include <io.h>
#include <stdio.h>


#include "raylib.h"
#include "raymath.h"
#include "vectUtils.h"

//these constants will need finetuning to work
#define CDRAG  0.2257 //Base 0.4257
#define CRR 12.8
#define CBRAKE 5000
#define PI 3.14159265f
#define G 9.81f

float radian_to_degree(const float radian) {
    return radian * 180 / PI;
}

float degree_to_radian(const float degree) {
    return degree * PI / 180;
}

float atand (const float angle) {
    return radian_to_degree((float)atan((double)degree_to_radian(angle)));
}

float sind (const float angle) {
    return (float)sin((double)degree_to_radian(angle));
}

/*
Returns drag force as a 2D vector, using a pointer to a car structure and the drag constant Cdrag
Here we are using a quadratic drag
 */
Vector2 compute_drag(const car* car, const float Cdrag) {
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
    const Vector2 facing = get_facing_vector(car);
    //Rear axle
    const float longitudinalSpeed = Vector2DotProduct(car->mechanics.speed,facing);
    const Vector2 RolResRear = Vector2Scale(facing,(-1)*Crr*get_rear_weight_ratio(car)*longitudinalSpeed);

    //Front axle
    const Vector2 FwheelUnit = Vector2Normalize(Vector2Rotate(facing,car->wheels.FwheelAngle));
    const Vector2 RolResFront =Vector2Scale(FwheelUnit,(-1)*Crr*get_front_weight_ratio(car)*Vector2DotProduct(FwheelUnit,car->mechanics.speed));

    //moment of RolResFront (rear moment is 0)
    Vector2 FaxleCenter = Vector2Add(car->centerPos,Vector2Rotate((Vector2){car->relativePositions.CtofLw.x,0},car->angle));
    float RolResFrontM = compute_moment(car,RolResFront,FaxleCenter);
    car->mechanics.rotationAcceleration -= RolResFrontM * 2;

    if(IsKeyDown(KEY_SPACE)) {
        printf("Moment %f\n",RolResFrontM);
    }

    return Vector2Add(RolResRear,RolResFront);
}


/*
 * Slows the car rotation because of the tires/air resistance
 */
void rotation_resistance(car* car) {
    if (fabsf(car->mechanics.rotationSpeed) < 0.01f) {
        car->mechanics.rotationSpeed = 0.0f;
    } else {
        car->mechanics.rotationSpeed *= 0.8f;
    }
}

/*
Returns traction force as a 2D vector using a car structure pointer
Currently using a constant as engine power
The car is considered rwd as the traction applies towards the nose of the car
 */
Vector2 compute_traction(const car* car) {
    Vector2 traction = Vector2Zero();
    Vector2 breaking = Vector2Zero();
    if ((IsKeyDown(KEY_S) || IsKeyDown(KEY_SPACE)) && Vector2Length(car->mechanics.speed) > 0.f ) {
        breaking = Vector2Scale(car->mechanics.speed,(-1)*CBRAKE);
    }
    if (IsKeyDown(KEY_W)) {
        if (car->mechanics.engineRPM < car->mechanics.maxRPM) {
            traction = compute_traction_v2(car,car->mechanics.gear);
        } else { //Hit the rev limiter
            traction = Vector2Zero();
        }
    }
   return Vector2Add(traction, breaking);
}

Vector2 compute_traction_v2(const car* car,int gear) {
    const float xd = 3.42f; //Differential ratio
    const float n = 0.7f; //Transmission efficiency
    const float testTorque = 480.f; // in Newton meters
    const Vector2 Fdrive = Vector2Scale(get_facing_vector(car), 20000 * testTorque * get_gear_ratio(car,gear) * xd * n / car->wheels.RwheelRadius);
    return Fdrive;
}

float get_torque(const car* car) {
    return 0.f;
}

float get_gear_ratio(const car* car,int gear) {
    switch (gear) {
        case 0:
            return 0;
        case 1:
            return 2.66f;
        case 2:
            return 1.78f;
        case 3:
            return 1.3f;
        case 4:
            return 1.f;
        case 5:
            return 0.74f;
        case 6:
            return 0.5f;
        case -1: // Reverse gear
            return -2.9f;
        default: return 0;
    }
}

Vector2 compute_lateral_force(car* car) {
    const Vector2 facing = get_facing_vector(car);
    //Front axle
    Vector2 FwheelUnit = Vector2Normalize(Vector2Rotate(facing,car->wheels.FwheelAngle)); // Unit vector for where the front wheels are pointed at
    float FrontSlipAngle = radian_to_degree(Vector2Angle(car->mechanics.speed,FwheelUnit));

    if(fabsf(FrontSlipAngle) < 1.f) {
        FrontSlipAngle = 0.f;
    }

    const float FrontLoad = car->wheels.FaxleMass * G;
    float Ff= simplified_magic_formula(10.f,1.3f,1.f,0.97f,FrontSlipAngle,FrontLoad); //Value in Newtons



    const Vector2 FwheelOrtho = Vector2Rotate(FwheelUnit,PI/2);
    Vector2 FfVec = Vector2Scale(FwheelOrtho,Ff);

    //Rear axle
    float RearSlipAngle = radian_to_degree(Vector2Angle(car->mechanics.speed,facing));

    if (fabsf(RearSlipAngle) < 1.f) {
        RearSlipAngle = 0.f;
    }
    const float DriftAngle = fminf(fabsf(RearSlipAngle),fabsf(radian_to_degree(Vector2Angle(car->mechanics.speed,Vector2Rotate(facing,PI)))));
    if (fabsf(DriftAngle) > 20.f && Vector2Length(car->mechanics.speed) > 500.f) {
        car->wheels.drifting = true;
    } else {
        car->wheels.drifting = false;
    }

    const float RearLoad = (float)car->wheels.FaxleMass * G;
    const float Rf = simplified_magic_formula(10.f,1.3f,1.f,0.97f,RearSlipAngle,RearLoad);

    const Vector2 RwheelOrtho = Vector2Rotate(facing,PI/2);
    Vector2 RfVec = Vector2Scale(RwheelOrtho,Rf);



    //Should be refactored elsewhere but we compute moments of both parts here to add to rotAccel

    Vector2 FaxleCenter = Vector2Add(car->centerPos,Vector2Rotate((Vector2){car->relativePositions.CtofLw.x,0},car->angle));
    Vector2 RaxleCenter = Vector2Add(car->centerPos,Vector2Rotate((Vector2){car->relativePositions.CtorLw.x,0},car->angle));

    float FfvecM = compute_moment(car,FfVec,FaxleCenter);
    float RfVecM = compute_moment(car,RfVec,RaxleCenter);

    car->mechanics.rotationAcceleration +=  RfVecM * 1.1f + FfvecM * 1.1f ;


    if (car->wheels.drifting) {
        FfVec = Vector2Scale(FfVec,70);
        RfVec = Vector2Scale(RfVec,70);

    } else {
        FfVec = Vector2Scale(FfVec,175);
        RfVec = Vector2Scale(RfVec,175);
    }


    const Vector2 res = Vector2Add(FfVec,RfVec);
    return res;
}
/*
Computes lateral forces with a simplified "magic" Pacejka formula
 */
float simplified_magic_formula (float stiffness, float shape, float peak, float curvature, float angle,float weightLoad) {
    return weightLoad * peak * sind(shape * atand(stiffness*angle - curvature * (stiffness*angle - atand(stiffness*angle))));
}

/*
 changes the car's acceleration vector in its mechanics using 2nd law

 */
void compute_acceleration(car* car) {
    car->mechanics.rotationAcceleration = 0;

    Vector2 acceleration = compute_traction(car);

    Vector2 drag = compute_drag(car,CDRAG);
    acceleration = Vector2Add(acceleration,drag);

    Vector2 rollingResistance = compute_rolling_resistance(car,CRR);
    acceleration = Vector2Add(acceleration,rollingResistance);

    Vector2 lateralForce = compute_lateral_force(car);
    acceleration = Vector2Add(acceleration,lateralForce);

    acceleration = Vector2Scale(acceleration,1/(car->mechanics.mass)); //Mass shouldn't be zero
    car->mechanics.acceleration = acceleration;

    rotation_resistance(car);
    car->mechanics.rotationAcceleration *= 1/car->mechanics.mass;

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
Changes the rotation speed with rotational acceleration
 */
void apply_acceleration(car* car, int framerate) {
    car->mechanics.speed = Vector2Add(car->mechanics.speed,
                                    Vector2Scale(car->mechanics.acceleration,(1 / (float)framerate)));

    car->mechanics.rotationSpeed += car->mechanics.rotationAcceleration * (1/(float)framerate);
    //update_weight_distrib(car);
}

/*
Changes the car's position based on it's speed
also rotates the car with its rotational speed
 */
void update_position(car* car, int framerate) {
    Vector2 delta = Vector2Scale(car->mechanics.speed,(1 / (float)framerate));
    car->centerPos = Vector2Add(car->centerPos,delta);
    car->angle += car->mechanics.rotationSpeed * (1/(float)framerate);
}

/*
Assumes the wheels aren't spinning for now to get the engine speed
*/
int get_rpm_from_speed(car* car) {
    float speed = get_speedometer(car);
    float wheelRotationRate = (speed * 30 * 20)/(PI * car->wheels.RwheelRadius);
    int rpm = (int)(wheelRotationRate * get_gear_ratio(car,car->mechanics.gear) * 3.42f);
    car->mechanics.engineRPM = rpm;
    return rpm;

}

float compute_moment(const car* car,Vector2 force, Vector2 application) {
    return (-1) * multiply(force,Vector2Subtract(application,car->centerPos));
}


