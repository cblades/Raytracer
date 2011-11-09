/*
 * light.c
 *
 * Initialize, dump, and hit functions for a light object.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include "common.h"
#include "object.h"
#include "safe.h"
#include "light.h"
#include "veclib3d.h"

/**
 * Initialize a light object from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  objtype - type of object to initalize
 *
 *  RETURNS:
 *  the newly created object
 */
obj_t * light_init(FILE *in, int objtype) {
    obj_t *obj = object_init(in, objtype);  // base object struct
    light_t *light = (light_t *)smalloc(sizeof(light_t));   // new light struct

    obj->priv = light;      // connect light to obj
    obj->dump = light_dump;

    char    buf[256];       // buffer for reading in values
    int     rc = 0;         // read counter


    // read in emissivity
    while (rc != 3 && !feof(in)) {
        rc = fscanf(in, "%lf %lf %lf", light->emissivity,
                                      (light->emissivity + 1),
                                      (light->emissivity + 2));
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "1)light read %s\n", buf);
#endif
    }

    rc = 0;

    // read in center
    while (rc != 3 && !feof(in)) {
        rc = fscanf(in, "%lf %lf %lf",  light->center,
                                       (light->center + 1),
                                       (light->center + 2));
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "2)light read %s\n", buf);
#endif
    }
    return obj;
}

/**
 * Prints information about a light object to a file.
 *
 * PARAMETERS:
 *  out -   file to write to
 *  obj -   object to dump
 */
void light_dump(FILE *out, obj_t *obj) {
    light_t *light = (light_t *)obj->priv; // get light struct out of obj

    fprintf(out, "\tTYPE: Light\n");
    fprintf(out, "\t\tEMISSIVITY: (%lf, %lf, %lf)\n", light->emissivity[0],
                                                      light->emissivity[1],
                                                      light->emissivity[2]);
    fprintf(out, "\t\tCENTER: (%lf, %lf, %lf)\n", light->center[0],
                                                  light->center[1],
                                                  light->center[2]);
}

