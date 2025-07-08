#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "car.h"
#include "raymath.h"
#include "physics.h"

int main(void) {
    printf("Starting le game\n");

    InitWindow(1500, 1000, "RayBagnole");
    SetTargetFPS(60);

    car* car = create_le_car();

    while (!WindowShouldClose()) {
        compute_body_positions(car);

        //Handling imputs
        if (IsKeyDown(KEY_LEFT)) {
            car->angle -= 0.05f;
            if (car->wheels.FwheelAngle > -0.6f) {
                car->wheels.FwheelAngle -= 0.05f;
            }
        }
        if (IsKeyDown(KEY_RIGHT)) {
            car->angle += 0.05f;
            if (car->wheels.FwheelAngle < 0.6f) {
                car->wheels.FwheelAngle += 0.05f;
            }
        }
        if (IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_LEFT)) {
            if (fabs(car->wheels.FwheelAngle) <= 0.1f) {
                car->wheels.FwheelAngle = 0.f;
            } else {
                car->wheels.FwheelAngle /= 1.3f;
            }

        }


        if (IsKeyDown(KEY_W)) {
            car->centerPos.y -= 1.f;
        }
        if (IsKeyDown(KEY_S)) {
            car->centerPos.y += 1.f;
        }
        if (IsKeyDown(KEY_A)) {
            car->centerPos.x -= 1.f;
        }
        if (IsKeyDown(KEY_D)) {
            car->centerPos.x += 1.f;
        }

        //Handling display

        BeginDrawing();
        DrawFPS(0, 0);
        ClearBackground(RAYWHITE);
        display_body(car);
        display_wheels(car);
        EndDrawing();


        //Updating forces and positions
        Vector2 acc = compute_acceleration(car);
        apply_acceleration(car,60);
        update_position(car,60);

    }
    free(car);
    return 0;
}
