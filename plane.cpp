/* Jacob Hamrick
	jfhamri
	CPSC 102 Section 1
	April 3rd, 2015

	Updated for MP3 - plane.cpp  
*/

#include "ray.h"

static pparm_t pln_parse[] = 
{
   {"point",	3, sizeof(double), "%lf", 0},
   {"normal",	3, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(pln_parse) / sizeof(pparm_t))

plane_t::plane_t(FILE *in, model_t *model, int attrmax):object_t(in, model)
{
   int mask;
   strcpy(obj_type, "plane");

   pln_parse[0].loc = &point;
   pln_parse[1].loc = &normal;
   mask = parser(in, pln_parse, NUM_ATTRS, attrmax);
   assert(mask == 3);
printf("\nnormal %lf %lf %lf", normal.x, normal.y, normal.z);
   vec_unit(&normal, &normal);
   vec_copy(&normal, &last_normal);

   ndotq = vec_dot(&point, &normal);
}

double plane_t::hits(vec_t *base, vec_t *dir)
{
   double ndotd;
   double t;
   double ndotb;

   ndotq = vec_dot(&normal, &point);
   ndotd = vec_dot(dir, &normal);

   if(ndotd == 0)
      return(-1);

   ndotb = vec_dot(&normal, base);
   t = (ndotq - ndotb) / ndotd;
   if(t <= 0)
      return(-1);

   vec_scale(t, dir, &last_hitpt);
   vec_sum(&last_hitpt, base, &last_hitpt);
   
   vec_copy(&normal, &last_normal);
   if((last_hitpt.z > 0.01) && (strcmp(obj_type, "projector")))
      return(-1);

   return(t);
}

void plane_t::printer(FILE *out)
{
   object_t::printer(out);

   fprintf(out, "%-12s %5.1f %5.1f %5.1f\n", "normal", normal.x
							  , normal.y
							  , normal.z);
   fprintf(out, "%-12s %5.1f %5.1f %5.1f\n", "point", point.x
							 , point.y
							 ,point.z);
}
