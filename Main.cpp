#include <iostream>
#include <array>
#include "raylib.h"
#include "raymath.h"
#include "Bounds.h"
#include "GridBins.h"
#include "Boid.h"
#include "Tests.h"

int main(int argc, char* argv[])
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib boids - Keith Lerner");

    Bounds bounds = { Vector3{ 0.0f, 0.0f, 0.0f }, Vector3One() * 200 };
    const int spawnCount = 1000;

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = bounds.Max() + bounds.Extents();  // Camera position
    camera.target = bounds.Center();                    // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };            // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    // Spawn boids for management
    GridBins<Boid *> gridBins = GridBins<Boid *>(bounds, 16);
	std::array<Boid, spawnCount> boids;
    for (int i = 0; i < spawnCount; i++)
	{
		// Generate a random position and velocity for each boid.
		// Position is within the bounds, velocity is normalized and scaled to 
        // max speed.
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

        
        // Create a boid with the generated position and velocity and add it 
        // to the grid bins for management.
        boids[i] = Boid{ i, pos, vel};

        // Note: The grid bins are not used in this example, at the moment 
        // they decrease performance rather than increase it. Grid bins were 
        // an idea caried over from the C# for Unity version of this project.
        //int binIndex = gridBins.WorldPosToVectorIndex(pos);
        //if (binIndex >= 0 && binIndex < gridBins.Bins().size())
        //    gridBins.Bins()[binIndex].push_back(boid);
    }


    DisableCursor(); // Limit cursor to relative movement inside the window

    SetTargetFPS(60);  // Set simulation to target 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update the camera and reset camera if requested
        UpdateCamera(&camera, CAMERA_FREE);
        if (IsKeyPressed('Z')) 
        {
            camera.position = bounds.Max() + (bounds.Extents() / 4);
            camera.target = bounds.Center();
        }

        // Update all Boids
        for (int i = 0; i < spawnCount; i++)
		{
			Boid boid = boids[i];
            Vector3 pos = boid.position;

            /*int binArrayIndex = gridBins.WorldPosToVectorIndex(pos);
			if (binArrayIndex < 0) continue;*/

            std::vector<Boid> neighbors = std::vector<Boid>{};

            for (size_t j = 0; j < boids.size(); j++)
            {
                if (boids[j].id == boid.id) continue;

                if (Vector3Distance(boids[j].position, boid.position) <=
                    boid.senseDistance)
                    neighbors.push_back(boids[j]);
            }

            // NOTE: Grid bins are not being used. See line64 for further information.
            /*std::vector<int> searchBins = 
                gridBins.GetNeighborBinIndices(binArrayIndex);
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

            // NOTE: Grid bins are not being used. See line64 for further information.
            /*int calculatedBinIndex = gridBins.WorldPosToVectorIndex(boid.position);
			if (calculatedBinIndex < 0 || calculatedBinIndex >= gridBins.Bins().size())
				continue;
			boids[i].UpdateBinIndex(calculatedBinIndex);*/
        }

        // Start drawing to the window
        BeginDrawing(); 

		// Clear the window and set the background color
        ClearBackground(Color{ 35, 35, 35, 255 });

        // Start to draw 3D objects with the camera
        BeginMode3D(camera);

		// Loop through all boids and draw them
		Vector3 pos = { 0.0f, 0.0f, 0.0f };
		Vector3 vel = { 0.0f, 0.0f, 0.0f };
		Vector3 normalizedVel = { 0.0f, 0.0f, 0.0f };
		Color color = { 0, 0, 0, 255 }; // Default color for boids
		float r = 0.0f, g = 0.0f, b = 0.0f;
        for (int i = 0; i < spawnCount; i++)
        {
            // Get i'th boid's position and velocity 
			pos = boids[i].position;
			vel = boids[i].velocity;

            normalizedVel = Vector3Normalize(vel);
			
			r = Vector3DotProduct(normalizedVel, Vector3{ 1.0f, 0.0f, 0.0f });
			g = Vector3DotProduct(normalizedVel, Vector3{ 0.0f, 1.0f, 0.0f });
			b = Vector3DotProduct(normalizedVel, Vector3{ 0.0f, 0.0f, 1.0f });

            color = Color{ 
                (unsigned char)(r * 127 + 128), 
                (unsigned char)(g * 127 + 128),
                (unsigned char)(b * 127 + 128),
                255 };

            DrawCapsule(pos, pos - normalizedVel * 2.0f, 1.0f, 2, 4, color);

            // NOTE: The following section was used for debugging grid bins 
            // which are not in use. See line 64 for more information.
            continue; // COMMENT THIS LINE TO DRAW BIN OF BOID 0
			if (i != 0) continue;
			int calculatedBinIndex = gridBins.WorldPosToVectorIndex(pos);
			if (calculatedBinIndex < 0 || calculatedBinIndex >= gridBins.Bins().size())
				DrawSphere(pos, 3.0f, RAYWHITE);
			Vector3 debugBoxSize = gridBins.BinSize();
			Vector3 debugBoxCenter = 
                gridBins.GetBinMin(calculatedBinIndex) + debugBoxSize / 2;
			DrawCubeWiresV(debugBoxCenter, debugBoxSize, YELLOW);
        }

		// Draw the bounds of the simulation
        DrawCubeWiresV(bounds.Center(), bounds.Size(), Color{ 128, 128, 128, 128 });

		// Stop drawing 3D objects
        EndMode3D();

        // Draw UI elements on top of 3D drawings
        DrawRectangle(10, 10, 320, 93, Fade(RAYWHITE, 0.75f));
        DrawRectangleLines(10, 10, 320, 93, BLACK);

        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Z to reset camera view", 40, 80, 10, DARKGRAY);

        // Display the current FPS in the top right corner
		DrawFPS(1500, 20); 

        // Stop drawing to the window
        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
