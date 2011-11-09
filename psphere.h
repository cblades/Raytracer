#include <stdio.h>
#include "object.h"

#ifndef PSPHERE_H
#define PSPHERE_H
obj_t *psphere_init(FILE *, int);

void psphere0_amb(obj_t *, double *);

void psphere1_amb(obj_t *, double *);

#endif
