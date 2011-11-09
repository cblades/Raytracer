/*
 * psphere.c
 *
 * Initialize, dump, and hit functions for a psphere object.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "object.h"
#include "safe.h"
#include "sphere.h"
#include "psphere.h"
#include "material.h"
#include "veclib3d.h"


/**
 * Function table that holds pointers to shader functions
 */
static void (*sphere_shaders[])(obj_t *, double *) =
{
    &psphere0_amb
};
#define PLANE_NUM_SHADERS sizeof(sphere_shaders) / sizeof(void *)

/**
 * Initialize a sphere object from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  objtype - type of object to initalize
 *
 *  RETURNS:
 *  the newly created object
 */
obj_t * psphere_init(FILE *in, int objtype) {
    obj_t *obj = sphere_init(in, objtype);  // base object struct
    char    buf[256];       // buffer for reading in values
    int     rc = 0;         // read counter
    int     sndx = -1;      // shader index

    // read in shader index
    while (rc != 1) {
        rc = fscanf(in, "%d", &sndx);
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "psphere read %s\n", buf);
#endif
    }
    
    if (sndx >= PLANE_NUM_SHADERS || sndx < 0) {
        fprintf(stderr, "Invalid shader index given: %d\n", sndx);
        exit(EXIT_FAILURE);
    } else {
        obj->getamb = sphere_shaders[sndx];
    }
    
    // seed randomizer for later use
    srand(1);

    return obj;
}

/*
 * Procedural shader 1
 *
 * PARAMETERS:
 *  obj     - the sphere object to shade
 *  value   - intensity vector
 */
void psphere0_amb(obj_t *obj, double *value) {
    double vec[3];
    sphere_t *p = (sphere_t *)(obj->priv);
    int distance;
    
    getamb_default(obj, value);   
    
    // find y distance
    vec_diff3(p->center, obj->hitloc, vec);
    distance = (int)vec_length3(vec);
       
    if (distance % 2 == 0) {
        int ran = rand();
        *(value + (ran % 3)) = 0;
    }
}
