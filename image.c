/* Jacob Hamrick
	jfhamri
	CPSC 102 Section 1
	April 3rd, 2015

	Updated for MP3  - image.c

	Updated by Stuart Jackson on 4/20/2015
*/

/*  functions:
    	ray_trace() - code to add
		make_pixel() - complete function provided
		make_row() - code to add
		image_create() - code to add
*/

#include "ray.h"


//=============================================================================
// ******* ray_trace( ) *******
// 
// 
//=============================================================================
void ray_trace (
   model_t *model,
   vec_t *base,                // view point
   vec_t *dir,                 // unit direction vector
   drgb_t *pix,                // pixel return location
   double total_dist,          // distance ray has travelled so far
   object_t *last_hit)         // most recently hit object
{
   double mindist;
   drgb_t thispix = {0.0, 0.0, 0.0};
   object_t *closest;
   double specref = 0.0;

   if(total_dist > MAX_DIST)
      return;

   /* Get the object (plane or sphere).        */
   /* Later we will use find_closest_object()  */
   /* to get the closest object                */
   closest = (object_t *)find_closest_object(
   model,base,dir,last_hit,&mindist);

   /* get the distance and the color */
   if(closest != NULL)
   {
      mindist = closest->hits(base,dir);
      closest->getambient(&thispix);
   
      add_illumination(model, base, closest, &thispix);
   }

// See if object has specular reflectivity
   if(closest != NULL)
   {
      closest->getspecular(&specref);
   }
// The actual appearance of the reflection is determined here
   if(specref != 0)
   {
      drgb_t specint = {0.0, 0.0, 0.0};
      vec_t ref_dir;
      vec_t norm;
      vec_t last_ht;
 //     vec_t last_ht; // holds the last hit point

      closest->getlast_normal(&norm);
      vec_reflect(&norm, dir, &ref_dir);

      closest->getlast_hitpt(&last_ht);
//      closest->getlast_hitpt(&last_ht);

      ray_trace(model, &last_ht, &ref_dir, &specint, total_dist, closest);

      pix_scale(specref, &specint, &specint);

      pix_sum(&specint, &thispix, &thispix);
   }

   /**  check to see if object was hit.  if so
   1)  add this distance to the total distance travelled so far
   2)  scale thispix by 1/total_distance
   3)  add thispix to pix
   **/   
   //  CODE HERE
	
   if(mindist >= 0.0)
   {
      //THIS IS NOW CORRECT, NEVER CHANGE IT
      total_dist += mindist;
      pix_scale(1.0/total_dist, &thispix,&thispix);
      pix_sum(&thispix, pix, pix);
   }  
}


//=============================================================================
// ******* make_pixel( ) *******
// 
// 
//=============================================================================
void make_pixel(model_t  *model, int  x, int  y) 
{
   vec_t raydir;
   vec_t vp;
   drgb_t pix = {0.0, 0.0, 0.0};
   camera_t *cam = model->cam;

   cam->getdir(x, y, &raydir);
   cam->getviewpt(&vp);
	
   /*  The ray_trace function determines the pixel color in */
   /*  d_rgb units.   The last two parameters are used ONLY */
   /*  in the case of specular (bouncing) rays              */
   ray_trace(model, &vp, &raydir, &pix, 0.0, NULL);
   cam->store_pixel(x, y, &pix);
   return;
}


//=============================================================================
// ******* make_row( ) *******
//  
//=============================================================================
void make_row(model_t  *model, int  y) 
{
   int x;
   camera_t *cam = model->cam;
   int xdim = cam->getxdim();

   /*  for each pixel in the row, invoke make_pixel() */
   /*  to paint the pixel.                            */
   //  CODE HERE   
   for(x = 0; x < xdim; x++)
   {
      make_pixel(model, x, y);
   }

}


//=============================================================================
// ******* image_create( ) *******
// 
// 
//=============================================================================
void image_create(model_t  *model, FILE  *out) 
{
   int y;
   camera_t *cam = model->cam;
   int ydim = cam->getydim();

   /*  fire rays through each pixel in the window; */
   /*  for each row, invoke make_row() to paint    */
   /*  the row of pixels.                          */
   //  CODE HERE 
   for(y = 0; y < ydim; y++)
   {
      make_row(model, y);
   }
   /*  write the image                           */
   cam->camera_write_image(out);
}

void add_illumination(model_t *model, vec_t *base, object_t *hitobj, 
   drgb_t *pixel)
{
   light_t *light;
   list_t *list;
   
   list = model->lgts;
   list->reset();
   
   while(list->not_end())
   {
      light = (light_t *)(list->get_entity());
      light->illuminate(model, base, hitobj, pixel);
      list->next_link();
   }
}
