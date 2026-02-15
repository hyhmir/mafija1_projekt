#include "matrix.h"
#include "physics.h"
#include "render.h"
#include "raylib.h"
#include <iostream>

using std::cout;

int main(int argc, char* argv[])
{
    if (argc < 11) {
        cout << "Usage: " << argv[0] << " <upper temp> <lower temp> <periodic> <m> <n> <parallel cond.> <perpendicular cond.> <angle1> <angle2> <capacity>\n";
        return 1;
    }

    const double utemp = std::stod(argv[1]);
    const double ltemp = std::stod(argv[2]);
    const bool periodic = std::stoi(argv[3]);
    const int m = std::stoi(argv[4]);
    const int n = std::stoi(argv[5]);
    const double lv = std::stod(argv[6]);
    const double lp = std::stod(argv[7]);
    const double angle1 = std::stod(argv[8]);
    const double angle2 = std::stod(argv[9]);
    const double c = std::stod(argv[10]);

    matrix old(angle1, angle2, lv, lp, {m, n}, ltemp, c);
    matrix newed(angle1, angle2, lv, lp, {m, n}, ltemp, c);

    InitWindow(m * 50, n * 50, "Matrix Visualization");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_matrix(old);
        EndDrawing();

        if (IsKeyDown(KEY_ENTER))
            step(old, newed, periodic, ltemp, utemp);
        else if (IsKeyPressed(KEY_Q)){
            CloseWindow();
            return 0;
        }
    }

    return 0;
}
