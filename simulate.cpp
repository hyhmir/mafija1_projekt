#include <string>
#include <vector>
#include <iostream>
#include <math.h>
#include <algorithm>
#include "raylib.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::sin;
using std::cos;
using std::swap;

struct blok
{
    int x;
    int y;
    double temp;
    double jx;
    double jy;
    double c;
    double lxx;
    double lyy;
    double lyx;

    blok() : x(0), y(0), temp(0.0), jx(0.0), jy(0.0), c(0.0), lxx(0.0), lyy(0.0), lyx(0.0) {}

    blok(double angle, double lv, double lp, std::tuple<int, int> position, double temperature, std::tuple<double, double> j, double c) : temp(temperature), c(c) {
        x = std::get<0>(position);
        y = std::get<1>(position);

        jx = std::get<0>(j);
        jy = std::get<1>(j);

        lxx = lv*cos(angle)*cos(angle) + lp*sin(angle)*sin(angle);
        lyy = lp*cos(angle)*cos(angle) + lv*sin(angle)*sin(angle);
        lyx = sin(angle)*cos(angle)*(lv - lp);
    }
};

struct matrix 
{
    int m;
    int n;
    vector<vector<blok>> vals;

    matrix(double angle1, double angle2, double lv, double lp, std::tuple<int, int> size, double temperature, double c) : m(std::get<0>(size)), n(std::get<1>(size)) {
        std::tuple<double, double> tok = {0.0, 0.0};
        std::tuple<int, int> position = {0, 0};
        vals.resize(m, vector<blok>(n));
        for (int i=0; i<m; i++) {
            for (int j=0; j<n; j++) {
                position = {i, j};
                if ((i+j)%2 == 0) {
                    vals[i][j] = blok(angle1, lv, lp, position, temperature, tok, c);
                }
                else {
                    vals[i][j] = blok(angle2, lv, lp, position, temperature, tok, c);
                }
                
            }
        }
    }

    // Access element safely
    blok& at(int i, int j) {
        return vals[i][j];
    }

    // Print matrix
    void print(bool tok) {
        if (tok) {
            for (int i = n-1; i >= 0; i--) {
                for (int j = 0; j < m; j++)
                    cout << vals[j][i].jy << " ";
                cout << endl;
            }
        }
        else {
            for (int i = n-1; i >= 0; i--) {
                for (int j = 0; j < m; j++)
                    cout << vals[j][i].jy << " ";
                cout << endl;
            }
            cout << endl;
            for (int i = n-1; i >= 0; i--) {
                for (int j = 0; j < m; j++)
                    cout << vals[j][i].temp << " ";
                cout << endl;
            }
        }
    }
};

blok update(blok old, blok& zgori, blok& spodi, blok& levo, blok& desno) {
    double j_in = spodi.jy - zgori.jy + levo.jx - desno.jx;
    old.temp += j_in/old.c;
    old.jx = old.lxx * (levo.temp - desno.temp) + old.lyx * (spodi.temp - zgori.temp);
    old.jy = old.lyx * (levo.temp - desno.temp) + old.lyy * (spodi.temp - zgori.temp);

    return old;
}

void step(matrix& old, matrix& newed, bool periodic, double temp_dol, double temp_gor) {

    if ((old.m != newed.m) or (old.n != newed.n) or (old.n<2) or (old.m<2)) {
        throw std::runtime_error("Dimensions don't match or are too small");
    }

    if (periodic) {
        for (int i=0; i<old.m; i++) {
            for (int j=0; j<old.n; j++) {
                if (j%(old.n-1) != 0) {
                    newed.vals[i][j] = update(old.vals[i][j], old.vals[i][j+1], old.vals[i][j-1], old.vals[((i-1)%(old.m-1) + (old.m-1))%(old.m-1)][j], old.vals[(i+1)%(old.m-1)][j]);
                }
                else if (j == 0) {
                    blok spodi = blok(0.0, 0.0, 0.0, {0, 0}, temp_dol, {old.vals[i][j].jx, old.vals[i][j].jy}, 0.0);
                    newed.vals[i][j] = update(old.vals[i][j], old.vals[i][j+1], spodi, old.vals[((i-1)%(old.m-1) + (old.m-1))%(old.m-1)][j], old.vals[(i+1)%(old.m-1)][j]);
                }
                else if (j == (old.n - 1)) {
                    blok zgori = blok(0.0, 0.0, 0.0, {0, 0}, temp_gor, {old.vals[i][j].jx, old.vals[i][j].jy}, 0.0);
                    newed.vals[i][j] = update(old.vals[i][j], zgori, old.vals[i][j-1], old.vals[((i-1)%(old.m-1) + (old.m-1))%(old.m-1)][j], old.vals[(i+1)%(old.m-1)][j]);
                }
            }
        }
    }

    else {
        for (int i=0; i<old.m; i++) {
            for (int j=0; j<old.n; j++) {
                if (i%(old.m-1) != 0 && j%(old.n-1) != 0) {
                    newed.vals[i][j] = update(old.vals[i][j], old.vals[i][j+1], old.vals[i][j-1], old.vals[i-1][j], old.vals[i+1][j]);
                }
                else if ((j%(old.n-1) == 0) && (i%(old.m-1) == 0)) {
                    blok levo = blok(0.0, 0.0, 0.0, {0, 0}, old.vals[i][j].temp, {0.0, 0.0}, 0.0);
                    blok desno = blok(0.0, 0.0, 0.0, {0, 0}, old.vals[i][j].temp, {0.0, 0.0}, 0.0);
                    blok spodi = blok(0.0, 0.0, 0.0, {0, 0}, temp_dol, {old.vals[i][j].jx, old.vals[i][j].jy}, 0.0);
                    blok zgori = blok(0.0, 0.0, 0.0, {0, 0}, temp_gor, {old.vals[i][j].jx, old.vals[i][j].jy}, 0.0);


                    int ii = (i == (old.m-1));   // 0 or 1
                    int jj = (j == (old.n-1));   // 0 or 1

                    int state = (ii << 1) | jj;
                    switch (state)
                    {
                    case 0:
                        newed.vals[i][j] = update(old.vals[i][j], old.vals[i][j+1], spodi, levo, old.vals[i+1][j]);
                        break;

                    case 1:
                        newed.vals[i][j] = update(old.vals[i][j], zgori, old.vals[i][j-1], levo, old.vals[i+1][j]);
                        break;

                    case 2:
                        newed.vals[i][j] = update(old.vals[i][j], old.vals[i][j+1], spodi, old.vals[i-1][j], desno);
                        break;

                    case 3:
                        newed.vals[i][j] = update(old.vals[i][j], zgori, old.vals[i][j-1], old.vals[i-1][j], desno);
                        break;
                    
                    default:
                        break;
                    }
                }
                else if (j == 0) {
                    blok spodi = blok(0.0, 0.0, 0.0, {0, 0}, temp_dol, {old.vals[i][j].jx, old.vals[i][j].jy}, 0.0);
                    newed.vals[i][j] = update(old.vals[i][j], old.vals[i][j+1], spodi, old.vals[i-1][j], old.vals[i+1][j]);
                }
                else if (j == old.n - 1) {
                    blok zgori = blok(0.0, 0.0, 0.0, {0, 0}, temp_dol, {old.vals[i][j].jx, old.vals[i][j].jy}, 0.0);
                    newed.vals[i][j] = update(old.vals[i][j], zgori, old.vals[i][j-1], old.vals[i-1][j], old.vals[i+1][j]);
                }
                else if (i == 0) {
                    blok levo = blok(0.0, 0.0, 0.0, {0, 0}, old.vals[i][j].temp, {0.0, 0.0}, 0.0);
                    newed.vals[i][j] = update(old.vals[i][j], old.vals[i][j+1], old.vals[i][j-1], levo, old.vals[i+1][j]);
                }
                else if (i == old.m - 1) {
                    blok desno = blok(0.0, 0.0, 0.0, {0, 0}, old.vals[i][j].temp, {0.0, 0.0}, 0.0);
                    newed.vals[i][j] = update(old.vals[i][j], old.vals[i][j+1], old.vals[i][j-1], old.vals[i-1][j], desno);
                }
            }
        }
    }
    

    swap(old, newed);
}


int main(int argc, char* argv[]) {

    // handling params
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

    // setting it up
    matrix old = matrix(angle1, angle2, lv, lp, {m, n}, ltemp, c);
    matrix newed = matrix(angle1, angle2, lv, lp, {m, n}, ltemp, c);

    old.print(false);

    cout << "Press ENTER to step, or 'q' to quit\n";

    const int cellSize = 50;

    InitWindow(m * cellSize, n * cellSize, "Matrix Visualization");
    SetTargetFPS(60);
    int count = 0;

    // string input;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                double value = old.at(r, c).temp; // 0.0 - 1.0
                unsigned char red = static_cast<unsigned char>(value * 255);
                unsigned char blue = static_cast<unsigned char>((1-value) * 255);
                unsigned char green = static_cast<unsigned char>(0 * 255);
                Color color = {red, green, blue, 255};
                DrawRectangle(c * cellSize, r * cellSize, cellSize-2, cellSize-2, color);
            }
        }

        EndDrawing();

        if (IsKeyPressed(KEY_Q)){
            CloseWindow();
            return 0;
        }
        else if(IsKeyDown(KEY_ENTER)) {
            step(old, newed, periodic, ltemp, utemp);
            // cout << "step" << count << endl;
            // count++;
        }
        // old.print(false);
    }

    return 0;
}
