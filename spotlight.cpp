// Stuart Jackson, sljacks, 1020-001, 4/18/2015, Major Assignment #3,
// with Jacob Hamrick, spotlights

#include "ray.h"

static pparm_t spt_parse [] =
{
	{"point",	3, sizeof(double), "%lf", 0},
	{"theta",	1, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(spt_parse) / sizeof(pparm_t))

// Similar to other constructors, initializes the spotlight_t
spotlight_t::spotlight_t
	(FILE *in,
	model_t *model,
	int attrmax)	
		: light_t(in, model, 2)
{
	int mask;
	double radtheta;	// Used to store theta in radians

	spt_parse[0].loc = &point;
	spt_parse[1].loc = &theta;
	mask = parser(in, spt_parse, NUM_ATTRS, attrmax);
	assert(mask == 3);

//	creates a unit vector in the centerline direction
	vec_diff(&location, &point, &direction);
	vec_unit(&direction, &direction);

//	Converts theta from degrees to radians
	radtheta = theta * (M_PI / 180);

//	Finds the cosine of theta
	costheta = cos(radtheta);
}

//	Prints off the spotlight attributes
void spotlight_t::printer(FILE *out)
{
//	This prints the generic attributes
	light_t::printer(out);

	fprintf(out, "%-12s %5.1f %5.1f %5.1f\n", "direction",
		direction.x, direction.y, direction.z);

	fprintf(out, "%-12s %5.1f\n", "halfwidth",
		theta);
}

//	Calculates where the spotlight hits using the angles made by the light
//	vectors
int spotlight_t::vischeck(vec_t *hitloc)	//*hitloc = last hit location
{
	vec_t ulocation;	// the location vector as a unit vector
	double ddir;		// holds the dot product
printf("test\n");
//	a unit vector from the location of the spotlight to the hit location
	vec_diff(&location, hitloc, &ulocation);
	vec_unit(&ulocation, &ulocation);
printf("unit location %lf %lf %lf", ulocation.x, ulocation.y, ulocation.z);
	ddir = vec_dot(&ulocation, &direction);

//	If the dot product of the direction vector is greater than costheta,
//	then the hit location is illuminated
	if(ddir > costheta)
	{
		return 1;
	}

	else
	{
		return 0;
	}
}
