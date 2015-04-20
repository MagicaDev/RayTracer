/* Jacob Hamrick
	jfhamri
	CPSC 102 Section 1
	April 3rd, 2015

	Updated for MP3  - vector.c
*/
/*  This file contains function definitions for    */
/*  for 3D vector operations                       */

/* Updated for MP3 on April 16th, 2015 by Stuart Jackson, sljacks */

/**  vector.c   **/

#include "vector.h"

/* Compute the sum of two vectors */
/* v3 = v2 + v1; */
void vec_sum(vec_t *v1, vec_t *v2, vec_t *v3)
{
   v3->x = (*v2).x + (*v1).x;
   v3->y = (*v2).y + (*v1).y;
   v3->z = (*v2).z + (*v1).z;
}

/* Compute the difference of two vectors */
/* v3 = v2 - v1 */
void vec_diff(vec_t *v1, vec_t *v2, vec_t *v3)
{
   v3->x = (*v2).x - (*v1).x;
   v3->y = (*v2).y - (*v1).y;
   v3->z = (*v2).z - (*v1).z;
}

/* Compute the componentwise product of two vectors */
/* v3 = v2 * v1; */
void vec_mult(vec_t *v1, vec_t *v2, vec_t *v3)
{
   v3->x = (*v2).x*(*v1).x;
   v3->y = (*v2).y*(*v1).y;
   v3->z = (*v2).z*(*v1).z;
}

/* Return the (dot) inner product of two input vectors     */
double vec_dot(vec_t *v1, vec_t *v2)
{
   return (((*v1).x)*((*v2).x) + ((*v1).y)*((*v2).y) + ((*v1).z)*((*v2).z));
}

/* Return length of a 3d vector */
double vec_len(vec_t *v1)  /* Vector whose length is desired */
{
   return sqrt(pow((*v1).x,2) + pow((*v1).y,2) + pow((*v1).z,2));
}

/* Scale a 3d vector by multiplying each element by scale factor */
void vec_scale(double fact, vec_t *v1, vec_t *v2)      /* Output vector */
{
   v2->x = (*v1).x*fact;
   v2->y = (*v1).y*fact;
   v2->z = (*v1).z*fact;
}

/* Construct a unit vector v2 in direction of input vector v1 */
void vec_unit(
vec_t *v1,    /* Input vector      */
vec_t *v2)    /* output unit vec   */
{
   double scale = 1.0/vec_len(v1);
   vec_scale(scale,v1,v2);
}

/* Copy vector v1 to v2 another */
void vec_copy(
vec_t *v1,   /* input vector */
vec_t *v2)   /* output vector */
{
   v2->x = (*v1).x;
   v2->y = (*v1).y;
   v2->z = (*v1).z;
}

/* Read in values of vector from file */
/* Return the # of items read	*/
int vec_read(
FILE *in,     /* input file  */
vec_t *v1)    /* vector from input file */
{
   int numRead = 0;;
   numRead = fscanf(in,"%lf %lf %lf",&(*v1).x, &(*v1).y, &(*v1).z);
   if(numRead != 3)
   {
      fprintf(stdout, "\n\n **** There was an error reading 3 inputs **** \n\n");
      exit(0);
   }	
   return numRead;
}

/* Print values of vector to file */
void vec_print(
FILE *out,     /* output file */
char *label,   /* label string */
vec_t *v1)      /* vector to print */
{
   fprintf(out,"%s  %8.3f  %8.3f  %8.3f\n ",label,v1->x, v1->y, v1->z);
}

/* vec_cross  */
void   vec_cross(
  vec_t *v1,      /* Left input vector  (V)*/
  vec_t *v2,      /* Right input vector (W)*/
  vec_t *v3)     /* Output vector      */
{
   vec_t temp;
   temp.x = (v1->y * v2->z) - (v1->z * v2->y);
   temp.y = (v1->z * v2->x) - (v1->x * v2->z);
   temp.z = (v1->x * v2->y) - (v1->y * v2->x);
   vec_copy(&temp,v3);
}

/* project a vector onto a plane */
void vec_project(
  vec_t *n,        /* plane normal      */ 
  vec_t *v,        /* input vector      */
  vec_t *p)       /* projected vector  */
{
   vec_t temp;
   vec_scale(vec_dot(n,v), n, &temp);	
   vec_diff(&temp,v,p);
}   

/* reflect a vector from a surface plane  */
void vec_reflect(
  vec_t *n,        /* surface normal      */
  vec_t *w,        /* incoming ray vector */
  vec_t *v)       /* reflected vector    */
{
   //V = [2N(UdotN)] - U
   vec_t u;
   vec_t temp;

   vec_scale(-1.0,w,&u);		    //U
   double scale = 2*(vec_dot(&u,n));  //2(UdotN)
   vec_scale(scale,n,&temp);

   vec_diff(&u,&temp,v);
}

/* Apply transform matrix to vector */
void vec_xform(
  mtx_t *m,		//input matrix
  vec_t *v1,		//vector to be transformed
  vec_t *v2)		//output vector
{
   vec_t temp;
   temp.x = vec_dot(&(m->row[0]),v1);
   temp.y = vec_dot(&(m->row[1]),v1);
   temp.z = vec_dot(&(m->row[2]),v1);

   vec_copy(&temp,v2);
}
   
/* Compute the transpose of a matrix */
void mtx_transpose(
  mtx_t *m1,             
  mtx_t *m2)
{
	mtx_t m3;
	
   m3.row[0].x = m1->row[0].x;
   m3.row[0].y = m1->row[1].x;
   m3.row[0].z = m1->row[2].x;

   m3.row[1].x = m1->row[0].y;
   m3.row[1].y = m1->row[1].y;
   m3.row[1].z = m1->row[2].y;

   m3.row[2].x = m1->row[0].z;
   m3.row[2].y = m1->row[1].z;
   m3.row[2].z = m1->row[2].z;

   vec_copy(&(m3.row[0]),&(m2->row[0]));
   vec_copy(&(m3.row[1]),&(m2->row[1]));
   vec_copy(&(m3.row[2]),&(m2->row[2]));
}


