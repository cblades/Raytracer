#include <stdio.h>
#include "object.h"

#ifndef FPLANE_H
#define FPLANE_H

obj_t *fplane_init(FILE *, int);

void fplane_free(obj_t *);

void fplane_dump(FILE *, obj_t *);

double hits_fplane(double *, double *, obj_t *);
#endif
