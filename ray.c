/** Ray.c
 *
 * Perform main ray tracing algorithm.
 *
 * Chris Blades
 *
 * 31/3/2011
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ray.h"
#include "veclib3d.h"
#include "common.h"
/**
 * Project rays from the view point through the screen to determine the
 * rgb values of that pixel.
 *
 * PARAMETERSS:
 *  model     - contains scene data
 *  base      - origin of ray
 *  dir       - direction of ray
 *  intensity - intensity of rgb values of the pixel
 *  total_dist- the total distance the ray has traveled
 *  last_hit  - location of the rays last hit
 */
void ray_trace(model_t *model, double base[3], double dir[3], 
               double intensity[3], double total_dist, obj_t *last_hit) {
    
    double ambient[3];      // holds ambient value for rgb at hit point
    obj_t *closest = NULL;  // closest object that ray hits
    double mindist = 0.0;   // distance from ray origin to hit point
    double specref[3] = {0.0, 0.0, 0.0};
    double ref_dir[3];

    if (total_dist > MAX_DIST) {
        return;
    }

    // get closet object that is hit
    closest = find_closest_obj(model->scene, base, dir, last_hit, &mindist);

    if (closest == NULL) {
        return;
    }
#ifdef DEBUG_TRACE
    fprintf(stderr, "closest object=%d\n", closest->objid);
    fprintf(stderr, "mindist=%lf\n", mindist);
#endif
    closest->getamb(closest, ambient);
    total_dist += mindist;

#ifdef DEBUG_TRACE
    fprintf(stderr, "Ambient of closest: %lf %lf %lf\n", *(ambient + 0),
                                                      *(ambient + 1),
                                                      *(ambient + 2));
#endif

   // set intensity
   // ambient
   vec_sum3(ambient, intensity, intensity);

   // start diffuse...
   diffuse_illumination(model, closest, intensity); 
#ifdef DEBUG_DIFFUSE
   fprintf(stderr, "ray_trace() mindist at end: %f\n", mindist);
#endif
   vec_scale3(1.0 / mindist, intensity, intensity);
   // end diffuse...
   
   // start specular...
   closest->getspec(closest, specref);

#ifdef DEBUG_SPECULAR
   vec_prn3(stderr, "specreff", specref);
#endif
    
   if (vec_dot3(specref, specref) > 0.0) {
        double specint[3] = {0.0, 0.0, 0.0};
        vec_reflect3(dir, closest->normal, ref_dir);       
#ifdef DEBUG_SPECULAR
   vec_prn3(stderr, "ref_dir", ref_dir);
#endif
        ray_trace(model, closest->hitloc, ref_dir, specint,
                                        total_dist, closest);
        specref[0] = specref[0] * specint[0];
        specref[1] = specref[1] * specint[1];
        specref[2] = specref[2] * specint[2];
   }

   vec_sum3(intensity, specref, intensity);
   // end specular...
}

/**
 * Returns the closest object that the ray hits.
 *
 * PARAMETERS:
 *  scene   - list of objects in the scene
 *  base    - origin of ray
 *  dir     - direction of ray
 *  last_hit- location of the ray's last hit
 *  mindist - distance ray has traveled
 * 
 * RETURNS:
 *  the closest object that the ray hits
 */
obj_t *find_closest_obj(list_t *scene, double base[3], 
                        double dir[3], obj_t *last_hit, double *mindist) {

    obj_t *closest = NULL;      // closest object that ray hits
    obj_t *obj = scene->head;   // first object
    double temp;                // temp holder to compare distances
    
    // have to initialize this to an invalid value, else this won't work
    // if there are no hit objects
    *mindist = -1;

    while (obj != NULL) {
        temp = obj->hits(base, dir, obj);
#ifdef DEBUG_CLOSEST
        fprintf(stderr, "found hit, th=%lf\n", temp);
#endif
        if (temp > 0) {
            if (temp < *mindist || closest == NULL) {
                if (last_hit == NULL) {
                    closest = obj;
                    *mindist = temp;
#ifdef DEBUG_CLOSEST
                fprintf(stderr, "found new closest, id=%d\n", closest->objid);
#endif
                } else if (last_hit->objid != obj->objid) {
                    closest = obj;
                    *mindist = temp;
#ifdef DEBUG_CLOSEST
                fprintf(stderr, "found new closest, id=%d\n", closest->objid);
#endif
                }

            }
        }

        obj = obj->next;
    }
    return closest;
}

/*
 * For a given object, checks against each light for diffuse lighting
 *
 * PARAMETERS:
 *  model     - struct holding all the lights and objects
 *  hitobj    - object that was hit and to check for diffusion
 *  intensity - pixel values vector
 */
void diffuse_illumination(model_t *model, obj_t *hitobj, double *intensity) {
    obj_t *light = model->lights->head;
    int    accumulator = 0;
    while (light != NULL) {
        accumulator += process_light(model->scene, hitobj, light, intensity);
        light = light->next;
    }
}


/**
 * Checks diffuse lighting at a specific point on a specific object for a 
 * specific light.  Checks to see if there is any occlussion and applies
 * diffuse lighting to pixel if needed.
 *
 * PARAMETERS:
 *  scene       - list of objects in the scene
 *  hitobj      - object that was hit and to check diffusion for
 *  light_obj   - light object to check
 *  intesnity   - vector describing the values of the pixel
 */
int process_light(list_t *scene, obj_t *hitobj, 
                  obj_t *light_obj, double *intensity) {
    double mindist;         // distance to nearest obj when checking occlussion
    obj_t *closest = NULL;  // closest object when checking for occlussion
    double dir[3];          // direction of ray from hitpt to light
    double diffuse[3];      // hold diffuse values for hitobj
    double dist;            // distance from hitpt to light
    double theta;           // cos(theta) where theta is b/w Normal and dir
    light_t *light;         // light to check
    
    // pull values out of hitobj and light_obj
    light = (light_t *)light_obj->priv;
    hitobj->getdif(hitobj, diffuse);

    // find direction and distance from hitpoint to light
    vec_diff3(hitobj->hitloc, light->center, dir);
#ifdef DEBUG_DIFFUSE
    vec_prn3(stderr, "direction vector was: ", dir);
#endif
    dist = vec_length3(dir);
    vec_unit3(dir, dir);
    


    // find cos(theta) where theta is the angle between the direction and
    // the normal at the hit point
    theta = vec_dot3(dir, hitobj->normal);
#ifdef DEBUG_DIFFUSE
    fprintf(stderr, "hit object id was: %d\n", hitobj->objid);
    vec_prn3(stderr, "hit point was:", hitobj->hitloc);
    vec_prn3(stderr, "normal at hitpoint: ", hitobj->normal);
    fprintf(stderr, "light object id was: %d\n", light_obj->objid);
    vec_prn3(stderr, "light center was: ", light->center);
    vec_prn3(stderr, "unit vector to light is: ", dir);
    fprintf(stderr, "distance to light is: %f\n", dist);
    fprintf(stderr, "cosine(theta) is: %f\n", theta); 
    fprintf(stderr, "distance to closest: %f\n", mindist);
#endif   


    // check for self occlusion (if cos(theta) < 0)
    if (theta < 0) {
#ifdef DEBUG_DIFFUSE
        fprintf(stderr, "Self-occluding\n");
#endif
        return -1;
    }
    
    // find the closest object in the direction of the light to check for
    // occlussion
    closest = find_closest_obj(scene, hitobj->hitloc, dir, hitobj, &mindist);

    
    // check to make sure light isn't occluded by some other object
    if (mindist < dist && mindist != -1) {
#ifdef DEBUG_DIFFUSE
        fprintf(stderr, "Found occluding object\n");
#endif
        return -1;
    // apply diffuse lighting to pixel
    } else {
        *(intensity + 0) += (diffuse[0] * light->emissivity[0] * theta) / dist;
        *(intensity + 1) += (diffuse[1] * light->emissivity[1] * theta) / dist;
        *(intensity + 2) += (diffuse[2] * light->emissivity[2] * theta) / dist;

#ifdef  DEBUG_DIFFUSE
        vec_prn3(stderr, "Emissivity of the light: ", light->emissivity);
        vec_prn3(stderr, "Diffuse reflectivity: ", diffuse);
        vec_prn3(stderr, "Current intensity: ", intensity);
#endif
    }
    return EXIT_SUCCESS;
}
