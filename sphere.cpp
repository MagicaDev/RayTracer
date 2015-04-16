/* Jacob Hamrick
	jfhamri
	CPSC 102 Section 1
	April 3rd, 2015

	Updated for MP3 - sphere.cpp  
*/

#include "ray.h"

static pparm_t sph_parse[] = 
{
   {"center",	3, sizeof(double), "%lf", 0},
   {"radius",	1, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(sph_parse) / sizeof(pparm_t))

sphere_t::sphere_t(FILE *in, model_t *model, int attrmax):object_t(in,model)
{
   int mask;
   strcpy(obj_type, "sphere");
	
   sph_parse[0].loc = &center;
   sph_parse[1].loc = &radius;
   mask = parser(in, sph_parse, NUM_ATTRS, attrmax);
   assert(mask == 3);
}

double sphere_t::hits(vec_t *base, vec_t *dir)
{
   vec_t v;	//V - viewpoint / start of ray
   vec_t vp;	//Vprime - new base of ray
   vec_t c;	//C - center of sphere
   vec_t cp;	//Cprime - new center of sphere
   vec_t d;	//D - UV in direction ray is traveling
   vec_t h;	//Hitpoint
   vec_t norm;	
   vec_t num;	
   vec_t temp;	
   double r = 0.0;	//Radius
   double qa,qb,qc;	//abc of Quadratic equation
   double dist;	//t sub h distance
   double fac;

   //Setting Values
   vec_copy(base,&v);
   vec_copy(&center,&c);
   vec_copy(dir, &d);
   r = radius;

   vec_diff(&c, &c, &cp);
   vec_diff(&c, &v,&vp);

   qa = vec_dot(&d,&d);
   qb = 2*(vec_dot(&vp,&d));
   qc = (vec_dot(&vp,&vp)) - (r*r);

   double disc = (qb*qb)-(4.0 * qa * qc);
   if(disc > 0.0)
   {
      dist = (-qb-sqrt(disc))/(2*qa);

      vec_scale(dist,&d,&temp);
      vec_sum(&v,&temp,&h);
		
      //copy to lasthit
      vec_copy(&h,&last_hitpt);

      //getting the normal
      vec_diff(&c, &h, &norm);
      fac = 1/(vec_len(&norm));
      vec_diff(&c ,&h ,&num);
      vec_scale(fac, &num, &norm);

      //copy to last normal
      vec_copy(&norm,&last_normal);
		
      return dist;
   }
   else
      return(-1);
}

void sphere_t::printer(FILE *out)
{
   object_t::printer(out);
	
   fprintf(out, "%-12s %5.1f %5.1f %5.1f\n", "center", center.x
							 , center.y
							 , center.z);
   fprintf(out, "%-12s %5.1f\n", "radius", radius);
}
