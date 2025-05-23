#include <iostream>
#include <array>
#include "raylib.h"
#include "raymath.h"
#include "Bounds.h"
#include "GridBins.h"
#include "Boid.h"

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera free");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3{ 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    Bounds bounds = { Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 100.0f, 100.0f, 100.0f } };
	GridBins gridBins(bounds, 10);
	const int spawnCount = 5000;

	std::array<Vector3, spawnCount> tempBoids;
	std::array<Boid, spawnCount> boids;

    for (int i = 0; i < spawnCount; i++)
    {
		Vector3 pos = 
        { 
            GetRandomValue(bounds.Min().x, bounds.Max().x), 
            GetRandomValue(bounds.Min().y, bounds.Max().y),
            GetRandomValue(bounds.Min().z, bounds.Max().z)
        };

        Vector3 vel = 
        {
            GetRandomValue(-1, 1),
            GetRandomValue(-1, 1),
            GetRandomValue(-1, 1)
        };

        vel = Vector3Normalize(vel);
		vel = Vector3Scale(vel, Boid::maxSpeed);

		tempBoids[i] = pos;
		boids[i] = Boid{ pos, vel};
    }


    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(144);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed('Z')) camera.target = Vector3{ 0.0f, 0.0f, 0.0f };

        // Move Boids
        for (int i = 0; i < spawnCount; i++)
        {
            Boid b = boids[i];
			Vector3 pos = b.position;
            Vector3 vel = b.velocity;
			b.position += vel * GetFrameTime();
			boids[i] = b;
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        for (int i = 0; i < spawnCount; i++)
        {
			Vector3 pos = boids[i].position;
			Vector2 screenPos = GetWorldToScreen(pos, camera);
			Vector2 screenSize = GetWorldToScreenEx(pos, camera, 10, 10);

            DrawCircle3D(pos, .5f, Vector3{ 0.0f, 1.0f, 0.0f }, 
                (float)GetTime() * 180.0f + i, BLUE);
        }

        DrawGrid(gridBins.Density(), gridBins.BinSize().x);

        EndMode3D();

        DrawRectangle(10, 10, 320, 93, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(10, 10, 320, 93, BLUE);

        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)", 40, 80, 10, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
