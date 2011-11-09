/*
 * Image.c
 *
 * Constructs ppm image
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <string.h>
#include <stdlib.h>
#include "safe.h"
#include "projection.h"
#include "veclib3d.h"
#include "image.h"
#include "ray.h"

/**
 * Call methods that find rgb values for each pixel in the ppm file.
 *
 * PARAMETERS:
 *  model   - model representing the 3d scene and other ray tracing values
 */
void make_image(model_t *model) {
    unsigned char *pixmap = NULL;               // buffer to hold pixel data
    unsigned char *pixcurr = NULL;              // current position in buffer
    int x = 0;                                  // x coord (in pixels)
    int y = 0;                                  // y coord (in pixels)
    int size = model->proj->win_size_pixel[0] * // size of the img in pixels
        model->proj->win_size_pixel[1];

    // allocate space for the buffer
    pixmap = (unsigned char *)smalloc(sizeof(unsigned char) * 3 * size);
    pixcurr = pixmap;

    // for every pixel, call make_pixel
    for (y = 0; y < model->proj->win_size_pixel[1]; y++) {
        for (x = 0; x < model->proj->win_size_pixel[0]; x++) {
#ifdef DEBUG_MAKE
            fprintf(stderr, "make_image: pixel(%d, %d)\n", x, y);
#endif
            // index into pixmap to get the location of the current pixel
            pixcurr = pixmap + ((model->proj->win_size_pixel[1] - y - 1) * 
                    model->proj->win_size_pixel[0] * 3) + (x * 3);

            make_pixel(model, x, y, pixcurr);

        }
    }
    
    // print header
    printf("P6 %d %d 255\n", model->proj->win_size_pixel[0],
            model->proj->win_size_pixel[1]);

    // dump pixel values to file
    fwrite(pixmap,
            sizeof(unsigned char),
            model->proj->win_size_pixel[0] * model->proj->win_size_pixel[1] * 3,
            stdout);

    free(pixmap);
}

/**
 * For each pixel, call ray_trace and set rgb values of corresponding pixel.
 *
 * PARAMETERS:
 *  model   - container for the scene and other ray tracing structs
 *  x       - x coordinate of the pixel
 *  y       - y coordinage of the pixel
 *  pixval  - pointer to location to store rgb values
 */
void make_pixel(model_t *model, int x, int y, unsigned char *pixval) {
    double *world     = alloca(3 * sizeof(double)); // world coords of pixel
    double *intensity = alloca(3 * sizeof(double)); // intensity of rgb
    double *dir       = alloca(3 * sizeof(double)); // direction of ray

    // convert pixel coords to world coords
    map_pix_to_world(model->proj, x, y, world);

    // zero out intensity
    memset(intensity, 0, 3 * sizeof(double));

    // find direction of ray and convert to unit vector
    vec_diff3(model->proj->view_point, world, dir);
    vec_unit3(dir, dir);

    ray_trace(model, model->proj->view_point, dir, intensity, 0.0, NULL);

#ifdef DEBUG_MAKE
    fprintf(stderr, "Intensity: %lf %lf %lf\n", *(intensity + 0),
            *(intensity + 1),
            *(intensity + 2));
#endif

    // clamp intensity so that 0 <= intensity[n] <= 1.0
    *(intensity + 0) = *(intensity + 0) < 0.0 ? 0.0 : *(intensity + 0);
    *(intensity + 0) = *(intensity + 0) > 1.0 ? 1.0 : *(intensity + 0);

    *(intensity + 1) = *(intensity + 1) < 0.0 ? 0.0 : *(intensity + 1);
    *(intensity + 1) = *(intensity + 1) > 1.0 ? 1.0 : *(intensity + 1);

    *(intensity + 2) = *(intensity + 2) < 0.0 ? 0.0 : *(intensity + 2);
    *(intensity + 2) = *(intensity + 2) > 1.0 ? 1.0 : *(intensity + 2);

    
    // set rgb value of pixel
    *(pixval + 0) = (int)(255 * (*(intensity + 0)));
    *(pixval + 1) = (int)(255 * (*(intensity + 1)));
    *(pixval + 2) = (int)(255 * (*(intensity + 2)));

#ifdef DEBUG_MAKE
    fprintf(stderr, "pixval=(%d, %d, %d)\n", *(pixval + 0),
                                             *(pixval + 1), 
                                             *(pixval + 2));
#endif
}
