/*
 * sphere.c
 *
 * Intialization, dump, and hits function for sphere objects.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include <math.h>
#include "common.h"
#include "sphere.h"
#include "object.h"
#include "safe.h"
#include "material.h"
#include "veclib3d.h"

/*
 * Intialize a sphere object by reading in from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  objtype - type of object to initialize
 *
 *  RETURN:
 *  newly created object
 */
obj_t *sphere_init(FILE *in, int objtype) {
    sphere_t *sphere = NULL;    // sphere object to create
    char    buf[256];           // buffer to read into
    int     rc = 0;             // read counter

    
    obj_t *obj = object_init(in, objtype);
    
    sphere = (sphere_t *)smalloc(sizeof(sphere_t));
    
    material_init(in, &obj->material);
    // connect sphere to obj
    obj->priv = sphere;

    // connect function pointers
    obj->hits = hits_sphere;
    obj->dump = sphere_dump;;

    // read in center
    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", &sphere->center[0], &sphere->center[1],
                                        &sphere->center[2]);
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "sphere read %s\n", buf);
#endif
    }

    rc = 0;

    // read in radius
    while (rc != 1) {
        rc = fscanf(in, "%lf", &sphere->radius);
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "sphere read %s\n", buf);
#endif
    }

    return obj;
}

/*
 * Prints information about a sphere object.
 *
 * PARAMETERS:
 *  out - file to print to
 *  obj - object to dump
 */
void sphere_dump(FILE *out, obj_t *obj) {   
    sphere_t *sphere = (sphere_t *)obj->priv;

    fprintf(out, "\tTYPE: Sphere\n");
    fprintf(out, "\t\tCENTER: (%lf, %lf, %lf)\n", *(sphere->center),
                                                  *(sphere->center + 1),
                                                  *(sphere->center + 2));
    fprintf(out, "\t\tRADIUS: %lf\n",   sphere->radius);
}

/*
 * Determines if a ray hits the given sphere object.
 *
 * PARAMETERS:
 *  base    - the starting point of the ray
 *  dir     - the direction of the ray
 *  obj     - object to check against
 *
 *  RETURNS:
 *  the distance from base to the hit point, or -1
 */
double hits_sphere(double *base, double *dir, obj_t *obj) {
    sphere_t *sphere = (sphere_t *)obj->priv;
    
    double Vprime[3];
    double H[3];
    double N[3];
    double temp[3];
    double a;
    double b;
    double c;
    double discriminant;
    double t;
    
    
    vec_diff3(sphere->center, base, Vprime);
    
    a = vec_dot3(dir, dir);
    b = 2.0 * vec_dot3(Vprime, dir);
    c = vec_dot3(Vprime, Vprime) - pow(sphere->radius, 2);

    discriminant = pow(b, 2) - (4 * a * c);
#ifdef DEBUG_OBJECTS
    fprintf(stderr, "a=%lf b=%lf c=%lf\n", a, b, c);
    fprintf(stderr, "discriminant=%lf\n", discriminant);
#endif
    if (discriminant > 0) {
        t = ((b * -1) - sqrt(discriminant)) / (2 * a);   
    } else {
        return -1;
    }
    

    vec_scale3(t, dir, temp);   
    vec_sum3(base, temp, H);

    vec_diff3(sphere->center, H, N);
    vec_unit3(N, N);

#ifdef DEBUG_OBJECTS
    vec_prn3(stderr, "Sphere Normal", N);
#endif
    obj->hitloc[0] = H[0];
    obj->hitloc[1] = H[1];
    obj->hitloc[2] = H[2];
    
    obj->normal[0] = N[0];
    obj->normal[1] = N[1];
    obj->normal[2] = N[2];

    return t;
}
