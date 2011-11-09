/*
 * tplane.c
 *
 * Initialize, dump, and hit functions for a tplane object.
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
#include "tplane.h"
#include "material.h"
#include "veclib3d.h"

/**
 * Initialize a tplane object from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  objtype - type of object to initalize
 *
 *  RETURNS:
 *  the newly created object
 */
obj_t * tplane_init(FILE *in, int objtype) {
    obj_t *obj = plane_init(in, objtype);  // base object struct
    plane_t *plane = (plane_t *)obj->priv; // base plane struct

    tplane_t *tplane = 
            (tplane_t *)smalloc(sizeof(tplane_t));   // new tplane struct

    plane->priv = tplane;      // connect tplane to obj
    obj->hits = hits_plane; // connect hits function
    obj->getamb = tp_amb;
    obj->getdif = tp_diff;
    obj->getspec = tp_spec;


    char    buf[256];       // buffer for reading in values
    int     rc = 0;         // read counter


    // read in x direction
    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", tplane->xdir, (tplane->xdir + 1),
                                        (tplane->xdir + 2));
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "tplane read %s\n", buf);
#endif
    }

    rc = 0;
    // read in normal
    while (rc != 2) {
        rc = fscanf(in, "%lf %lf", tplane->size,
                                       (tplane->size + 1));

        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "tplane read %s\n", buf);
#endif
    }

    
    // initailize backgorund material
    material_init(in, &tplane->background);
    
    // construct rotation matrix
   double normal[3];
   double xdir[3];
   
   //vec_project3(plane->normal, tplane->xdir, tplane->xdir);

   vec_unit3(plane->normal, normal);
   vec_unit3(tplane->xdir, xdir);
    

   vec_unit3(xdir, tplane->rotmat[0]);
   vec_unit3(normal, tplane->rotmat[2]);

   vec_cross3(tplane->rotmat[2], tplane->rotmat[0], tplane->rotmat[1]);
   return obj;
}

/**
 * Prints information about a tplane object to a file.
 *
 * PARAMETERS:
 *  out -   file to write to
 *  obj -   object to dump
 */
void tplane_dump(FILE *out, obj_t *obj) {
    plane_t *plane = (plane_t *)obj->priv;
    tplane_t *tplane = 
                (tplane_t *)plane->priv; // get tplane struct out of obj

    fprintf(out, "\tTYPE: TPlane\n");

    plane_dump(out, obj);
    fprintf(out, "\t\tXDIR: (%lf, %lf, %lf)\n",   tplane->xdir[0],
                                                  tplane->xdir[1],
                                                  tplane->xdir[2]);
    fprintf(out, "\t\tSIZE: (%lf, %lf)\n", tplane->size[0],
                                                  tplane->size[1]);
}


/**
 * Retrieve diffuse values for the last hitpoint
 *
 * PARAMETERS:
 *  obj     - tplane object
 *  value   - intensity vector
 */
void tp_diff(obj_t *obj, double *value) {
#ifdef DEBUG_TPLANE
    fprintf(stderr, "tp_diff\n");
#endif

    plane_t *pln = (plane_t *)obj->priv;
    tplane_t *tp = (tplane_t *)pln->priv;
    material_t *mat;

    if (tp_select(obj)) {
        mat = &obj->material;
    } else {
        mat = &tp->background;
    }

    *(value + 0)  = mat->diffuse[0];  
    *(value + 1)  = mat->diffuse[1]; 
    *(value + 2)  = mat->diffuse[2];
}


/**
 * Retrieve ambient values for the last hitpoint
 *
 * PARAMETERS:
 *  obj     - tplane object
 *  value   - intensity vector
 */
void tp_amb(obj_t *obj, double *value) {
#ifdef DEBUG_TPLANE
    fprintf(stderr, "tp_amb\n");
#endif

    plane_t *pln = (plane_t *)obj->priv;
    tplane_t *tp = (tplane_t *)pln->priv;
    material_t *mat;

    if (tp_select(obj)) {
        mat = &obj->material;
    } else {
        mat = &tp->background;
    }

    *(value + 0)  = mat->ambient[0];  
    *(value + 1)  = mat->ambient[1]; 
    *(value + 2)  = mat->ambient[2];
}


/**
 * Retrieve specular values for the last hitpoint
 *
 * PARAMETERS:
 *  obj     - tplane object
 *  value   - intensity vector
 */
void tp_spec(obj_t *obj, double *value) {
#ifdef DEBUG_TPLANE
    fprintf(stderr, "tp_spec\n");
#endif

    plane_t *pln = (plane_t *)obj->priv;
    tplane_t *tp = (tplane_t *)pln->priv;
    material_t *mat;

    if (tp_select(obj)) {
        mat = &obj->material;
    } else {
        mat = &tp->background;
    }

    *(value + 0)  = mat->specular[0];  
    *(value + 1)  = mat->specular[1]; 
    *(value + 2)  = mat->specular[2];
}

/**
 * Determine if the current hitpoint is on a foreground or background
 * tile.
 *
 * PARAMETERS:
 *  obj - tplane object
 *
 *  RETURNS:
 *  0 for background and 1 for foreground
 */
int tp_select(obj_t *obj) {
    plane_t  *plane  = (plane_t *)obj->priv;       // plane struct
    tplane_t *tplane = (tplane_t *)plane->priv;    // tplane struct

    double *newhit = (double *)alloca(sizeof(double) * 3);

    vec_diff3(plane->point, obj->hitloc, newhit);

    xform3(tplane->rotmat, newhit, newhit);

    // HACK!  the 1000 is to take care of rouding error near boundries
    int relx = (int) (1000 + newhit[0] / tplane->size[0]);
    int rely = (int) (1000 + newhit[1] / tplane->size[1]);

    int total = relx + rely;
#ifdef DEBUG_TPLANE
    fprintf(stderr, "relx = %d, rely = %d, mod = %d\n", relx, rely, total % 2);
#endif

    return total % 2;
}
