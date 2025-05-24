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
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib boids - Keith Lerner");

    Bounds bounds = { Vector3{ 0.0f, 0.0f, 0.0f }, Vector3One() * 360 };
    const int spawnCount = 1000;

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = bounds.Max() + bounds.Extents();  // Camera position
    camera.target = bounds.Center();                    // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };            // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Spawn boids for management
    GridBins<Boid> gridBins = GridBins<Boid>(bounds, 2);
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

		boids[i] = Boid{ i, pos, vel};
    }


    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(200);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsKeyPressed('Z')) 
        {
            camera.position = bounds.Max() + (bounds.Extents() / 4);
            camera.target = bounds.Center();
        }

        // Update Boids
        for (int i = 0; i < spawnCount; i++)
		{
			Boid boid = boids[i];
            Vector3 pos = boid.position;

            /*int binArrayIndex = gridBins.WorldPosToVectorIndex(pos);
			if (binArrayIndex < 0) continue;*/

            std::vector<Boid> neighbors = std::vector<Boid>{}; // THROWS INDEX OUT OF RANGE

            for (size_t j = 0; j < boids.size(); j++)
            {
                if (boids[j].id == boid.id) continue;

                if (Vector3Distance(boids[j].position, boid.position) <=
                    boid.senseDistance)
                    neighbors.push_back(boids[j]);
            }

            /*std::vector<int> searchBins = gridBins.GetNeighborBinIndices(binArrayIndex);

            for (size_t j = 0; j < searchBins.size(); j++)
            {
				int arrayIndex = searchBins[j];

                std::vector<Boid> binBoids = gridBins.Bins()[arrayIndex];
                for (size_t k = 0; k < binBoids.size(); k++)
                {
					if (binBoids[k].id == boid.id) continue;

					if (Vector3Distance(binBoids[k].position, boid.position) <= 
                        boid.senseDistance)
						neighbors.push_back(binBoids[k]);
                }
            }*/

			// Update the boid's data
			boids[i].Movement(neighbors, bounds);
			boids[i].FixToBounds(bounds);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(Color{ 35, 35, 35, 255 });

        BeginMode3D(camera);

        for (int i = 0; i < spawnCount; i++)
        {
			Vector3 pos = boids[i].position;
			Vector3 vel = boids[i].velocity;

            //Vector3 lineEnd = pos - Vector3Scale(Vector3Normalize(vel), GetFrameTime() * Boid::maxSpeed);
            //DrawLine3D(pos, lineEnd, RED);

			bool inBounds = bounds.Contains(pos);
			Vector3 normalizedVel = Vector3Normalize(vel);
			
			float r = Vector3DotProduct(normalizedVel, Vector3{ 1.0f, 0.0f, 0.0f });
			float g = Vector3DotProduct(normalizedVel, Vector3{ 0.0f, 1.0f, 0.0f });
			float b = Vector3DotProduct(normalizedVel, Vector3{ 0.0f, 0.0f, 1.0f });

            Color color = Color{ 
                (unsigned char)(r * 255), 
                (unsigned char)(g * 255),
                (unsigned char)(b * 255),
                255 };

            DrawCapsule(pos, pos - normalizedVel * 2.0f, 1.0f, 2, 4, 
                (inBounds ? color : BLACK));
        }

        //DrawGrid(gridBins.Density(), gridBins.BinSize().x);
        DrawCubeWiresV(bounds.Center(), bounds.Size(), Color{ 128, 128, 128, 128 });

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
