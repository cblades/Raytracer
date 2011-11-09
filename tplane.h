#include <stdio.h>
#include "object.h"

#ifndef TPLANE_H
#define TPLANE_H

obj_t *tplane_init(FILE *, int);

void tplane_dump(FILE *, obj_t *);

void tp_diff(obj_t *, double *);

void tp_amb(obj_t *, double *);

void tp_spec(obj_t *, double *);

int tp_select(obj_t *);
#endif
