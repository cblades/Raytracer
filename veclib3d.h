#ifndef VECLIB3D_H
#define VECLIB3D_H

double vec_dot3(double *, double *);

void   vec_scale3(double, double *, double *);

double vec_length3(double *);

void   vec_diff3(double *, double *, double *);

void   vec_sum3(double *, double *, double *);

void   vec_unit3(double *, double *);

void   vec_prn3(FILE *, char *, double *);

void vec_read3(FILE *, double *);

void vec_matmul3(double left[][3], double right[][3], double out[][3]);

void xpose3(double in[][3], double out[][3]);

void vec_cross3(double *, double *, double *);

void xform3(double y[][3], double *, double *);

void vec_project3(double *, double *, double *);

void vec_reflect3(double *, double *, double *);
#endif
