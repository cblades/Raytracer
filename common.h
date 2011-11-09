#include <stdio.h>

#ifndef COMMON_H
#define COMMON_H

#define UNKNOWN_TYPE 1
#define INIT_FAILURE 2

#define FIT_TEXTURE  1
#define TILE_TEXTURE 2

#define FILENAME_SIZE 40

/* object types */
#define FIRST_TYPE  10
#define LIGHT       10
#define SPHERE      13
#define PLANE       14
#define FPLANE      15
#define TPLANE      16
#define TEX_PLANE   17
#define P_SPHERE    19
#define P_PLANE     20
#define LAST_TYPE   20
#define LAST_LIGHT  10


/* material */
typedef struct material_type {
    double ambient[3];
    double diffuse[3];
    double specular[3];
} material_t;

typedef struct obj_type {
    struct obj_type *next;
    int    objid;
    int    objtype;

    /* hits function */
    double (*hits) (double *, double *, struct obj_type *);
    
    /* dump function */
    void   (*dump) (FILE *, struct obj_type *);
    
    /* delete function */
    void (*obj_free) (struct obj_type *);

    /* reflectivity and light functions */
    void   (*getamb) (struct obj_type *, double *);
    void   (*getdif) (struct obj_type *, double *);
    void   (*getspec) (struct obj_type *, double *);

    /* defines refelctivity and light properties */
    material_t material;

    /* private data area */
    void    *priv;

    double hitloc[3];
    double normal[3];
} obj_t;


/* holds texture data for textured plane */
typedef struct texture_type {
    int         size[2];    /* x, y dimensions */
    unsigned char *texbuf;  /* pixmap buffer */
} texture_t;

/* textured plane type */
typedef struct texplane_type {
    int         texmode;                    /* 1 -> fit and 2 -> tile */
    char        texname[FILENAME_SIZE];     /* .ppm filename */
    texture_t  *texture;                    /* pointer to texture structure */
} texplane_t;


/* tiled plane */
typedef struct tplane_type {
    double xdir[3];         /* orientation of tiling */
    double size[2];         /* size of tiles */
    double rotmat[3][3];    /* rotation matrix */
    material_t background;  /* background color */
} tplane_t;

/* Infinite plane */
typedef struct plane_type {
    double  point[3];   // point on the plane
    double  normal[3];  // normal vector
    
    void *priv;         // used for inheritance
} plane_t;

/* finite plane */
typedef struct fplane_type {
    double xdir[3];     // x axis direction
    double size[2];     // width x height
    double rotmat[3][3];// rotation matrix
    double lasthit[2];  // used for textures
    void  *priv;        // hold derived classes
} fplane_t;

/* Sphere */
typedef struct sphere_type {
    double  center[3];
    double  radius;
} sphere_t;


/* Light */
typedef struct light_type {
    double center[3];
    double emissivity[3];
} light_t;

typedef struct projection_type {
    int     win_size_pixel[2];
    double  win_size_world[2];
    double  view_point[3];
} proj_t;


typedef struct list_type {
    obj_t   *head;
    obj_t   *tail;
} list_t;

typedef struct model_type {
    proj_t  *proj;
    list_t  *lights;
    list_t  *scene;
}   model_t;

#endif
