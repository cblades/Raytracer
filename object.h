#include <stdio.h>
#include "ray.h"

#ifndef OBJECT_H
#define OBJECT_H
obj_t *object_init(FILE *, int);

void obj_free(obj_t *);

void getamb_default (obj_t *, double *);

void getdif_default (obj_t *, double *);

void getspec_default (obj_t *, double *);

void object_dump(FILE *, obj_t *);
#endif
