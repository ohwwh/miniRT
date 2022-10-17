#ifndef FT_MLX_H
#define FT_MLX_H
#include <stdio.h>
#include "./mlx/mlx.h"
#include "camera.h"
#include "vector.h"
#include "objects.h"

typedef struct s_vars {
	void* mlx;
	void* win;
	int is_move;
	int is_trace;
	int window_height;
	int window_width;
	t_camera camera;
	t_object* world;
	t_object* light;
	int anti;
	int changed;
	
} t_vars;

typedef struct s_scene
{
	t_cam	cam;
	t_light	*light;
	t_amb	amb;
	t_objs	*objs;
}	t_scene;

typedef struct	s_minirt
{
	t_mlx		mlx;
	t_scene		scene;
	t_ray		ray;
	double		u;
	double		v;
}               t_minirt;

int 	rgb_to_int(t_color c);
void 	ft_pixel_put(t_vars *vars, int x, int y, int color);
void	ft_mlx_init(t_vars *vars);
void	ft_mlx_new(t_vars *vars, int x, int y, char *name);

#endif