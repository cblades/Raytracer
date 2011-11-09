#include "common.h"

#ifndef TEXPLANE_H
#define TEXPLANE_H
obj_t * texplane_init(FILE *, int);

void texplane_dump(FILE *, obj_t *);

void texplane_diff(obj_t *, double *);

void texplane_amb(obj_t *, double *);

void texplane_free(obj_t *);
#endif
