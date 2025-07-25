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



    InitWindow(0, 0, "RayBagnole");

    const int FPS = 120;
    SetTargetFPS(FPS);
    const int ScreenWidth = GetScreenWidth();
    const int ScreenHeight = GetScreenHeight();
    printf("Screen size: %dx%d\n", ScreenWidth, ScreenHeight);

    car* car = create_le_car(ScreenHeight, ScreenWidth);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ car->centerPos.x, car->centerPos.y};
    camera.offset = (Vector2){ (float)ScreenWidth/2.0f, (float)ScreenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


    while (!WindowShouldClose()) {
        compute_body_positions(car);

        //Handling imputs
        if (IsKeyDown(KEY_A)) {
            car->angle -= 0.05f * 60/(float)FPS;
            if (car->wheels.FwheelAngle > -0.6f) {
                car->wheels.FwheelAngle -= 0.05f * 60/(float)FPS;
            }
        }
        if (IsKeyDown(KEY_D)) {
            car->angle += 0.05f * 60/(float)FPS;
            if (car->wheels.FwheelAngle < 0.6f) {
                car->wheels.FwheelAngle += 0.05f * 60/(float)FPS;
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
            car->angle += 3.14f;
        }
        if (IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) {
            if (fabs(car->wheels.FwheelAngle) <= 0.1f) {
                car->wheels.FwheelAngle = 0.f;
            } else {
                car->wheels.FwheelAngle /= 1.3f;
            }

        }
        shift_gears(car);




        //Handling display

        BeginDrawing();
        BeginMode2D(camera);



        ClearBackground(RAYWHITE);

        rlPushMatrix();
        rlTranslatef(0, 25*50, 0);
        rlRotatef(90, 1, 0, 0);
        DrawGrid(100, 50);
        rlPopMatrix();
        display_body(car);
        display_wheels(car);


        //Draw the UI to follow the camera
        DrawFPS((int)camera.target.x - ScreenWidth/ 2 + 20, (int)camera.target.y - ScreenHeight/2 + 20);

        DrawTextEx(GetFontDefault(), TextFormat("%.1f km/h", get_speedometer(car)),
                Vector2Add(camera.target,(Vector2){(float)ScreenWidth/2 - 300, (float)ScreenHeight/2 - 140}), 55, 2, BLACK);


        if (car->mechanics.gear == -1) {
            DrawTextEx(GetFontDefault(), "R",
               Vector2Add(camera.target,(Vector2){(float)ScreenWidth/2 - 300, (float)ScreenHeight/2 - 200}), 55, 2, BLACK);
        } else {
            DrawTextEx(GetFontDefault(), TextFormat("%d", car->mechanics.gear),
               Vector2Add(camera.target,(Vector2){(float)ScreenWidth/2 - 300, (float)ScreenHeight/2 - 200}), 55, 2, BLACK);


        }
        camera_follow(car,&camera);




        EndMode2D();
        EndDrawing();

        //Updating forces and positions
        Vector2 acc = compute_acceleration(car);
        apply_acceleration(car,FPS);
        update_position(car,FPS);


    }
    free(car);
    return 0;
}
