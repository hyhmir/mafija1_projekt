#include "physics.h"
#include <stdexcept>
#include <algorithm>


using std::swap;


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