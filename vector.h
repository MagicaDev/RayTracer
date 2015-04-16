/** vector.h  **/

/* This header file contains vector definitions and     */
/* prototypes for 3-D vector operations                 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/*  Three dimensional coordinate structures   */

#define X 0
#define Y 1
#define Z 2

typedef struct vec_type
{
   double x;
   double y;
   double z;
} vec_t;

typedef struct matrix_type
{
   vec_t row[3];
} mtx_t;

/**  prototypes   **/

void   vec_sum(vec_t *, vec_t *, vec_t *);
void   vec_mult(vec_t *, vec_t *, vec_t *);
void   vec_diff(vec_t *, vec_t *, vec_t *);
double vec_dot(vec_t *, vec_t *);
double vec_len(vec_t *);
void   vec_scale(double, vec_t *, vec_t *);
void   vec_unit(vec_t *, vec_t *);
void   vec_copy(vec_t *, vec_t *);
int    vec_read(FILE *, vec_t *v1);
void   vec_print(FILE *, char *, vec_t *);
