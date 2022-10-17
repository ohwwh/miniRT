#ifndef RANDOM_H
#define RANDOM_H
#include <math.h>
#include <stdlib.h>
#include "vector.h"

typedef struct s_onb {
	t_vec u;
	t_vec v;
	t_vec w;
} t_onb;

typedef struct s_cosine_pdf {
	t_vec dir;
	double value;
} t_cosine_pdf;

double random_double(double min, double max, int anti);
t_vec rand_sphere();
t_vec rand_hemi_sphere(t_vec normal);
t_vec random_cosine_direction();
t_onb create_onb(t_vec n);
t_vec local(t_onb *onb, t_vec a);

#endif