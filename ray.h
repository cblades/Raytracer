#include "common.h"

#ifndef RAY_H
#define RAY_H

#define MAX_DIST 30

void ray_trace(model_t *, double *, double *, double *, double, obj_t *);

obj_t *find_closest_obj(list_t *, double *, double *, obj_t *, double *);

void diffuse_illumination(model_t *, obj_t *, double *);

int process_light(list_t *, obj_t *, obj_t *, double *);
#endif
