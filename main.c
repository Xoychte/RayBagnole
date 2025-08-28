#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "car.h"
#include "raymath.h"
#include "physics.h"
#include "rlgl.h"


int main(void) {
    printf("Starting le game\n");

    //for web
    /*
    const int ScreenWidth = 1600;
    const int ScreenHeight = 900;

    InitWindow(ScreenWidth, ScreenHeight, "RayBagnole");
    */
    const int FPS = 200;
    SetTargetFPS(FPS);
    //for dektop

    InitWindow(0,0,"RayBagnole");
    int m = GetCurrentMonitor();
    const int ScreenWidth = GetMonitorWidth(m);
    const int ScreenHeight = GetMonitorHeight(m);
    printf("Screen size: %dx%d\n", ScreenWidth, ScreenHeight);

    car* car = create_le_car(ScreenHeight, ScreenWidth);

    Camera2D camera = { 0 };
    camera.target = (Vector2){ car->centerPos.x, car->centerPos.y};
    camera.offset = (Vector2){ (float)ScreenWidth/2.0f, (float)ScreenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


    //Testing background
    Image bcg = LoadImage("testmap.png");
    if (!IsImageValid(bcg)) {
        printf("Image is not valid\n");
        return 1;
    }
    Texture2D background = LoadTextureFromImage(bcg);
    UnloadImage(bcg);


    // Create a RenderTexture2D to use as a canvas, will be used to add the tires marks when drifting
    RenderTexture2D target = LoadRenderTexture(10000, 10000);

    // Clear render texture before entering the game loop
    BeginTextureMode(target);
    ClearBackground((Color){ 255, 255, 255, 0 }); //Transparent background
    EndTextureMode();

    printf("Entering main loop\n");
    while (!WindowShouldClose()) {
        compute_body_positions(car);

        int currentFPS = GetFPS();
        if (currentFPS == 0) { //Prevent dividing by zero
                currentFPS = FPS;
        }
        //Handling inputs
        if (IsKeyDown(KEY_A)) {
            if (car->wheels.FwheelAngle > -0.3f) {
                car->wheels.FwheelAngle -= 0.02f * 60/(float)currentFPS;
            }
        }
        if (IsKeyDown(KEY_D)) {
            if (car->wheels.FwheelAngle < 0.3f) {
                car->wheels.FwheelAngle += 0.02f * 60/(float)currentFPS;
            }
        }

        if (IsKeyPressed(KEY_R)) {
            car->centerPos = Vector2Zero();
            car->angle = 0.f;
        }
        if (IsKeyUp(KEY_A) && IsKeyUp(KEY_D)) {
            if (fabsf(car->wheels.FwheelAngle) <= 0.1f) {
                car->wheels.FwheelAngle = 0.f;
            } else {
                car->wheels.FwheelAngle -= 0.03f * (car->wheels.FwheelAngle/fabsf(car->wheels.FwheelAngle))* 200/(float)currentFPS ;
            }

        }
        shift_gears(car);
        shift_automatic(car);





        //Handling display
        BeginDrawing();
        BeginMode2D(camera);



        ClearBackground(RAYWHITE);
        DrawTexture(background,0,0,WHITE);
        DrawTextureRec(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2) { 0, 0 }, WHITE);



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

        //TODO refactor these in a draw_ui function
        if (car->mechanics.gear == -1) {
            DrawTextEx(GetFontDefault(), "R",
               Vector2Add(camera.target,(Vector2){(float)ScreenWidth/2 - 300, (float)ScreenHeight/2 - 200}), 55, 2, BLACK);
        } else if (car->mechanics.gear == 0) {
            DrawTextEx(GetFontDefault(), "N",
              Vector2Add(camera.target,(Vector2){(float)ScreenWidth/2 - 300, (float)ScreenHeight/2 - 200}), 55, 2, BLACK);
        } else {
            DrawTextEx(GetFontDefault(), TextFormat("%d", car->mechanics.gear),
               Vector2Add(camera.target,(Vector2){(float)ScreenWidth/2 - 300, (float)ScreenHeight/2 - 200}), 55, 2, BLACK);
        }

        //test
            DrawTextEx(GetFontDefault(), TextFormat("%.1f rad/s", car->mechanics.rotationSpeed),
                Vector2Add(camera.target,(Vector2){(float)ScreenWidth/2 - 300, (float)ScreenHeight/2 - 300}), 55, 2, BLACK);

        show_tachometer(car,&camera,ScreenWidth,ScreenHeight);


        camera_follow(car,&camera);

        show_drifting(car,&target);

        EndMode2D();
        EndDrawing();

        //Updating forces and positions

        compute_acceleration(car);
        apply_acceleration(car,currentFPS);
        update_position(car,currentFPS);
        int rpm = get_rpm_from_speed(car);



    }

    free(car);
    UnloadTexture(background);
    UnloadRenderTexture(target);
    return 0;
}
