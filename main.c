/*
 * Main.c
 *
 * Entry point for ray tracer
 *
 * Chris Blades
 *
 * 31/3/2011
 */

#include "common.h"
#include "model.h"
#include "projection.h"
#include "list.h"
#include "safe.h"
#include "image.h"

/**
 * Entry point for ray tracer.  call methods to initialize model, projection,
 * and all object.
 *
 * PARAMETERS:
 *  argc    - the number of command line arguments
 *  argv    - array of pointers to command line arguments
 */
int main(int argc, char **argv) {
    // container for scene data and objects
    model_t *model = (model_t *)smalloc(sizeof(model_t));

    int rc; // return value from model_init

    // initialize projection
    model->proj = projection_init(argc, argv, stdin);

    projection_dump(stderr, model->proj);

    model->lights = list_init();
    model->scene = list_init();

    rc = model_init(stdin, model);

    model_dump(stderr, model);

    if (rc == 0) {
        make_image(model);
    }

    list_del(model->lights);
    list_del(model->scene);

    free(model->proj);
    free(model);

    return(EXIT_SUCCESS);
}
