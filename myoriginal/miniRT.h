#ifndef MINIRT_H
#define MINIRT_H


#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "camera.h"
#include "vector.h"
#include "random.h"
#include "../libohw/includes/libft.h"
#include "../mlx/mlx.h"
#define MAX_DEPTH 50
//#define EPS 0.001
//#define ANTI 100

typedef struct s_discriminant
{
	double  Dsc;
	double  a;
    double  b;
    double  c;
    double  t1;
    double  t2;
}	t_discriminant;

typedef struct s_ray {
	t_point origin;
	t_vec dir;
} t_ray;

typedef struct s_hit_record {
	t_point p;
	t_vec normal;
	t_color color;
	double tmax;
	double tmin;
	double t;
	int front_face;
	int idx;
	int mat;
	int type;
	double refraction;
	double specular;
	double u;
	double v;
	int is_first;
} t_hit_record;

typedef struct s_ambient
{
	t_vec	col;
	double	ratio;
	int 	count;
}	t_ambient;

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
	double specular;
} t_objs;

typedef struct s_light
{
	t_vec			src;
	double			ratio;
	t_objs			object;
	t_ambient		ambient;
	int				count;
	struct s_light	*next;
}	t_light;

typedef struct	s_mlx
{
	void	*mlx;
	void	*mlx_win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_mlx;


typedef struct s_scene
{
	t_camera	camera;
	t_light		*light;
	t_objs	*world;
	t_ambient	ambient;
	int anti;
	int changed;
	double light_sample;
}	t_scene;

typedef struct s_minirt {
	t_mlx mlx;
	t_scene scene;
	t_ray ray;
	int is_move;
	int is_trace;
	
} t_minirt;

void path_render(t_minirt vars);


int		convert_rgb(int r, int g, int b);
int 	rgb_to_int(t_color c);
void	put_color(t_mlx *data, int x, int y, int color);
void 	ft_pixel_put(t_minirt *vars, int x, int y, int color);
void	ft_mlx_init(t_minirt *vars);
void	ft_mlx_new(t_minirt *vars, int x, int y, char *name);


int front_face(t_ray *r, t_hit_record* rec);
int find_hitpoint_path(t_ray* ray, t_objs *objs, t_light *light, t_hit_record* rec, int depth);
void hit_sphere(t_objs* s, t_ray* r, t_hit_record* rec);
void hit_cylinder(t_objs *cy, t_ray *ray, t_hit_record *rec);
void hit_caps(t_objs *cy, t_ray *ray, t_hit_record *rec);
void hit_plane(t_objs *pl, t_ray *ray, t_hit_record* rec);
void hit_rectangle_xy(t_objs *rect, t_ray *ray, t_hit_record* rec);
void hit_rectangle_yz(t_objs *rect, t_ray *ray, t_hit_record* rec);
void hit_rectangle_xz(t_objs *rect, t_ray *ray, t_hit_record* rec);



void set_refraction(t_objs* obj, double ref);
void set_specular(t_objs* obj, double spec);
double get_light_size(t_objs object);
t_objs create_sphere(t_point c, double r, t_color color, int mat);
t_objs create_cylinder(t_point c, double r, double h, t_vec dir, t_color color, int mat);
t_objs create_plane(t_point c, t_vec dir, t_color color, int mat);
t_objs create_rectangle_xy(t_vec x, t_vec y, double k, t_color color, int mat);
t_objs create_rectangle_yz(t_vec y, t_vec z, double k, t_color color, int mat);
t_objs create_rectangle_xz(t_vec x, t_vec z, double k, t_color color, int mat);


t_ray 	ray(t_point org, t_vec dir);
t_point 	ray_end(t_ray* ray, double t);
t_vec reflect(t_vec v, t_vec n);
t_color ray_color_2(t_ray r, t_objs* world, t_light* light);
t_color ray_color(t_ray r, t_objs* world, t_light* light, int depth);


int	keypress(int keycode, t_minirt* vars);
int	keyrelease(int keycode, t_minirt* vars);
int scroll(int mousecode, int x, int y, t_minirt* vars);
int key_hook_move(t_minirt* vars);
void key_press_move(t_minirt* vars, int keycode);
void key_press_rotate(t_minirt* vars, int keycode);
void key_press_mode_change(t_minirt* vars, int keycode);

#endif