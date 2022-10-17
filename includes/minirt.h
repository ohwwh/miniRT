# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>

#include "../mlx/mlx.h"
#include "../libft/libft.h"

#include "camera.h"
#include "random.h"

# define PI 3.14159265358979323846
# define EPS 0.0001
# define LUMEN 3 
# define ROTATE 0.1
# define STEP 5
# define MAX_DEPTH 50

# define HEIGHT 600
# define WIDTH 900

# define CY 1
# define PL 2
# define SP 3

typedef enum s_bool{
	FALSE = 0,
	TRUE
} t_bool;

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
} t_keycode;

typedef struct	s_mlx
{
	void	*mlx;
	void	*mlx_win;
	int 	window_height;
	int 	window_width;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_mlx;

/*typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}	t_vec;*/

typedef struct t_camera
{
	t_vec	origin;
	t_vec	lookat;
	double	fov;
	double	ratio;
	double	height;
	double 	width;
	t_vec 	forward;
	t_vec	vup;
	t_vec	up;
	t_vec	right;
	int		count;
	t_vec lower_left_corner;
	t_vec horizontal;
	t_vec vertical;
}	t_camera;

typedef struct s_amb
{
	t_vec	col;
	double	ratio;
	int 	count;
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
	int				refraction;
	int 			specular;
}	t_objs;

typedef struct s_light
{
	t_vec			src;
	double			ratio;
	t_objs			*object;
	struct s_light	*next;
}	t_light;

typedef struct s_scene
{
	t_camera	camera;
	t_light	*light;
	t_amb	amb;
	t_objs	*objs;
	int anti;
	int changed;
}	t_scene;

typedef struct s_ray
{
	t_vec origin;
	t_vec dir;
	t_vec color;
} t_ray;

typedef struct s_hit_hit_record
{
    t_vec       p; // 교점
    t_vec       normal; // 법선
    double      tmin;
    double      tmax;
    double      t;
    t_bool      front_face; // 객체가 카메라 앞에 있는지
	t_vec		color;
	int			mat;
	int			refraction;
	int			specular;
	int			type;
} t_hit_record;

typedef struct	s_minirt
{
	t_mlx		mlx;
	t_scene		scene;
	t_ray		ray;
	int is_move;
	int is_trace;
	double		u;
	double		v;
}	t_minirt;

int		check_file(int ac, char **av);
void 	err_handler(char *msg);

t_vec	get_color(char *s);
t_vec	get_vec(char *s);
void	parse_line(char *id, char **tockens, t_scene *sc);
void	parse(t_scene *sc, int fd);

void	parse_sphere(t_scene *sc, char **tockens);
void	parse_cylinder(t_scene *sc, char **tockens);
void	parse_plane(t_scene *sc, char **tockens);

t_light	*alloc_light(t_scene *sc);
void	parse_ambient(t_scene *sc, char **tokens);
void	parse_camera(t_scene *sc, char **tokens);
void	parse_light(t_scene *sc, char **tokens);

double	ft_atod(const char *str);
void	free_split(char **s);

void print_scene(t_scene sc);

void	rt_render(t_minirt *data);

t_vec create_vec(double x, double y, double z);
void set_vec(t_vec *vec, double x, double y, double z);
double vec_len(t_vec vec);
t_vec vec_sum(t_vec vec1, t_vec vec2);
t_vec      vec_sub(t_vec vec, t_vec vec2);
t_vec vec_scalar_mul(t_vec vec, double s);
t_vec   vec_mul(t_vec vec1, t_vec vec2);
t_vec      vec_division(t_vec vec, double t);
double  vdot(t_vec vec, t_vec vec2);
t_vec  vcross(t_vec vec1, t_vec vec2);
t_vec      unit_vec(t_vec vec);
t_vec  vmin(t_vec vec1, t_vec vec2);

void set_camera_param(t_camera *cam);
t_ray       ray_primary(t_camera *cam, double u, double v);

t_vec get_raycolor(t_minirt *data);
t_vec calcul_ratio(t_vec col1, t_vec col2, double ratio);
t_vec	calcul_color(t_scene *sc, t_hit_record hr, t_vec amb, t_ray ray);

t_hit_record find_hitpoint(t_ray *ray, t_objs *objs);
t_hit_record hit_plane(t_hit_record saved, t_ray *ray, t_objs *pl);
t_hit_record hit_sphere(t_hit_record saved, t_ray *ray, t_objs *sp);
t_hit_record hit_cylinder(t_hit_record saved, t_ray *ray, t_objs *cy);
t_hit_record hit_caps(t_hit_record saved, t_ray *ray, t_objs *cy);

int	keybind(int keycode, t_minirt *data);
int	ft_close(t_minirt *data);



void path_render(t_minirt vars);


int 	rgb_to_int(t_color c);
void	put_color(t_mlx *data, int x, int y, int color);
void 	ft_pixel_put(t_minirt *vars, int x, int y, int color);
void	ft_mlx_init(t_minirt *vars);
void	ft_mlx_new(t_minirt *vars, int x, int y, char *name);


int front_face(t_ray *r, t_hit_record* rec);
int find_hitpoint_hoh(t_ray* ray, t_objs *objs, t_light *light, t_hit_record* rec);
int hit_sphere_hoh(t_objs* s, t_ray* r, t_hit_record* rec);
int hit_cylinder_hoh(t_objs *cy, t_ray *ray, t_hit_record *rec);
int hit_caps_hoh(t_objs *cy, t_ray *ray, t_hit_record *rec);
int hit_plane_hoh(t_objs *pl, t_ray *ray, t_hit_record* rec);
int hit_rectangle_xy(t_objs *rect, t_ray *ray, t_hit_record* rec);
int hit_rectangle_yz(t_objs *rect, t_ray *ray, t_hit_record* rec);
int hit_rectangle_xz(t_objs *rect, t_ray *ray, t_hit_record* rec);



void set_refraction(t_objs* obj, double ref);
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