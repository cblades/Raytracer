#ifndef TEXTURE_H
#define TEXTURE_H

int texture_load(texplane_t *);

int texture_map(fplane_t *, double *);

void texel_get(texture_t *, double, double, double *);

void texture_free(texture_t *tex);
#endif
