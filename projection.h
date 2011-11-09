#include "common.h"

#ifndef PROJECTION_H
#define PROJECTION_H

proj_t *projection_init(int, char **, FILE *);

void projection_dump(FILE *, proj_t *);

void map_pix_to_world(proj_t *, int, int, double *);

void map_world_to_pix(double *, int *);
#endif
