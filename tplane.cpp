/* Jacob Hamrick
	jfhamri
	CPSC 102 Section 1
	April 3rd, 2015

	Updated for MP3  - tplane.cpp 
*/

/* tplane functions:
	tplane_t()
	printer()
	hits() 
	getambient()
*/

#include "ray.h"

static pparm_t tpln_parse[] = 
{
   {"altmaterial",	1, sizeof(char[NAME_LEN]), "%s", 0},
   {"dimension",	2, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(tpln_parse) / sizeof(pparm_t))


tplane_t::tplane_t(FILE *in, model_t *model, int attrmax):plane_t(in,model,2)
{   
   int mask; 
   strcpy(obj_type, "tiled plane");

   char matname[NAME_LEN];
   tpln_parse[0].loc = &matname;
   tpln_parse[1].loc = &dims;
   mask = parser(in, tpln_parse, NUM_ATTRS, attrmax);
   assert(mask == 3);

   altmat = material_getbyname(model, matname);
}

void tplane_t::printer(FILE  *out)
{
   plane_t::printer(out);

   fprintf(out,"%-12s %5.1f %5.1f\n", "dimension",
      dims[0], dims[1]);
   fprintf(out,"%-12s %5.1s\n", "altmaterial",altmat->material_getname());
}

void tplane_t::getambient(drgb_t *value)
{
   int foreground = 0;


   int x_ndx = (last_hitpt.x+10000)/dims[0];
   int z_ndx = (last_hitpt.z+10000)/dims[1];
	
   if(((x_ndx+z_ndx) & 1) == 0)
		foreground = 1;

   if(foreground)
      plane_t::getambient(value);
   else
      altmat->material_getambient(value);
}

