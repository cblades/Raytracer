/*
 * material.c
 *
 * Initialize and dump fucntion for material struct.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include "common.h"
#include "material.h"

/**
 * Initialize a material struct by reading in ambient, diffuse, and specular
 * rgb values from a file.
 *
 * PARAMETERS:
 *  in  -   file to read from
 *  mat -   material struct to initialize
 */
void material_init(FILE *in, material_t *mat) {
    char buf[256];  // buffer to read into
    int  rc = 0;    // read counter

    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", mat->ambient, (mat->ambient + 1),
                                        (mat->ambient + 2) );
        fgets(buf, 256, in);
    }
    
    rc = 0;

    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", mat->diffuse, (mat->diffuse + 1),
                                        (mat->diffuse + 2) );
        fgets(buf, 256, in);
    }

    rc = 0;

    while (rc != 3) {
        rc = fscanf(in, "%lf %lf %lf", mat->specular, (mat->specular + 1),
                                        (mat->specular + 2) );
        fgets(buf, 256, in);
    }
}

/*
 * Print information in a material struct.
 *
 * PARAMETERS:
 *  out - file to write to
 *  mat - material struct to dump
 */
void material_dump(FILE *out, material_t *mat) {
    fprintf(out, "\tMATERIAL: \n");
    fprintf(out, "\t\tAmbient {%lf, %lf, %lf}\n", mat->ambient[0], 
                                                  mat->ambient[1],
                                                  mat->ambient[2]);

    fprintf(out, "\t\tDiffuse {%lf, %lf, %lf}\n", mat->diffuse[0], 
                                                  mat->diffuse[1],
                                                  mat->diffuse[2]);

    fprintf(out, "\t\tSpecular {%lf, %lf, %lf}\n", mat->specular[0],
                                                   mat->specular[1],
                                                   mat->specular[2]);
}
