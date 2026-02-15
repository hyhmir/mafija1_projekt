#include "render.h"
#include "raylib.h"

void draw_matrix(const matrix& m) {
    const int cellSize = 50;

    for (int r = 0; r < m.m; r++) {
        for (int c = 0; c < m.n; c++) {
            double value = m.vals[r][c].temp;
            unsigned char red = (unsigned char)(value * 255);
            unsigned char blue = (unsigned char)((1-value) * 255);
            Color color = { red, 0, blue, 255 };

            DrawRectangle(c * cellSize, r * cellSize,
                          cellSize-2, cellSize-2, color);
        }
    }
}