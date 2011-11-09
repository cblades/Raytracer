#include <stdio.h>
#include "header.h"
#include "safe.h"
#include "common.h"
#include "texture.h"
#include "projection.h"

#define PPM_COLOR_VERSION 6

/*
 * Loads a ppm file as a texture and stores pixel values.
 *
 * PARAMETERS:
 *  tp  - texplane objec to store texture struct in
 */
int texture_load(texplane_t *tp) {
    int            size       = 0;
    ppm_header    *header     = NULL;
    FILE          *texFile    = NULL;
    texture_t     *texture    = NULL;

    if ((texFile = fopenAndCheck(tp->texname, "r")) == NULL) {
#ifdef DEBUG_TEXTURE
        fprintf(stderr, "failed to open texture file:%s|\n", tp->texname);
#endif
        return EXIT_FAILURE;
    }

    header = (ppm_header *)malloc(sizeof(ppm_header));
    readHeader(texFile, header);
   
    if (header == NULL) {
        fprintf(stderr, "Invalid header, exiting..\n");
#ifdef DEBUG_TEXTURE
        fprintf(stderr, "Invalid texture header\n");
#endif
        return EXIT_FAILURE;
    } else if (header->version != PPM_COLOR_VERSION) {
        fprintf(stderr, "Header version must be P6, exiting.\n");
#ifdef DEBUG_TEXTURE
        fprintf(stderr, "Invalid header version: %d\n", header->version);
#endif
        return EXIT_FAILURE;
    }

    texture = (texture_t *)smalloc(sizeof(texture_t));
    tp->texture = texture;
    
    texture->size[0] = header->width;
    texture->size[1] = header->height;

#ifdef DEBUG_TEXTURE
    fprintf(stderr, "texture size: %d X %d\n", texture->size[0], 
                                               texture->size[1]);
#endif

    size = header->width * header->height;
    
    texture->texbuf = (unsigned char *)smalloc(3 * size);

    int numRead = fread(texture->texbuf, sizeof(unsigned char),
                                                        size * 3, texFile);

    if (numRead != size * 3) {
        fprintf(stderr, "Corrupt input file, got %d bytes\n", numRead);
        return EXIT_FAILURE;
    }


    free(header);
    return EXIT_SUCCESS;

}

/*
 * Returns the rgb values for the texture at a given point.
 *
 * PARAMETERS:
 *  fp    - finite plane object that holds the point to retrieve
 *  texel - array to store texture rgb values in
 */
int texture_map(fplane_t *fp, double *texel) {
    texplane_t *tp  = (texplane_t *)fp->priv;
    texture_t  *tex = (texture_t  *)tp->texture;
    double xfrac, yfrac;
    
    // scale mode
    if (tp->texmode == FIT_TEXTURE) {
#ifdef DEBUG_TEXTURE
        fprintf(stderr, "lasthit: (%lf, %lf)\n", fp->lasthit[0], 
                                                 fp->lasthit[1]);
#endif
        xfrac = fp->lasthit[0] / fp->size[0];
        yfrac = fp->lasthit[1] / fp->size[1];

        texel_get(tex, xfrac, yfrac, texel);

    // tile mode
    } else {
        int pixhit[2];
        map_world_to_pix(fp->lasthit, pixhit);

        xfrac = (double)(pixhit[0] % tex->size[0]) / tex->size[0];
        yfrac = (double)(pixhit[1] % tex->size[1]) / tex->size[1];

        texel_get(tex, xfrac, yfrac, texel);
    }
    return EXIT_SUCCESS;
}


/*
 * given a relative x and y coordiante, finds the corrisponding
 * texture values.
 *
 * PARAMETERS:
 *  tex     - texture object
 *  xrel    - x coordinate relative to a point
 *  yrel    - y coordinate relative to a point
 *  texel   - array to store texture rgb values in
 *
 */
void texel_get(texture_t *tex, double xrel, double yrel, double *texel) {
    unsigned char *texloc;
    int xtex;       /* x coordinate of the texel */
    int ytex;       /* y coordinate of the texel */
    
    xtex = xrel * tex->size[0];
    ytex = yrel * tex->size[1];

#ifdef DEBUG_TEXTURE
    fprintf(stderr, "xrel, yrel = (%lf, %lf)\n", xrel, yrel);
    fprintf(stderr, "xtex, ytex = (%d, %d)\n", xtex, ytex);
#endif

    // make sure the texture coordinates make sense
    if ((xtex < 0) || (ytex < 0) || 
        (xtex >= tex->size[0]) || (ytex >= tex->size[1])) {
#ifdef DEBUG_TEXTURE
        fprintf(stderr, "got (xtex, ytex) = (%d, %d)\n", xtex, ytex);
#endif
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG_TEXTURE
    fprintf(stderr, "texloc offset = %d\n", offset);
#endif

    texloc = tex->texbuf + 
                (((tex->size[1] - 1 - ytex) * tex->size[0] * 3) + 3 * xtex);

    *(texel + 0) = *(texloc + 0) / 255.0;
    *(texel + 1) = *(texloc + 1) / 255.0;
    *(texel + 2) = *(texloc + 2) / 255.0;
}


void texture_free(texture_t *tex) {
    if (tex->texbuf != NULL) {
        free(tex->texbuf);
    }
}
