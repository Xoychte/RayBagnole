#include <stdio.h>
#include <stdlib.h>

#include "build.h"
#include "raylib.h"
#include "car.h"
#include "raymath.h"
#include "physics.h"
#include "rlgl.h"
#include "vectUtils.h"

typedef enum GameScreen {DRIVING = 0,BUILD} GameScreen;

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

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(0,0,"RayBagnole");
    int m = GetCurrentMonitor();
    const int ScreenWidth = GetMonitorWidth(m);
    const int ScreenHeight = GetMonitorHeight(m);
    printf("Screen size: %dx%d\n", ScreenWidth, ScreenHeight);

    GameScreen screen = DRIVING;

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

    //TODO remove this tests for build
    carBuild* build = (carBuild*)malloc(sizeof(carBuild));
    if (build == NULL) {
        printf("Failed to allocate memory for build\n");
        return 1;
    }
    build->selection = 2;
    Vector2 screenCenter = (Vector2){ (float)ScreenWidth/2.0f, (float)ScreenHeight/2.0f };
    build->positionFromCenter.Fl = (Vector2){ 200,-150};
    build->positionFromCenter.Rl = (Vector2){-250,-170};
    init_bodypos(build,screenCenter);
    build->wheels.FwheelRadius = 40.f;
    build->wheels.RwheelRadius = 40.f;
    build->wheels.FwheelWidth = 40.f;
    build->wheels.RwheelWidth = 40.f;
    build->positionFromCenter.fT = (Vector2){ 150,-140};
    build->positionFromCenter.rT = (Vector2){-200,-160};
    init_bodypos(build,screenCenter);


    printf("Entering main loop\n");
    while (!WindowShouldClose()) {
        switch (screen) {
            case DRIVING: {
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

                if (IsKeyPressed(KEY_TAB)) {
                    screen = BUILD;
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
            } break;

            case BUILD: {
                build->updated = false;
                if (IsKeyPressed(KEY_TAB)) {
                    screen = DRIVING;
                }
                if (build->selection == 1) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(GetMousePosition(),build->body.frontLeft,50) && build->body.frontLeft.y < screenCenter.y) {
                        build->positionFromCenter.Fl = Vector2Add(build->positionFromCenter.Fl,GetMouseDelta());
                        build->updated = true;
                    }

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(GetMousePosition(),build->body.rearLeft,50) && build->body.rearLeft.y < screenCenter.y) {
                        build->positionFromCenter.Rl = Vector2Add(build->positionFromCenter.Rl,GetMouseDelta());
                        build->updated = true;
                    }
                }
                if (build->selection == 2) {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(GetMousePosition(),build->wheels.Fcenter,50)) {
                        build->positionFromCenter.fT = Vector2Add(build->positionFromCenter.fT,GetMouseDelta());
                        build->updated = true;
                    }

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(GetMousePosition(),build->wheels.Rcenter,50)) {
                        build->positionFromCenter.rT = Vector2Add(build->positionFromCenter.rT,GetMouseDelta());
                        build->updated = true;
                    }
                }
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    check_buttons(build);
                }

                if (build->updated) {
                    prevent_illegal(build,screenCenter);
                    init_bodypos(build,screenCenter);
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("BUILDING SCREEN", 20, 20, 40, MAROON);
                DrawText("Forward ->",screenCenter.x - 70, 20, 40, MAROON);
                DrawCircleV(GetMousePosition(),5,GREEN);

                //Buttons
                DrawRectangleRec((Rectangle){30,100,100,40},GRAY);
                DrawText("Body",30,100,40,DARKGRAY);

                DrawRectangleRec((Rectangle){30,160,140,40},GRAY);
                DrawText("Wheels",30,160,40,DARKGRAY);

                draw_body(build);
                draw_wheels(build);
                EndDrawing();

            }break;
            default:exit(EXIT_FAILURE);
        }

    }

    free(car);
    UnloadTexture(background);
    UnloadRenderTexture(target);
    return 0;
}
