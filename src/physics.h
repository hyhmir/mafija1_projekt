#ifndef PHYSICS_H
#define PHYSICS_H

#include "matrix.h"

blok update(blok old, blok& zgori, blok& spodi,
            blok& levo, blok& desno);

void step(matrix& old, matrix& newed,
          bool periodic, double temp_dol, double temp_gor);

#endif