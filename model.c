/*
 * model.c
 *
 * Initializes and dumps a model struct.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include "sphere.h"
#include "light.h"
#include "plane.h"
#include "fplane.h"
#include "tplane.h"
#include "texplane.h"
#include "pplane.h"
#include "psphere.h"
#include "projection.h"
#include "object.h"
#include "common.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>


obj_t *dummy_init(FILE *, int);

static obj_t *(*object_loaders[])(FILE *, int) =
{
    light_init, 
    dummy_init,
    dummy_init,
    sphere_init,
    plane_init,
    fplane_init,
    tplane_init,
    texplane_init,
    dummy_init,
    psphere_init,
    pplane_init
};
#define NUM_LOADERS sizeof(object_loaders) / sizeof(void *)

/**
 * Initialize a model struct and scene object by reading in from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  model   - model to initalize
 *
 * RETURN:
 * 0 on success
 */
int model_init(FILE *in, model_t *model) {
    char buf[256];      // buffer to read into
    int  objtype = 0;   // type of object currently being read in
    obj_t *obj;         // new object being initalized

    // read in from file to initialize scene objects
    while (fscanf(in, "%d", &objtype) != EOF) {
        fgets(buf, 256, in);
#ifdef DEBUG_MODEL
        fprintf(stderr, "model_init read %s\n", buf);
#endif
        if (objtype > LAST_TYPE || objtype < FIRST_TYPE) {
            fprintf(stderr, "Invalid object type: %d\n", objtype);
            exit(EXIT_FAILURE);
        }
        obj = object_loaders[(objtype - FIRST_TYPE)](in, objtype); 

        /*
        switch (objtype) {
            case LIGHT:
                obj = light_init(in, objtype);
                break;
            case SPHERE:
                obj = sphere_init(in, objtype);
                break;
            case PLANE:
                obj = plane_init(in, objtype);
                break;
            case FPLANE:
                obj = fplane_init(in, objtype);
                break;
            case TPLANE:
                obj = tplane_init(in, objtype);
                break;
            case P_SPHERE:
                obj = psphere_init(in, objtype);
                break;
            case P_PLANE:
                obj = pplane_init(in, objtype);
                break;
            default:
                fprintf(stderr, "Unknown object type: %d\n", objtype);
                exit(UNKNOWN_TYPE);
                break;
        }
        */

        if (obj != NULL) {
            if (objtype > LAST_LIGHT) {
                list_add(model->scene, obj);
            } else {
                list_add(model->lights, obj);
            }
        } else {
            fprintf(stderr, "Error initializing object: %d\n", objtype);
            exit(INIT_FAILURE);
        }

    }

    return 0;
}

/**
 * Prints information contained in a models struct.
 *
 * PARAMETERS:
 *  out     - file to write to
 *  model   - model to dump
 */
void model_dump(FILE *out, model_t *model) {
    obj_t *obj = model->scene->head;    

    fprintf(out, "MODEL:\n");
    projection_dump(stderr, model->proj);
    
    // print out objects in the scene
    while (obj != NULL) {
        obj->dump(stderr, obj);
        obj = obj->next;
    }

}

/**
 * Place holder for objtypes that haven't been defined
 */
obj_t *dummy_init(FILE *in, int objtype) {
    fprintf(stderr, "Invalid object type %d\n", objtype);   
    exit(EXIT_FAILURE);
}


