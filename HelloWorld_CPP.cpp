#include <iostream>
#include "C:\raylib\raylib\src\raylib.h"

int main(int argc, char* argv[])
{
    // Phase 1 output (console output stream)
    //std::cout << "Hello, World!" << std::endl;

    // Phase 2 output (raylib window)
    int screenWidth = 500;
    int screenHeight = 400;
    //const char[] text = "Hello, World!"; 

    InitWindow(screenWidth, screenHeight, "Hello, World!");

    SetTargetFPS(144);

    // Main raylib update loop
    while (!WindowShouldClose()) // Detects window close button and ESC key
    {
        // Update variables
        

        // Draw to window
        BeginDrawing();

        ClearBackground(Color{255, 255, 255, 255});

        DrawText("Hello, World!", 250, 200, 20, Color{0, 0, 0, 255});
        
        EndDrawing();
    }

    // De-initialize window
    CloseWindow();
    
    return 0;
}
