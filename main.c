#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "car.h"
#include "raymath.h"
#include "physics.h"
#include "rlgl.h"
#include "vectUtils.h"

int main(void) {
    printf("Starting le game\n");

    InitWindow(1500, 1000, "RayBagnole");
    SetTargetFPS(60);

    car* car = create_le_car();

    Camera2D camera = { 0 };
    camera.target = (Vector2){ car->centerPos.x + 20.0f, car->centerPos.y + 20.0f };
    camera.offset = (Vector2){ 1500/2.0f, 1000/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


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

        if (IsKeyPressed(KEY_ENTER)) {
            car->angle += 3.14f;
        }
        if (IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_LEFT)) {
            if (fabs(car->wheels.FwheelAngle) <= 0.1f) {
                car->wheels.FwheelAngle = 0.f;
            } else {
                car->wheels.FwheelAngle /= 1.3f;
            }

        }

        if (IsKeyDown(KEY_A)) {
            printf(" Front %f , Rear %f\n",get_front_weight_ratio(car), get_rear_weight_ratio(car));
        }



        //Handling display

        BeginDrawing();
        BeginMode2D(camera);


        DrawFPS(0, 0);
        ClearBackground(RAYWHITE);

        rlPushMatrix();
        rlTranslatef(0, 25*50, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, 50);
        rlPopMatrix();
        display_body(car);
        display_wheels(car);


        camera_follow(car,&camera);


        EndMode2D();
        EndDrawing();

        //Updating forces and positions
        Vector2 acc = compute_acceleration(car);
        apply_acceleration(car,60);
        update_position(car,60);


    }
    free(car);
    return 0;
}
