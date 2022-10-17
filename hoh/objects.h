#ifndef OBJECTS_H
#define OBJECTS_H
#include "vector.h"

typedef struct s_object {
	int				type;
	t_point			center;
	t_vec			dir;
	double 			radius;
	double 			height;
	t_color			color;
	t_vec			norm;
	struct s_object	*next;
    int mat;
	double refraction;
} t_object;

void set_refraction(t_object* obj, double ref);
t_object create_sphere(t_point c, double r, t_color color, int mat);
t_object create_cylinder(t_point c, double r, double h, t_vec dir, t_color color, int mat);
t_object create_plane(t_point c, t_vec dir, t_color color, int mat);
t_object create_rectangle_xy(t_vec x, t_vec y, double k, t_color color, int mat);
t_object create_rectangle_yz(t_vec y, t_vec z, double k, t_color color, int mat);
t_object create_rectangle_xz(t_vec x, t_vec z, double k, t_color color, int mat);

#endif