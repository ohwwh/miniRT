#ifndef RAY_H
#define RAY_H
#include "vector.h"
#include "objects.h"
#include "random.h"
#include "hit.h"

typedef struct s_ray {
	t_point origin;
	t_vec dir;
} t_ray;

typedef struct s_record {
	t_point p;
	t_vec normal;
	t_color color;
	double t_max;
	double t_min;
	double t;
	int front_face;
	int idx;
	int mat;
	double refraction;
	double u;
	double v;
} t_record;

t_ray 	ray(t_point org, t_vec dir);
t_point 	ray_end(t_ray* ray, double t);
t_vec reflect(t_vec v, t_vec n);
t_color ray_color_2(t_ray r, t_object* world, t_object* light);
t_color ray_color(t_ray r, t_object* world, t_object* light, int depth);

#endif