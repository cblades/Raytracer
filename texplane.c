/*
 * texplane.c
 *
 * Initialize, dump, and hit functions for a texplane object.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "object.h"
#include "safe.h"
#include "plane.h"
#include "fplane.h"
#include "texture.h"
#include "texplane.h"
#include "material.h"
#include "veclib3d.h"

/**
 * Initialize an texplane object from a file.
 *
 * PARAMETERS:
 *  in      - file to read from
 *  objtype - type of object to initalize
 *
 *  RETURNS:
 *  the newly created object
 */
obj_t * texplane_init(FILE *in, int objtype) {
    texplane_t *texplane;
    fplane_t   *fp;
    plane_t    *p;
    obj_t      *obj;
    
    obj = fplane_init(in, objtype);  // base object struct
    
    p = (plane_t *)obj->priv;
    fp = (fplane_t *)p->priv;

    texplane = 
            (texplane_t *)smalloc(sizeof(texplane_t));   // new texplane struct
        
    fp->priv = texplane;      // connect texplane to obj
    obj->dump = texplane_dump;    
    obj->getamb = texplane_amb;
    obj->getdif = texplane_diff;
    obj->obj_free = texplane_free;

    char    buf[256];       // buffer for reading in values
    int     rc = 0;         // read counter

    texplane->texname[0] = '\0';

    while (strlen(texplane->texname) < 2) {
        // read in texture filename
        fgets(texplane->texname, FILENAME_SIZE, in);
    }
    if (texplane->texname[strlen(texplane->texname) - 1] == '\n') {
        texplane->texname[strlen(texplane->texname) - 1] = '\0';
    }
#ifdef DEBUG_TEXTURE
        fprintf(stderr, "texture file: %s\n", texplane->texname);
#endif    
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "texplane read %s\n", buf);
#endif

    rc = 0;
    // read in tile mode
    while (rc != 1) {
        rc = fscanf(in, "%df", &texplane->texmode);

        fgets(buf, 256, in);

#ifdef DEBUG_TEXTURE
        fprintf(stderr, "Texturing mode: ");
        if (texplane->texmode == FIT_TEXTURE) {
            fprintf(stderr, "scale\n");
        } else if (texplane->texmode == TILE_TEXTURE) {
            fprintf(stderr, "tile\n");
        } else {
            fprintf(stderr, "unrecognized (%d\n", texplane->texmode);
        }
#endif
#ifdef DEBUG_OBJECTS
        fprintf(stderr, "texplane read %s\n", buf);
#endif
    }
    
    if (texture_load(texplane)) {
#ifdef DEBUG_TEXTURE
        fprintf(stderr, "Failed to load texture file\n");
        return NULL;
#endif
    }

   return obj;
}


/*
 * Frees memory associated with a texplane object.
 *
 * PARMETERS:
 *  obj - texplane object to free
 */
void texplane_free(obj_t *obj) {
    plane_t *p = (plane_t *)obj->priv;
    fplane_t *fp = (fplane_t *)p->priv;
    texplane_t *tp = (texplane_t *)fp->priv;
   
    if (tp->texture != NULL) {
        texture_free(tp->texture);
        free(tp->texture);
    }

    fplane_free(obj);
}


/**
 * Prints information about a texplane object to a file.
 *
 * PARAMETERS:
 *  out -   file to write to
 *  obj -   object to dump
 */
void texplane_dump(FILE *out, obj_t *obj) {
    plane_t *plane = (plane_t *)obj->priv;
    fplane_t *fplane = (fplane_t *)plane->priv;
    texplane_t *texplane = 
                (texplane_t *)fplane->priv; // get texplane struct out of obj

    fprintf(out, "\tTYPE: TexPlane\n");
    
    fplane_dump(out, obj);

    fprintf(out, "\t\tTexture file: %s\n", texplane->texname);
    fprintf(out, "\t\tTexture Mode: %d\n", texplane->texmode);
}

/*
 * Retrieves the diffuse value for a texplane and applies
 * the texture to the diffuse value
 *
 * PARAMETRS:
 *  obj    - texplane object
 *  values - array to store rgb values in
 */
void texplane_diff(obj_t *obj, double *values) {
    plane_t    *p   = (plane_t *)obj->priv;
    fplane_t   *fp  = (fplane_t *)p->priv;
    material_t *mat = &obj->material;
    double texel[3];

    texture_map(fp, texel);
#ifdef DEBUG_TEXTURE
    fprintf(stderr, "Texel: (%lf, %lf, %lf)\n", texel[0], texel[1], texel[2]);
#endif

    *(values + 0) = mat->diffuse[0] * texel[0];
    *(values + 1) = mat->diffuse[1] * texel[1];
    *(values + 2) = mat->diffuse[2] * texel[2];
}

/*
 * Retrieves the ambient value for a texplane and applies
 * the texture to the ambient value
 *
 * PARAMETRS:
 *  obj    - texplane object
 *  values - array to store rgb values in
 */
void texplane_amb(obj_t *obj, double *values) {
    plane_t    *p   = (plane_t *)obj->priv;
    fplane_t   *fp  = (fplane_t *)p->priv;
    material_t *mat = &obj->material;
    double texel[3];

    texture_map(fp, texel);
#ifdef DEBUG_TEXTURE
    fprintf(stderr, "Texel: (%lf, %lf, %lf)\n", texel[0], texel[1], texel[2]);
#endif

    *(values + 0) = mat->ambient[0] * texel[0];
    *(values + 1) = mat->ambient[1] * texel[1];
    *(values + 2) = mat->ambient[2] * texel[2];
}
