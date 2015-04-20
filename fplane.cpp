// Stuart Jackson, sljacks, 1020-001, 4/16/2015, Major Assignment # 3 with
// Jacob Hamrick, finite planes

#include "ray.h"

static pparm_t fpln_parse[] =
{
	{"xdir",	3, sizeof(double), "%lf", 0},
	{"dimensions",	2, sizeof(double), "%lf", 0},
};
#define NUM_ATTRS (sizeof(fpln_parse) / sizeof(pparm_t))

fplane_t::fplane_t(FILE *in, model_t *model, int attrmax) : plane_t(in,model,2)
{
	int mask;
	vec_t holder;

	strcpy(obj_type, "finite plane");

	fpln_parse[0].loc = &xdir;
	fpln_parse[1].loc = &dims;
	mask = parser(in, fpln_parse, NUM_ATTRS, attrmax);
	assert(mask == 3);

//	Project xdir onto a plane, creating projxdir
printf("\nnormal %lf %lf %lf", normal.x, normal.y, normal.z);
printf("\nxdir %lf %lf %lf", xdir.x, xdir.y, xdir.z);
printf("\npoint %lf %lf %lf", point.x, point.y, point.z);
printf("\ndimensions %lf %lf", dims[0], dims[1]);
	vec_reflect(&normal, &xdir, &holder);
	vec_copy(&holder, &projxdir);
printf("\nprojxdir %lf %lf %lf\n", projxdir.x, projxdir.y, projxdir.z);
	assert((abs(projxdir.x) + abs(projxdir.y) + abs(projxdir.z)) != 0.0);

//	Makes projxdir unit lengthi
	vec_unit(&projxdir, &holder);
	vec_copy(&holder, &projxdir);
printf("projxdir unit %lf %lf %lf\n", projxdir.x, projxdir.y, projxdir.z);
//	Creates a matrix that can rotate projxdir into the x-axis and the
//	plane normal into the positive z-axis
	vec_copy(&projxdir, &rot.row[0]);
	vec_copy(&normal, &rot.row[2]);
	vec_unit(&rot.row[2], &holder);
	vec_copy(&holder, &rot.row[2]);
	vec_cross(&rot.row[2], &rot.row[0], &holder);
	vec_copy(&holder, &rot.row[1]);
}

void fplane_t::printer(FILE *out)
{
	plane_t::printer(out);

	fprintf(out, "%-12s %5.1f %5.1f %5.1f\n", "xdir",
		xdir.x, xdir.y, xdir.z);
	fprintf(out, "%-12s %5.1f %5.1f", "dimensions",
		dims[0], dims[1]);
}

// Determines if the infinite plane has been hit, then determines if the
// finite plane has been hit
double fplane_t::hits(
	vec_t *base,	/* ray base		 */
	vec_t *dir)	/* unit direction vector */
{
	vec_t newloc;
	double t;

	t = plane_t::hits(base, dir);
	assert(t != -1);
printf("point %lf %lf %lf\n", point.x, point.y, point.z);
printf("last_hitpt %lf %lf %lf\n", last_hitpt.x, last_hitpt.y, last_hitpt.z);
	vec_diff(&point, &last_hitpt, &newloc);
	vec_xform(&rot, &newloc, &newloc);
printf("newloc %lf %lf", newloc.x, newloc.y);
	if ((0 <= newloc.x and newloc.x <= dims[0]) and
	(0 <= newloc.y and newloc.y <= dims[1]))
	{
		return t;
	}

	else
	{
		return -1;
	}
}
