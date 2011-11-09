#include <stdio.h>
#include "ray.h"

#ifndef SPHERE_H
#define SPHERE_H

obj_t *sphere_init(FILE *, int);

void sphere_dump(FILE *, obj_t *);

double hits_sphere(double *, double *, obj_t *);
#endif
