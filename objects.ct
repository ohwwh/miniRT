#include "objects.h"

void set_refraction(t_objs* obj, double ref)
{
	obj->refraction = ref;
}

double get_light_size(t_objs object)
{
	const double rad = object.radius;
	const double x = object.center.y - object.center.x;
	const double y = object.dir.y - object.dir.x;

	if (object.type == 3)
		return (rad * rad * 3.1415926535897932385);
	else if (object.type == 4 || object.type == 5 || object.type == 6)
		return (x * y);
	else
	{
		printf("unsupported\n");
		return (-1);
	}
}

t_objs create_sphere(t_point c, double r, t_color color, int mat)
{
	t_objs ret;

    ret.type = 3;
	ret.center = c;
	ret.radius = r;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0.45;
	return (ret);
}

t_objs create_cylinder(t_point c, double r, double h, t_vec dir, t_color color, int mat)
{
	t_objs ret;

	ret.type = 2;
	ret.center = c;
	ret.radius = r;
	ret.height = h;
	ret.dir = dir;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;
	return (ret);
}

t_objs create_plane(t_point c, t_vec dir, t_color color, int mat)
{
	t_objs ret;

	ret.type = 1;
	ret.center = c;
	ret.dir = dir;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;
	return (ret);
}

t_objs create_rectangle_xy(t_vec x, t_vec y, double k, t_color color, int mat)
{
	t_objs ret;

	ret.type = 4;
	ret.center = x;
	ret.dir = y;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;

	return (ret);
}

t_objs create_rectangle_yz(t_vec y, t_vec z, double k, t_color color, int mat)
{
	t_objs ret;

	ret.type = 5;
	ret.center = y;
	ret.dir = z;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;

	return (ret);
}

t_objs create_rectangle_xz(t_vec x, t_vec z, double k, t_color color, int mat)
{
	t_objs ret;

	ret.type = 6;
	ret.center = x;
	ret.dir = z;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;

	return (ret);
}