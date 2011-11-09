/*
 * object.c
 *
 * Initialize and dump object structs.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include "common.h"
#include "safe.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"

/**
 * Intialize an object by reading in from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  objtyp  - type of object to read in
 *
 *  RETURNS:
 *  pointer to the newly initialized object
 */
obj_t *object_init(FILE *in, int objtype) {
    obj_t   *new      = NULL;   // object being created
    static int id     = 0;      // id for object, every object has unize id
    
    new = (obj_t *)smalloc(sizeof(obj_t));


    new->objtype = objtype;
    new->objid   = id;
    
    new->getamb = getamb_default;
    new->getdif = getdif_default;
    new->getspec = getspec_default;
    new->obj_free = obj_free;
     
    new->next=NULL;
    id++;
    return new;
}

/*
 * Frees an obj_t by freeing the priv pointer and then the obj_t struct.
 *
 * PARAMETERS:
 *  obj -   object to free
 */
void obj_free(obj_t * obj) {
    free(obj->priv);
    free(obj);
}

/**
 * Returns the ambient values contained within an object's material.
 *
 * PARAMETERS:
 *  obj       - object to retrieve ambient values from
 *  intensity - array to store ambient values in
 */
void getamb_default (obj_t *obj, double *intensity) {
    material_t *mat = &obj->material; // material struct
    double *ambient = mat->ambient;   // ambient array from mat

    *(intensity + 0) = *(ambient + 0);
    *(intensity + 1) = *(ambient + 1);
    *(intensity + 2) = *(ambient + 2);

}

/**
 * Returns the diffuse values contained within an object's material.
 *
 * PARAMETERS:
 *  obj       - object to retrieve diffuse values from
 *  intensity - array to store ambient values in
 */
void getdif_default (obj_t *obj, double *intensity) {
    material_t *mat = &obj->material; // material struct
    double *diffuse = mat->diffuse;   // ambient array from mat

    *(intensity + 0) = *(diffuse + 0);
    *(intensity + 1) = *(diffuse + 1);
    *(intensity + 2) = *(diffuse + 2);

}


/**
 * Returns the specular values contained within an object's material.
 *
 * PARAMETERS:
 *  obj       - object to retrieve diffuse values from
 *  intensity - array to store specular values in
 */
void getspec_default (obj_t *obj, double *intensity) {
    material_t *mat = &obj->material; // material struct
    double *specular = mat->specular;   // ambient array from mat

    *(intensity + 0) = *(specular + 0);
    *(intensity + 1) = *(specular + 1);
    *(intensity + 2) = *(specular + 2);

}

/**
 * Prints information about an object.
 *
 * PARAMETERS:
 *  out - file to write to
 *  obj - object to dump
 */
void object_dump(FILE *out, obj_t *obj) {
    fprintf(out, "\t\tOBJECT ID: %d\n", obj->objid);
    material_dump(out, &obj->material);
    
    switch (obj->objtype) {
        case SPHERE:
            sphere_dump(stderr, obj);
            break;
        case PLANE:
            plane_dump(stderr, obj);
            break;
    }
}
