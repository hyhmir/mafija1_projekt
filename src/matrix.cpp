#include "matrix.h"
#include <iostream>
#include <cmath>
#include <vector>

using std::cout;
using std::endl;
using std::sin;
using std::cos;
using std::vector;


blok::blok() : x(0), y(0), temp(0.0), jx(0.0), jy(0.0), c(0.0), lxx(0.0), lyy(0.0), lyx(0.0) {}

blok::blok(double angle, double lv, double lp, std::tuple<int, int> position, double temperature, std::tuple<double, double> j, double c) : temp(temperature), c(c) {
    x = std::get<0>(position);
    y = std::get<1>(position);

    jx = std::get<0>(j);
    jy = std::get<1>(j);

    lxx = lv*cos(angle)*cos(angle) + lp*sin(angle)*sin(angle);
    lyy = lp*cos(angle)*cos(angle) + lv*sin(angle)*sin(angle);
    lyx = sin(angle)*cos(angle)*(lv - lp);
}

matrix::matrix(double angle1, double angle2, double lv, double lp, std::tuple<int, int> size, double temperature, double c) : m(std::get<0>(size)), n(std::get<1>(size)) {
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

blok& matrix::at(int i, int j) {
    return vals[i][j];
}

void matrix::print(bool tok) {
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