/*
 * fplane.c
 *
 * Initialize, dump, and hit functions for a fplane object.
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
#include "fplane.h"
#include "material.h"
#include "veclib3d.h"

/**
 * Initialize an ffplane object from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  objtype - type of object to initalize
 *
 *  RETURNS:
 *  the newly created object
 */
obj_t * fplane_init(FILE *in, int objtype) {
    obj_t *obj = plane_init(in, objtype);  // base object struct
    plane_t *plane = (plane_t *)obj->priv; // base plane struct

    fplane_t *fplane = 
            (fplane_t *)smalloc(sizeof(fplane_t));   // new fplane struct
    
    fplane->priv = NULL;

    plane->priv = fplane;      // connect fplane to obj
    obj->hits = hits_fplane; // connect hits function
    obj->dump = fplane_dump;
    obj->obj_free = fplane_free;

    char    buf[256];       // buffer for reading in values
    int     rc = 0;         // read counter


    // read in x direction
    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", fplane->xdir, (fplane->xdir + 1),
                                        (fplane->xdir + 2));
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "fplane read %s\n", buf);
#endif
    }

    rc = 0;
    // read in size
    while (rc != 2) {
        rc = fscanf(in, "%lf %lf", fplane->size,
                                       (fplane->size + 1));

        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "fplane read %s\n", buf);
#endif
    }


    // construct rotation matrix
    
   double normal[3];
   double xdir[3];
 
   vec_project3(plane->normal, fplane->xdir, fplane->xdir);
      
   vec_unit3(plane->normal, normal);
   vec_unit3(fplane->xdir, xdir);
    
   
   vec_unit3(xdir, fplane->rotmat[0]);
   vec_unit3(normal, fplane->rotmat[2]);

   vec_cross3(fplane->rotmat[2], fplane->rotmat[0], fplane->rotmat[1]);
   return obj;
}

void fplane_free(obj_t *obj) {
    plane_t  *p  = (plane_t *)obj->priv;
    fplane_t *fp = (fplane_t *)p->priv;

    if (fp->priv != NULL) {
        free(fp->priv);
    }

    plane_free(obj);
}


/**
 * Prints information about a fplane object to a file.
 *
 * PARAMETERS:
 *  out -   file to write to
 *  obj -   object to dump
 */
void fplane_dump(FILE *out, obj_t *obj) {
    plane_t *plane = (plane_t *)obj->priv;
    fplane_t *fplane = 
                (fplane_t *)plane->priv; // get fplane struct out of obj

    fprintf(out, "\tTYPE: FPlane\n");

    plane_dump(out, obj);
    fprintf(out, "\t\tXDIR: (%lf, %lf, %lf)\n",   fplane->xdir[0],
                                                  fplane->xdir[1],
                                                  fplane->xdir[2]);
    fprintf(out, "\t\tSIZE: (%lf, %lf)\n", fplane->size[0],
                                                  fplane->size[1]);
}

/*
 * Determines if a ray hits the given fplane object.
 *
 * PARAMETERS:
 * base -   starting point of ray
 * dir  -   direction of ray
 * obj  -   object to test against
 *
 * RETURNS:
 * the distance from the base to the hit point
 */
double hits_fplane(double *base, double *dir, obj_t *obj) {
    plane_t  *plane  = (plane_t *)obj->priv;       // plane struct
    fplane_t *fplane = (fplane_t *)plane->priv;    // fplane struct

    double *newhit = (double *)alloca(sizeof(double) * 3);

    // check to see if ray would hit an infinite plane
    double t = hits_plane(base, dir, obj);
    
    // if not, return
    // should be less than 0, 0.000 is a hack for rounding error
    if (t < 0.00001) {
        return(t);
    }
    
    vec_diff3(plane->point, obj->hitloc, newhit);
    
    
    xform3(fplane->rotmat, newhit, newhit);

    if ((newhit[0] > fplane->size[0]) || (newhit[0] < 0.0)) {
        return -1;
    }

    if ((newhit[1] > fplane->size[1]) || (newhit[1] < 0.0)) {
        return -1;
    }
    
    fplane->lasthit[0] = newhit[0];
    fplane->lasthit[1] = newhit[1];


    return t;
    
}
