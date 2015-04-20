/* light.cpp */

#include "ray.h"

static pparm_t lght_parse[] = 
{
   {"location",	    3, sizeof(double), "%lf", 0},
   {"emissivity",   3, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(lght_parse) / sizeof(pparm_t))

light_t::light_t(FILE *in, model_t *model, int attrmax)
{
   char hold[NAME_LEN];
   int count;
   isspotlight = 0;

   count = fscanf(in, "%s", name);
   assert(count == 1);

   count = fscanf(in, "%s", hold);
   assert(*hold == '{');

   int mask;   
   lght_parse[0].loc = &location;
   lght_parse[1].loc = &emissivity;
   mask = parser(in, lght_parse, NUM_ATTRS, attrmax);
   assert(mask == 3);
   //add to the lights list
   model->lgts->add((void *)this);
}

void light_t::getemiss(drgb_t *ret)
{
   ret->r = emissivity.r;
   ret->g = emissivity.g;
   ret->b = emissivity.b;
}

void light_t::getloc(vec_t *loc)
{
   vec_copy(&location, loc);
}

void light_t::illuminate(model_t *model, vec_t *base, object_t *hitobj, 
   drgb_t *pixel)
{
   vec_t dir; 		//unit direction from light to hitpt
   object_t *obj;	//closest object in dir to light
   double close;	//dist to closest object in dir from light
   double cos;		//cos of angle between normal and dir to light
   double dist;		//distance from light to hitpt
   vec_t lasthit;	//from hitobj argument
   vec_t lastnorm;	//from hitobj argument
   drgb_t diffuse = {0.0, 0.0, 0.0};

   hitobj->getlast_hitpt(&lasthit);
   vec_diff(&lasthit, &location, &dir);
   dist = vec_len(&dir);
   vec_unit(&dir, &dir);
   
   hitobj->getlast_normal(&lastnorm);
   
   cos = vec_dot(&dir, &lastnorm);
   if(cos <= 0.0)
      return;

   if (vischeck(&lasthit) == 0 and isspotlight == 1)
      return;
   
   obj = (object_t *)find_closest_object(model, &lasthit, &dir, hitobj
         , &close);
   if((obj != NULL) && (close < dist))
      return;
   
   hitobj->getdiffuse(&diffuse);
    
   pix_prod(&diffuse, &emissivity, &diffuse);
   pix_scale((cos/dist),&diffuse, &diffuse);
   pix_sum(pixel, &diffuse, pixel);
   
   //at this point, pixels are right value
}

void light_t::printer(FILE *out)
{
   fprintf(out,"\n%-12s %s \n", "light" ,name);
   fprintf(out, "%-12s %5.1f %5.1f %5.1f\n", "location", location.x, 
      location.y, location.z);
   fprintf(out, "%-12s %5.1f %5.1f %5.1f\n", "emissivity", emissivity.r, 
      emissivity.g, emissivity.b);
}

void light_t::add_glint(object_t *hitobj, vec_t *base, 
      vec_t *dir, drgb_t *pixel)
{
   double shine;
   hitobj->getshine(&shine);
   if(shine == 0.0)
   {
      
   }
}

void light_list_print(model_t *model, FILE *out)
{
   light_t *light;
   list_t *list = model->lgts;

   list->reset();
   while(list->not_end())
   {
      light = (light_t *)list->get_entity();
      light->printer(out);
      fprintf(out, "\n");
      list->next_link();
   } 
}

int light_t::vischeck(vec_t *hitloc)
{
	return 0;
}
