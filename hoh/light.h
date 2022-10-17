#ifndef LIGHT_H
#define LIGHT_H
#include "vector.h"
#include "ray.h"
#include "hit.h"

typedef struct s_light
{
	t_vec			src;
	double			ratio;
	struct s_light	*next;
}	t_light;

#endif