#ifndef MINIRT_H
#define MINIRT_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include "ray.h"
#include "ft_mlx.h"
#include "objects.h"
#include "random.h"
#include "light.h"
#include "camera.h"
#include "move.h"
#include "../libft/libft.h"
#define MAX_DEPTH 50
//#define ANTI 100



void print_init(t_vars vars);

#endif