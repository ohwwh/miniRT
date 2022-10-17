#include "objects.h"

void set_refraction(t_object* obj, double ref)
{
	obj->refraction = ref;
}

t_object create_sphere(t_point c, double r, t_color color, int mat)
{
	t_object ret;

    ret.type = 3;
	ret.center = c;
	ret.radius = r;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	return (ret);
}

t_object create_cylinder(t_point c, double r, double h, t_vec dir, t_color color, int mat)
{
	t_object ret;

	ret.type = 2;
	ret.center = c;
	ret.radius = r;
	ret.height = h;
	ret.dir = dir;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	return (ret);
}

t_object create_plane(t_point c, t_vec dir, t_color color, int mat)
{
	t_object ret;

	ret.type = 1;
	ret.center = c;
	ret.dir = dir;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	return (ret);
}

t_object create_rectangle_xy(t_vec x, t_vec y, double k, t_color color, int mat)
{
	t_object ret;

	ret.type = 4;
	ret.center = x;
	ret.dir = y;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;

	return (ret);
}

t_object create_rectangle_yz(t_vec y, t_vec z, double k, t_color color, int mat)
{
	t_object ret;

	ret.type = 5;
	ret.center = y;
	ret.dir = z;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;

	return (ret);
}

t_object create_rectangle_xz(t_vec x, t_vec z, double k, t_color color, int mat)
{
	t_object ret;

	ret.type = 6;
	ret.center = x;
	ret.dir = z;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;

	return (ret);
}