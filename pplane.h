#include <stdio.h>
#include "object.h"

#ifndef PPLANE_H
#define PPLANE_H
obj_t *pplane_init(FILE *, int);

void pplane0_amb(obj_t *, double *);

void pplane1_amb(obj_t *, double *);

#endif
