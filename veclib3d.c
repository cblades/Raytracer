/**
 * Definition of functions to perform various operations on 3d vectors.
 *
 * Chris Blades
 * Feb. 18, 2010
 */
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "veclib3d.h"

/**
 * Returns the dot product of two vectors:
 * P dot Q = P(x) * Q(x) + P(y) * Q(y) + P(z) * Q(z)
 *
 * PARAMETERS:
 *  vecA - one of the vectors to use in dot product
 *  vecB - other vector to use in dot product
 *
 *  RETURN:
 *  the dot product of vecA and vecB
 */
double vec_dot3(double *vecA, double *vecB) {
    double product;

    product = ((*vecA) * (*vecB)) +
              (*(vecA + 1) * *(vecB + 1)) +
              (*(vecA + 2) * *(vecB + 2));

    return product;
}

/*
 * Scales a given vector by the given factor, and stores the resulting vector
 * in the given output vector.
 * aP = (P(X) * a, P(Y) * a, P(Z) * a)
 *
 * PARAMETERS:
 *  factor  - scalor value to multiply vector by
 *  input   - the vector to scale
 *  output  - where to store the resulting scaled vector
 */
void   vec_scale3(double factor, double *input, double *output) {
    double x;
    double y;
    double z;

    x = *input * factor;
    y = *(input + 1) * factor;
    z = *(input + 2) * factor;

    *output        = x;
    *(output + 1) = y;
    *(output + 2) = z;
}

/*
 * Returns the magnitude or length of the given vector.
 *  |P| = sqrt(P(x)^2 + P(Y)^2 + P(Z)^2)
 * 
 * PARAMETERS:
 *  vec - vector to find the magnitude of.
 *
 *  RETURN:
 *  the magnitude of vec
 */
double vec_length3(double *vec) {
    double length;
    double xSqr;
    double ySqr;
    double zSqr;

    // TODO: get rid of magic number 2
    xSqr = pow(*vec, 2);
    ySqr = pow(*(vec + 1), 2);
    zSqr = pow(*(vec + 2), 2);

    length = sqrt(xSqr + ySqr + zSqr);
    
    return length;
}

/*
 * Finds the difference between two given vectors and stores the resulting
 * vector at the given location.
 *
 * PARAMETERS:
 *  subtract - the vector to subtract (right side of operation)
 *  from     - the vector to subtract from
 *  output   - where to store the resulting vector
 */
void   vec_diff3(double *subtract, double *from, double *output) {
    double diffX;
    double diffY;
    double diffZ;

    diffX = *from - *subtract;
    diffY = *(from + 1) - *(subtract + 1);
    diffZ = *(from + 2) - *(subtract + 2);

    *output       = diffX;
    *(output + 1) = diffY;
    *(output + 2) = diffZ;
}

/**
 * Finds the sum of two given vectors and stores the resulting vector at the 
 * given location.
 *
 * PARAMETERS:
 *  vecA    - vector to sum
 *  vecB    - other vector to sum
 *  output  - where to store the resulting vector
 */
void   vec_sum3(double *vecA, double *vecB, double *output) {
    double sumX;
    double sumY;
    double sumZ;

    sumX = *vecA + *vecB;
    sumY = *(vecA + 1) + *(vecB + 1);
    sumZ = *(vecA + 2) + *(vecB + 2);

    *output       = sumX;
    *(output + 1) = sumY;
    *(output + 2) = sumZ;

}

/*
 * Convert a given vector into a unit vector and store the resulting vector in
 * the given location.
 * U = (1/||P||)P
 *
 * PARAMETERS:
 *  input   - vector to convert to a unit vector
 *  output  - where to store the resulting unit vector
 */
void   vec_unit3(double *input, double *output) {
    double length;

    length = vec_length3(input);
    
    // if the length of the input vector is 0, this operation is undefined, 
    // do nothing
    if (length == 0) {
        return;
    }

    vec_scale3((1/length), input, output);
}

/*
 * Print a vector with the given lable in format:
 *
 * <label> = (P(x), P(y), P(z))
 *
 * PARAMETERS:
 *  label   - label to print out with the vector
 *  vector  - the vector to print
 */
void   vec_prn3(FILE *out, char *label, double *vector) {
    fprintf(out, "%s = (%f, %f, %f)\n", label, *vector, 
                                               *(vector + 1), 
                                               *(vector + 2));
}

/*
 * Read in a 3 dimensional vector
 *
 * PARAMETERS:
 *  in  - input file
 *  vec - where to store the vector
 */
void vec_read3(FILE *in, double *vec) {
    int rc = 0;
    char buf[256];

    do {
        fgets(buf, 256, in);
        rc = sscanf(buf, "%lf %lf %lf", vec, (vec + 1), (vec + 2));
    } while (rc != 3);
}

/*
 * 3 x 3 matrix multiplier.
 *
 * PARAMETERS:
 *  x   -   left input vector
 *  y   -   right input vector
 *  out -   output matrix
 */
void vec_matmul3(double x[][3], double y[][3],
                                        double out[][3]) {
   int i;
   int j;
   double temp[3];
    
   for(i = 0; i < 3; i++) {
      for(j=0; j < 3; j++) {
         temp[0] = y[0][j];
         temp[1] = y[1][j];
         temp[2] = y[2][j];

         out[i][j] = vec_dot3(x[i], temp);
      }
   }

}

/*
 * 3 x 3 matrix transpose
 *
 * PARAMETERS:
 *  x  -   original matrix
 *  z  -   output matrix
 */
void xpose3(double x[][3], double z[][3]) {
    double temp;
    int i, j;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            temp = x[i][j];
            z[i][j] = x[j][i];
            z[j][i] = temp;
        }
    }
}

/*
 * Compute the outer product of two input vectors
 *
 * PARAMETERS:
 *  v1    - v1 vector
 *  v2   - v2 vector
 *  out     - output vector
 */
void vec_cross3(double *v1, double *v2, double *out) {
    double *temp = (double *)alloca(sizeof(double) * 3);

    *(temp + 0) = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    *(temp + 1) = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    *(temp + 2) = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    *(out + 0) = *(temp + 0);
    *(out + 1) = *(temp + 1);    
    *(out + 2) = *(temp + 2);
}

/*
 * Perform a linear transform in four dimensional space
 * by applying a 3 x 3 matrix to a 3 x 1 column vector.
 *
 * PARAMETERS:
 *  y   - transform matrix
 *  x   - input vector
 *  z   - output vector
 */
void xform3(double y[][3], double *x, double *z) {
    double *temp = (double *)alloca(sizeof(double) * 3);

    *(temp + 0) = vec_dot3(y[0], x);
    *(temp + 1) = vec_dot3(y[1], x);
    *(temp + 2) = vec_dot3(y[2], x);

    *(z + 0) =  *(temp + 0); 
    *(z + 1) =  *(temp + 1);
    *(z + 2) =  *(temp + 2); 
}

/*
 * Projct a vector onto a plane
 *
 * PARAMETERS:
 *  normal  -   plane normal
 *  in      -   input vector
 *  proj    -   projected vector
 */
void vec_project3(double *normal, double *in, double *proj) {
   double temp[3];
   double NdotIn = vec_dot3(normal, in);

   vec_scale3(NdotIn, normal, temp);
   vec_diff3(temp, in, proj);
}


/*
 * Finds the direction that is a reflection of unitin across unitnorm.
 *
 * PARAMETERS:
 *  unitin   - unit vector input direction
 *  unitnorm - unit vector normal
 *  unitout  - unit vector output direction
 */
void vec_reflect3(double *unitin, double *unitnorm, double *unitout) {

   double temp[3];
   double UdotN;
   double U[3];

   vec_scale3(-1, unitin, U);

   UdotN = (vec_dot3(U, unitnorm));
   
   vec_scale3(UdotN, unitnorm, temp);
   vec_scale3(2, temp, temp);
   vec_diff3(U, temp, unitout);
}
