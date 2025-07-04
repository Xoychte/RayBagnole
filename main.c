#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "car.h"
#include "raymath.h"

int main(void) {
    printf("Starting le game\n");

    InitWindow(800, 600, "RayBagnole");
    SetTargetFPS(60);

    car* car = create_le_car();

    while (!WindowShouldClose()) {
        compute_body_positions(car);

        //Handling imputs
        if (IsKeyDown(KEY_LEFT)) {
            car->angle -= 0.05f;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            car->angle += 0.05f;
        }
        if (IsKeyDown(KEY_UP)) {
            car->mechanics.acceleration = get_facing_vector(car);
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

        display_car(car);
        EndDrawing();


        //Updating forces and positions

        car->centerPos = Vector2Add(car->centerPos, car->mechanics.speed);
        car->mechanics.speed = Vector2Add(car->mechanics.speed, car->mechanics.acceleration);

    }
    free(car);
    return 0;
}
