/* Jacob Hamrick
	jfhamri
	CPSC 102 Section 1
	April 3rd, 2015

	Updated for MP3 - camera.cpp
*/

#include "ray.h"
//private vars
// - cookie
// - name
// - pixel_dim[2]
// - world_dim[2]
// - view_point  (vec_t)
// - irgb_t *pixmap

static pparm_t cam_parse[] = 
{
   {"pixeldim",	2, sizeof(int), "%d", 0},
   {"worlddim",	2, sizeof(double), "%lf", 0},
   {"viewpoint",	3, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(cam_parse) / sizeof(pparm_t))

camera_t::camera_t(FILE *in)
{
   char buf[256];
   int mask;

   assert(fscanf(in, "%s", name) == 1);
   fscanf(in, "%s", buf);
   assert(buf[0] == '{');

   cookie = CAM_COOKIE;
   cam_parse[0].loc = &pixel_dim;
   cam_parse[1].loc = &world_dim;
   cam_parse[2].loc = &view_point;
	
   mask = parser(in, cam_parse, NUM_ATTRS, 0);
   assert(mask != 0);
	
   pixmap = (irgb_t *)malloc(sizeof(irgb_t) * pixel_dim[0] 
						 * pixel_dim[1]);
}

void camera_t::getdir(int x, int y, vec_t *dir)
{
   vec_t tmp;

   double dx = x;
   double dy = y;
   
   if(AA_SAMPLES > 1)
   {
      dx = randomize(dx);
      dy = randomize(dy);
   }
   
   tmp.x = world_dim[X] * dx / (pixel_dim[X]-1);
   tmp.y = world_dim[Y] * dy / (pixel_dim[Y]-1);
   tmp.z = 0.0;

   vec_copy(&tmp,dir);
   vec_diff(&view_point, dir, dir);
   vec_unit(dir, dir);
}

double camera_t::randomize(double num)
{
   double rannum;
   rannum = (double)(random());
   rannum = rannum / 0x7fffffff;
   rannum -= 0.5;
   return(rannum + num);
}

void camera_t::store_pixel(int x, int y, drgb_t *pix)
{
   int maprow;
   irgb_t *maploc;

   maprow = pixel_dim[1] - y - 1;
   maploc = pixmap + maprow * pixel_dim[0] + x;

   scale_and_clamp(pix);

   maploc->r = (unsigned char)pix->r;
   maploc->g = (unsigned char)pix->g;
   maploc->b = (unsigned char)pix->b;
}   

int camera_t::getxdim(void)
{
   return pixel_dim[0];
}

int camera_t::getydim(void)
{
   return pixel_dim[1];
}

void camera_t::getviewpt(vec_t *view)
{
   vec_copy(&view_point,view);
}

void camera_t::camera_print(FILE *out)
{
   fprintf(out, "%-12s %s\n", "camera", name);
   fprintf(out, "%-12s %5.1d %5.1d\n", "pixeldim", pixel_dim[X]
						      , pixel_dim[Y]);
   fprintf(out, "%-12s %5.1f %5.1f\n", "worlddim", world_dim[X]
						      , world_dim[Y]);
   fprintf(out, "%-12s %5.1f %5.1f %5.1f\n", "viewpoint", view_point.x
							     , view_point.y
							     , view_point.z);
}

void camera_t::camera_write_image(FILE *out)
{
   fprintf(out, "P6\n");
   fprintf(out, "%d %d %d\n", pixel_dim[X], pixel_dim[Y], 255);

   fwrite(pixmap, sizeof(irgb_t), 
		((pixel_dim[0])*(pixel_dim[1])), out);
}

void camera_t::getpixsize(double *x, double *y)
{
   *x = (double)pixel_dim[X];
   *y = (double)pixel_dim[Y];
}

void camera_t::scale_and_clamp(drgb_t *given)
{
   drgb_t temp;
   temp.r = (given->r*255) + 0.5;
   temp.g = (given->g*255) + 0.5;
   temp.b = (given->b*255) + 0.5;
   if(temp.r > 255.0)
      temp.r =255.0;
   else if(temp.r < 0.0)
      temp.r = 0.0;
   if(temp.g > 255.0)
      temp.g = 255.0;
   else if(temp.g < 0.0)
      temp.g = 0.0;
   if(temp.b > 255.0)
      temp.b = 255.0;
   else if(temp.b < 0.0)
      temp.b = 0.0;

   pix_copy(&temp,given);
}
