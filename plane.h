#include <stdio.h>
#include "object.h"

#ifndef PLANE_H
#define PLANE_H

obj_t *plane_init(FILE *, int);

void plane_free(obj_t *);

void plane_dump(FILE *, obj_t *);

double hits_plane(double *, double *, obj_t *);
#endif
