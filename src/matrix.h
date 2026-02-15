#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <tuple>

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

    blok();
    blok(double angle, double lv, double lp,
         std::tuple<int,int> position,
         double temperature,
         std::tuple<double,double> j,
         double c);
};

struct matrix
{
    int m;
    int n;
    std::vector<std::vector<blok>> vals;

    matrix(double angle1, double angle2, double lv, double lp,
           std::tuple<int,int> size, double temperature, double c);

    blok& at(int i, int j);
    void print(bool tok);
};

#endif