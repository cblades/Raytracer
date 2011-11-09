/*
 * plane.c
 *
 * Initialize, dump, and hit functions for a plane object.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include "common.h"
#include "object.h"
#include "safe.h"
#include "plane.h"
#include "material.h"
#include "veclib3d.h"

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
obj_t * plane_init(FILE *in, int objtype) {
    obj_t *obj = object_init(in, objtype);  // base object struct
    plane_t *plane = (plane_t *)smalloc(sizeof(plane_t));   // new plane struct

    material_init(in, &obj->material);
    plane->priv = NULL;
    obj->priv = plane;      // connect plane to obj
    obj->hits = hits_plane; // connect hits function
    obj->dump = plane_dump;
    obj->obj_free = plane_free;

    char    buf[256];       // buffer for reading in values
    int     rc = 0;         // read counter

    // read in normal
    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", plane->normal,
                                       (plane->normal + 1),
                                       (plane->normal + 2));
        vec_unit3(plane->normal, plane->normal);
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "plane read %s\n", buf);
#endif
    }

    rc = 0;
    // read in point
    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", plane->point, (plane->point + 1),
                                        (plane->point + 2));
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "plane read %s\n", buf);
#endif
    }

    return obj;
}

/*
 * Free a plane structure, free priv pointer and then call obj_free.
 *
 * PARAMETER:
 *  obj -   plane object to free
 */
void plane_free(obj_t *obj) {
    plane_t *plane = (plane_t *)(obj->priv);
    if (plane->priv != NULL) {
        free(plane->priv);
    }
    obj_free(obj);
}

/**
 * Prints information about a plane object to a file.
 *
 * PARAMETERS:
 *  out -   file to write to
 *  obj -   object to dump
 */
void plane_dump(FILE *out, obj_t *obj) {
    plane_t *plane = (plane_t *)obj->priv; // get plane struct out of obj

    fprintf(out, "\tTYPE: Plane\n");
    fprintf(out, "\t\tPOINT: (%lf, %lf, %lf)\n",  plane->point[0],
                                                  plane->point[1],
                                                  plane->point[2]);
    fprintf(out, "\t\tNORMAL: (%lf, %lf, %lf)\n", plane->normal[0],
                                                  plane->normal[1],
                                                  plane->normal[2]);
}

/*
 * Determines if a ray hits the given plane object.
 *
 * PARAMETERS:
 * base -   starting point of ray
 * dir  -   direction of ray
 * obj  -   object to test against
 *
 * RETURNS:
 * the distance from the base to the hit point
 */
double hits_plane(double *base, double *dir, obj_t *obj) {
    plane_t *plane = (plane_t *)obj->priv;                  // plane struct

    // Normal dot Point
    double NdotQ = vec_dot3(plane->normal, plane->point); 
    // Normal dot ray origin
    double NdotV = vec_dot3(plane->normal, base);
    // Normal dot direction of ray
    double NdotD = vec_dot3(plane->normal, dir);

    double t = (NdotQ - NdotV) / (NdotD);

    // ray is parallel to plane, not meaningful, so consider a miss
    if (NdotD == 0.0) {
        return -1;

    } 
    
    // if t < 0, plane lies in front of screen, so disregard
    if (t < 0.0001) {
        return -1;
    }
    
    double H[3];
    double tD[3];
    vec_scale3(t, dir, tD);
    vec_sum3(base, tD, H);
 
    // make sure hit point is behind the screen
    if (H[2] >= 0) {
        return -1;
    }

    obj->hitloc[0] = H[0];
    obj->hitloc[1] = H[1];
    obj->hitloc[2] = H[2];

    obj->normal[0] = plane->normal[0];
    obj->normal[1] = plane->normal[1];
    obj->normal[2] = plane->normal[2];

    return t;
}

