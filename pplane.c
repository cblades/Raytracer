/*
 * pplane.c
 *
 * Initialize, dump, and hit functions for a pplane object.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "object.h"
#include "safe.h"
#include "plane.h"
#include "pplane.h"
#include "material.h"
#include "veclib3d.h"


/**
 * Function table that holds pointers to shader functions
 */
static void (*plane_shaders[])(obj_t *, double *) =
{
    &pplane0_amb,
    &pplane1_amb
};
#define PLANE_NUM_SHADERS sizeof(plane_shaders) / sizeof(void *)

/**
 * Initialize a plane object from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  objtype - type of object to initalize
 *
 *  RETURNS:
 *  the newly created object
 */
obj_t * pplane_init(FILE *in, int objtype) {
    obj_t *obj = plane_init(in, objtype);  // base object struct
    char    buf[256];       // buffer for reading in values
    int     rc = 0;         // read counter
    int     sndx = -1;      // shader index

    // read in shader index
    while (rc != 1) {
        rc = fscanf(in, "%d", &sndx);
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "pplane read %s\n", buf);
#endif
    }
    
    if (sndx >= PLANE_NUM_SHADERS || sndx < 0) {
        fprintf(stderr, "Invalid shader index given: %d\n", sndx);
        exit(EXIT_FAILURE);
    } else {
        obj->getamb = plane_shaders[sndx];
    }
    return obj;
}

/*
 * Procedural shader 1
 *
 * PARAMETERS:
 *  obj     - the plane object to shade
 *  value   - intensity vector
 */
void pplane0_amb(obj_t *obj, double *value) {
    double vec[3];
    plane_t *p = (plane_t *)(obj->priv);
    int distanceX, distanceY;
    
    // find x distance
    getamb_default(obj, value);
    vec_diff3(p->point, obj->hitloc, vec);
    vec[1] = 0;
    distanceX = (int)vec_length3(vec);
    
    // find y distance
    vec_diff3(p->point, obj->hitloc, vec);
    vec[0] = 0;
    distanceY = (int)vec_length3(vec);

    if ((distanceX % 6 >= 3) && (distanceY % 6 >= 3)) {
        
        *(value + 0) = 0;
        *(value + 1) = 0;
        *(value + 2) = 0;
    }
}


/*
 * Procedural shader 2
 *
 * PARAMETERS:
 *  obj     - the plane object to shade
 *  value   - intensity vector
 */
void pplane1_amb(obj_t *obj, double *value) {
    double vec[3];
    plane_t *p = (plane_t *)(obj->priv);
    double distanceX;
    
    // find x distance
    getamb_default(obj, value);
    vec_diff3(p->point, obj->hitloc, vec);
    vec[1] = 0;
    distanceX = (int)vec_length3(vec);
    distanceX = distanceX / 10;
       

    *(value + 0) = *(value + 0) / cos(distanceX);
    *(value + 1) = *(value + 1) / cos(distanceX);
    *(value + 2) = *(value + 2) / cos(distanceX);
}
