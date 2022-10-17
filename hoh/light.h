#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"
#include "ray.h"
#include "hit.h"

typedef struct s_light
{
	t_color color;
	t_point origin;
} t_light;

t_light create_light(t_color color, t_point origin);
t_color point_light_get(t_ray* ray, t_record* rec, t_light* light);

#endif