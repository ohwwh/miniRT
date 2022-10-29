/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 18:37:08 by hako              #+#    #+#             */
/*   Updated: 2022/10/29 15:11:12 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H 

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <pthread.h>
# include <time.h>

# include "../mlx/mlx.h"
# include "../libft/libft.h"

# include "random.h"

# define PI 3.14159265358979323846
# define EPS 0.001
# define LUMEN 3 
# define ROTATE 0.1
# define STEP 5
# define MAX_DEPTH 30
# define KSN 64 
# define KS 0.5

# define WIDTH 800
# define HEIGHT 600

# define ANTI 111
# define LT	0.5

# define CAM_SPEED 30

# define CY 1
# define PL 2
# define SP 3
# define RCXY 4
# define RCYZ 5
# define RCXZ 6

# define TH 3

typedef struct s_minirt t_minirt;

typedef enum s_bool{
	FALSE = 0,
	TRUE
}	t_bool;

typedef enum s_keycode{
	ESC = 53,
	ZERO = 29,
	ONE = 18,
	TWO = 19,
	THREE = 20,
	FOUR = 21,
	FIVE = 23,
	SIX = 22,
	W = 13,
	A = 0,
	S = 1,
	D = 2,
	UP = 126,
	LEFT = 123,
	DOWN = 125,
	RIGHT = 124
}	t_keycode;

typedef struct s_mlx
{
	void	*mlx;
	void	*mlx_win;
	int		window_height;
	int		window_width;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_mlx;

typedef struct s_amb
{
	t_vec	col;
	double	ratio;
	int		count;
}	t_amb;

typedef struct s_objs
{
	int				type;
	t_vec			center;
	t_vec			dir;
	double			radius;
	double			height;
	t_vec			color;
	struct s_objs	*next;
	int				mat;
	double			refraction;
	double			specular;
	double			fuzzy;
}	t_objs;

typedef struct s_camera
{
	t_vec	origin;
	t_vec	dir;
	double	distance;
	double	fov;
	double	ratio;
	double	viewport_height;
	double	viewport_width;
	t_vec	forward;
	t_vec	vup;
	t_vec	up;
	t_vec	right;
	int		count;

}	t_camera;

typedef struct s_light
{
	t_vec			src;
	double			ratio;
	double			distance;
	t_objs			object;
	int				count;
	struct s_light	*next;
}	t_light;

typedef struct s_scene
{
	t_camera	camera;
	t_light		*light;
	t_amb		amb;
	t_objs		*objs;
	int			anti;
	int			changed;
}	t_scene;

typedef struct s_ray
{
	t_vec	origin;
	t_vec	dir;
	t_vec	color;
}	t_ray;

typedef struct s_hit_record
{
	t_vec		p;
	t_vec		normal;
	double		tmin;
	double		tmax;
	double		t;
	t_bool		front_face;
	t_vec		color;
	int			mat;
	double		refraction;
	double		specular;
	double		fuzzy;
	int			type;
}	t_hit_record;

typedef struct s_shared
{
	pthread_mutex_t	mutex;
	t_minirt		*vars;
	int				sampling;
	int				working;
	int				work[TH];
	int				x;
	int				y;
} t_shared;

typedef struct s_thread
{
	int 		thr_num;
	pthread_t 	thr;
	t_color		color;
	t_shared	*sh;
} t_thread;

typedef struct s_minirt
{
	t_mlx		mlx;
	t_scene		scene;
	t_ray		ray;
	int			is_move;
	int			is_trace;
	int			mode;
	double		u;
	double		v;
	int			x;
	int			y;
	t_thread	thr[TH];
}	t_minirt;

typedef struct s_discriminant
{
	double	dsc;
	double	a;
	double	b;
	double	c;
	double	t1;
	double	t2;
}	t_discriminant;

void			set_camera(t_camera *cam);

int				check_file(int ac, char **av);
void			err_handler(char *msg);

t_bool			is_valid_color(char *s);
t_vec			make_vec(double x, double y, double z);

void			set_init_distance(t_minirt *data);
void			init_rt(t_minirt *data);
int				create_light_object(t_scene *sc);

t_vec			get_color(char *s);
t_vec			get_vec(char *s);
void			parse_line(char *id, char **tockens, t_scene *sc);
void			parse(t_scene *sc, int fd);

void			parse_sphere(t_scene *sc, char **tockens);
void			parse_cylinder(t_scene *sc, char **tockens);
void			parse_plane(t_scene *sc, char **tockens);
void			parse_light_sphere(t_scene *sc, char **tokens);
void			parse_light_rectangle_xy(t_scene *sc, char **tokens);
void			parse_light_rectangle_yz(t_scene *sc, char **tokens);
void			parse_light_rectangle_xz(t_scene *sc, char **tokens);
void			parse_rectangle_xy(t_scene *sc, char **tokens);
void			parse_rectangle_yz(t_scene *sc, char **tokens);
void			parse_rectangle_xz(t_scene *sc, char **tokens);

t_light			*alloc_light(t_scene *sc);
void			parse_ambient(t_scene *sc, char **tokens);
void			parse_camera(t_scene *sc, char **tokens);
void			parse_light(t_scene *sc, char **tokens);
int				create_light_object(t_scene *scene);

double			ft_atod(const char *str);
void			free_split(char **s);

void			print_scene(t_scene sc);

void			rt_render(t_minirt *data);

t_vec			create_vec(double x, double y, double z);
void			set_vec(t_vec *vec, double x, double y, double z);
double			vec_len(t_vec vec);
t_vec			vec_sum(t_vec vec1, t_vec vec2);
t_vec			vec_sub(t_vec vec, t_vec vec2);
t_vec			vec_scalar_mul(t_vec vec, double s);
t_vec			vec_mul(t_vec vec1, t_vec vec2);
t_vec			vec_division(t_vec vec, double t);
double			vdot(t_vec vec, t_vec vec2);
t_vec			vcross(t_vec vec1, t_vec vec2);
t_vec			unit_vec(t_vec vec);
t_vec			vmin(t_vec vec1, t_vec vec2);

t_ray			ray(t_point origin, t_vec dir);
t_ray			ray_primary(t_camera *cam, double u, double v);
t_point			ray_end(t_ray *ray, double t);

t_vec			get_raycolor(t_minirt *data);
t_vec			add_color(t_vec col1, t_vec col2);
t_vec			calcul_ratio(t_vec col1, t_vec col2, double ratio);
t_vec			calcul_color(t_scene *sc, t_hit_record hr,
					t_vec amb, t_ray ray);

t_vec			diffuse(t_hit_record hr, t_light *light, double d);
t_vec			specular(t_hit_record hr, t_light *light, t_ray ray);
int				shadow(t_scene *sc, t_hit_record hr, t_light *light);

t_color			ray_color(t_ray r, t_scene *sc, int depth);
t_color			ray_color_raw(t_ray r, t_scene *sc);

double			scattering_pdf(t_ray *scattered, t_hit_record *rec);
double			scatter(t_ray *r, t_hit_record *rec,
					t_ray *scattered, t_light *light);

void			generate_scattered(t_hit_record *rec,
					t_ray *scattered, t_onb *uvw);
void			generate_light_sample_sphere(
					t_hit_record *rec, t_ray *scattered, t_objs *light);
void			generate_random_importance(t_hit_record *rec,
					t_ray *scattered, t_light *temp, t_onb *uvw);

t_vec			random_to_sphere(double radius, double distance_squared);
double			cosine_pdf(const t_vec *dir, const t_vec *w);
double			sphere_light_pdf(t_hit_record *rec,
					t_ray *scattered, t_objs *light);
double			get_pdf(t_hit_record *rec,
					t_ray *scattered, t_light *light, t_onb *uvw);
double			mixture_pdf_value(t_hit_record *rec,
					t_ray *scattered, t_light *light);

t_hit_record	find_hitpoint(t_ray *ray, t_objs *objs);
int				find_hitpoint_path(t_ray *ray, t_objs *objs,
					t_light *light, t_hit_record *rec);
void			hit_plane(t_objs *pl, t_ray *ray, t_hit_record *rec);
void			hit_sphere(t_objs *s, t_ray	*r, t_hit_record *rec);
void			hit_cylinder(t_objs *cy, t_ray *ray, t_hit_record *rec);
void			hit_caps(t_objs *cy, t_ray *ray, t_hit_record *rec);
void			hit_rectangle_xy(t_objs *rect, t_ray *ray, t_hit_record *rec);
void			hit_rectangle_yz(t_objs *rect, t_ray *ray, t_hit_record *rec);
void			hit_rectangle_xz(t_objs *rect, t_ray *ray, t_hit_record *rec);
void			set_face_normal(t_hit_record *rec,
					t_ray *ray, t_vec outward_normal);

t_objs 			create_sphere(t_point c, double r, t_color color, int mat);
t_objs 			create_cylinder(t_point c, double r, double h, t_vec dir, t_color color, int mat);
t_objs 			create_plane(t_point c, t_vec dir, t_color color, int mat);
t_objs 			create_rectangle_xy(t_vec x, t_vec y, double k, t_color color, int mat);
t_objs 			create_rectangle_yz(t_vec y, t_vec z, double k, t_color color, int mat);
t_objs 			create_rectangle_xz(t_vec x, t_vec z, double k, t_color color, int mat);

void			set_record(t_objs *s, t_ray *r, t_hit_record *rec, double root);

int				keypress(int keycode, t_minirt *vars);
int				keyrelease(int keycode, t_minirt *vars);
int				scroll(int mousecode, int x, int y, t_minirt *vars);
int				key_hook_move(t_minirt *vars);
void			key_press_move(t_minirt *vars, int keycode);
void			key_press_rotate(t_minirt *vars, int keycode);
void			key_press_mode_change(t_minirt *vars, int keycode);
int				ft_close(t_minirt *data);

void			object_move(t_minirt *data, int type);
void			object_rotate(t_minirt *data, int type);
void			camera_move(t_minirt *vars);
void			camera_rotate(t_minirt *vars);
void			camera_zoom(t_minirt *vars);

double			reflectance(double cos, double ref_ratio);
t_vec			reflect(t_vec v, t_vec n);
t_vec			refract(t_vec v, t_vec n, double e, double cos);

void			raw_render(t_minirt *v);
void			path_render(t_minirt *vars);
int				convert_rgb(int r, int g, int b);

int				rgb_to_int(t_color c);
t_color			get_sky_color(t_ray r);
void			put_color(t_mlx *data, int x, int y, int color);
void			ft_pixel_put(t_minirt *vars, int x, int y, int color);
void			ft_mlx_init(t_minirt *vars);
void			ft_mlx_new(t_minirt *vars, int x, int y, char *name);

double			get_light_size(t_objs object);
double			clamp(double x);
void 			firefly(t_vec *color);

void			path_render_threaded(t_minirt *vars);

#endif