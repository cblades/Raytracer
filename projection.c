/*
 * projection.c
 *
 * Intialize and dump functions for a projection struct.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "safe.h"

static proj_t *projection;

/*
 * initialize a projection struct from a file and command line arguments.
 *
 * PARAMETERS:
 * argc - number of command line arguments
 * argv - array of command line arguments
 * in   - file to read from
 *
 * RETURNS:
 * an initialized projection struct
 */
proj_t *projection_init(int argc, char **argv, FILE *in) {
    char buf[256];      // buffer for reading in
    int  rc      = 0;   // read counter

    // new projection struct
    proj_t *proj = (proj_t *)smalloc(sizeof(proj_t));

    // read in read in size of world
    while (rc != 2) {
        rc = fscanf(in, "%lf %lf", proj->win_size_world,
                                   (proj->win_size_world + 1));
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "projection read %s\n", buf);
#endif
    }

    rc = 0;

    // read in view point location
    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", proj->view_point,
                                   (proj->view_point + 1),
                                   (proj->view_point + 2));
        fgets(buf, 256, in);
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "projection read %s\n", buf);
#endif
    }

    // copy size of screen in pixels from command line args
    proj->win_size_pixel[0] = atoi(argv[1]);
    proj->win_size_pixel[1] = atoi(argv[2]);
    
    projection = proj;
    
    return proj;
}

/**
 * Print information about a projection structure.
 *
 * PARAMETERS:
 *  out  - file to print to
 *  proj - projection to dump
 */
void projection_dump(FILE *out, proj_t *proj) {
    fprintf(out, "\tPROJECTION:\n");
    fprintf(out, "\t\tWindow size pixels: %d X %d\n", proj->win_size_pixel[0], 
                                                     proj->win_size_pixel[1]);

    fprintf(out, "\t\tWindow size world: %lf X %lf\n", proj->win_size_world[0],
                                                     proj->win_size_world[1]);

    fprintf(out, "\t\tViewpoint: (%lf, %lf, %lf)\n",  proj->view_point[0],
                                                    proj->view_point[1],
                                                    proj->view_point[2]);
}

/*
 * Converts coordinates (in pixels) on the screen to 3d coordinates in the
 * scene.
 *
 * PARAMETERS:
 *  proj    - projection struct
 *  x       - x coordinate on screen
 *  y       - y coordinate on screen
 *  world   - array to store 3d coordinates in
 */
void map_pix_to_world(proj_t *proj, int x, int y, double *world) {
    *(world + 0) = (double)x / (proj->win_size_pixel[0] - 1) *
                    proj->win_size_world[0];

    *(world + 0) -= proj->win_size_world[0] / 2.0;

    *(world + 1) = (double)y / (proj->win_size_pixel[1] - 1) *
                   proj->win_size_world[1];

    *(world + 1) -= proj->win_size_world[1] / 2.0;

    *(world + 2) = 0.0;
}

void map_world_to_pix(double *point, int *pixhit) {
    *(pixhit + 0) = (projection->win_size_pixel[0] - 1) *  
                    (*(point + 0) / projection->win_size_world[0]);
                       
    
    *(pixhit + 1) = (projection->win_size_pixel[1] - 1) * 
                    (*(point + 1) / projection->win_size_world[1]);
                     
}
